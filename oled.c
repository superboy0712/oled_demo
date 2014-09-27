/*****************************************************************
 * oled.c
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/
#include "pin_layout.h"
#include "mem_map.h"
#include "helpFile.h"
#include "font_5x7.h"
#include "util/delay.h"
#include <stdio.h>
/*					RD	WR	CS	DC
 * 	Write command 	H 	↑ 	L 	L
*	Write data 		H 	↑ 	L	H
*/
#ifdef _LCM_12864
#define WR_HIGH set_bit(OLED_WR_PORT, OLED_WR_BIT)
#define WR_LOW clr_bit(OLED_WR_PORT, OLED_WR_BIT)
#define CS_HIGH set_bit(OLED_CS_PORT, OLED_CS_BIT)
#define CS_LOW clr_bit(OLED_CS_PORT, OLED_CS_BIT)
#define DC_HIGH set_bit(OLED_DC_PORT, OLED_DC_BIT)
#define DC_LOW clr_bit(OLED_DC_PORT, OLED_DC_BIT)
#endif
// some Marcos
#define START_COL 0
#define END_COL 127
#define START_PAGE 0
#define END_PAGE 7
#define LINE_LENGTH (END_COL - START_COL + 1)
#define CHA_WIDTH 5
#define MAX_CHARS_A_LINE (LINE_LENGTH/CHA_WIDTH)
// global var, representing the position.
volatile static uint8_t current_col_address = 0;// from from 0 to END_COL - START_COL
volatile static uint8_t current_pag_address = 0;// from from 0 to END_PAG - START_PAGE
// buffer ram, possibly locate in ext sram
uint8_t oled_disp_buffer[128];
// forward declaration
void oled_init(void);
void oled_home(void);
void oled_goto_xy(uint8_t x,uint8_t y);
void oled_goto_nextln(void);
void oled_clear(void);
void oled_putchar( const char c);
void oled_putchar_inverse(char c);
void oled_putchar_buffer(char c);
void oled_buffer_update(void);
void oled_buffer_wr(uint8_t col, uint8_t row, uint8_t *data, uint8_t length);
void oled_putstr( const char * str);
void oled_wr_d(uint8_t data);
void oled_wr_cmd(uint8_t cmd);
void oled_set_normal(void);
void oled_set_inverse(void);// whole
int oled_putchar_printf(char var, FILE *stream);
//
#ifdef _LCM_12864
void oled_wr_cmd(uint8_t cmd){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_CMD_START;
	*addr = cmd;*/
	CS_LOW;
	DC_LOW;
	WR_LOW;
	asm("nop");
	asm("nop");

	OLED_DATA_PORT = cmd;
	asm("nop");
	asm("nop");

	WR_HIGH;
	asm("nop");
}

void oled_wr_d(uint8_t data){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_DATA_START;
	*addr = data;*/
	CS_LOW;
	DC_HIGH;
	WR_LOW;
	asm("nop");
	asm("nop");

	OLED_DATA_PORT = data;
	asm("nop");
	asm("nop");

	WR_HIGH;
	asm("nop");
}
#else
void oled_wr_cmd(uint8_t cmd){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_CMD_START;
	*addr = cmd;*/
	char * const addr = OLED_ADDR_CMD_START;
	*addr = cmd;
}

void oled_wr_d(uint8_t data){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_DATA_START;
	*addr = data;*/
	char * const addr = OLED_ADDR_DATA_START;
		*addr = data;
}
#endif

void oled_init(void){
#ifdef _LCM_12864
	// pin configuration
	OLED_DATA_DDR = 0xff;
	set_bit(OLED_CS_DDR, OLED_CS_BIT);
	set_bit(OLED_WR_DDR, OLED_WR_BIT);
	set_bit(OLED_DC_DDR, OLED_DC_BIT);
	//
#else
	// enable xem function.
	MCUCR |= (1<<SRE);        //Enable External Memory interface
	SFIOR |= (1<<XMM2);       //Mask out higher 4 bits of address
#endif
	oled_wr_cmd(0xae); // display off
	oled_wr_cmd(0xa1); //segment remap
	oled_wr_cmd(0xda); //common pads hardware: alternative
	oled_wr_cmd(0x12);
	oled_wr_cmd(0xc8); //common output scan direction:com63~com0
	oled_wr_cmd(0xa8); //multiplex ration mode:63
	oled_wr_cmd(0x3f);
	oled_wr_cmd(0xd5); //display divide ratio/osc. freq. mode
	oled_wr_cmd(0x80);
	oled_wr_cmd(0x81); //contrast control
	oled_wr_cmd(0xff); // highest contrast
	oled_wr_cmd(0xd9); //set pre-charge period
	oled_wr_cmd(0x22);
	//
/*
	oled_wr_cmd(0x20); //Set Memory Addressing Mode
	oled_wr_cmd(0x10); // page mode
	oled_wr_cmd(0x00);
	oled_wr_cmd(0x1f);
	oled_wr_cmd(0x10); // page mode
*/

	//
	oled_wr_cmd(0x20); //Set Memory Addressing Mode
	oled_wr_cmd(0x00); // page mode
	/*oled_wr_cmd(0x21); // column
	oled_wr_cmd(4);
	oled_wr_cmd(123);
	//
	oled_wr_cmd(0x22); // page
	oled_wr_cmd(0);
	oled_wr_cmd(7);
	*/
//	oled_wr_cmd(0x40); // start line


	oled_wr_cmd(0xdb); //VCOM deselect level mode
	oled_wr_cmd(0x30);
	oled_wr_cmd(0xad); //master configuration
	oled_wr_cmd(0x00);
	oled_wr_cmd(0xa4); //out follows RAM content
	oled_wr_cmd(0xa6); //set normal display
	oled_wr_cmd(0xaf); // display on
	uint8_t i;
	for(i = 0; i<128; i++)
		oled_disp_buffer[i] = 0;
}

void oled_goto_xy(uint8_t col,uint8_t row){
	// input: 0 to max - min.
	current_col_address = col;
	current_pag_address = row;
//	oled_wr_cmd(0x40);
	oled_wr_cmd(0x21);// set col address
	oled_wr_cmd(col + START_COL);
	oled_wr_cmd(END_COL);
	oled_wr_cmd(0x22);// set pag address
	oled_wr_cmd(row + START_PAGE);
	oled_wr_cmd(END_PAGE);
}

void oled_goto_nextln(void){
	// input: 0 to max-min
	current_pag_address++;
	if(current_pag_address > END_PAGE - START_PAGE)
		current_pag_address = 0;
	oled_goto_xy(0, current_pag_address);
}

void oled_putchar( const char c){
	int i;
	const char j = (c-' ');
	current_col_address+=CHA_WIDTH;

	if(current_col_address/CHA_WIDTH >= MAX_CHARS_A_LINE-2){
		//current_col_address = 0;
		oled_goto_nextln();
	}


	for(i = 0; i < 5; i++){
		// pay attention, progmem read
		 oled_wr_d(pgm_read_byte(&font[(int)j][i]));
	}
}

void oled_set_inverse(void){
	oled_wr_cmd(0xa7);
}

void oled_set_normal(void){
	oled_wr_cmd(0xa6);
}

void oled_putstr( const char * str){
	while(*str)
		oled_putchar(*str++);
}
void oled_clear(void){
	current_col_address = 0;
	current_pag_address = 0;
	oled_wr_cmd(0xae); // off
	//
	oled_wr_cmd(0x21);
	oled_wr_cmd(1);
	oled_wr_cmd(126);
	oled_wr_cmd(0x22);
	oled_wr_cmd(0);
	oled_wr_cmd(7);
	oled_wr_cmd(0x40);
	int temp;
	for(temp = 0; temp < 8*126; temp++){
		oled_wr_d(0x00);
	}
	oled_wr_cmd(0x21);
	oled_wr_cmd(START_COL);
	oled_wr_cmd(END_COL);
	oled_wr_cmd(0x22);
	oled_wr_cmd(START_PAGE);
	oled_wr_cmd(END_PAGE);
	oled_wr_cmd(0x7c);
	oled_wr_cmd(0xaf); // on
}

int oled_putchar_printf(char var, FILE *stream){
	if(var == '\r'||var =='\n'){
		oled_goto_nextln();
		return 0;
	}

	oled_putchar(var);
	return 0;
}

void oled_putchar_inverse(char c){
	oled_putchar(~c);
}

void oled_buffer_wr(
		uint8_t col,
		uint8_t row,
		uint8_t *data,
		uint8_t length){
	if(length<0 || length > (128 - row*16- col + 1))
		return;
	while(length--){
		oled_disp_buffer[row*16+col] = *data;
		data++;
	}
}
void oled_buffer_update(void){
	uint8_t i;
	// maybe need some aligning modifications
	for (i = 0; i<128; i++){
		oled_wr_d(oled_disp_buffer[i]);
	}
}

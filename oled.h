/*****************************************************************
 * oled.h
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef OLED_H_
#define OLED_H_
#include <inttypes.h>
#include <stdio.h>
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
#endif /* OLED_H_ */

/*****************************************************************
 * mem_map.h
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef MEM_MAP_H_
#define MEM_MAP_H_

#define OLED_ADDR_CMD_START		(void*)0x1000
#define OLED_ADDR_CMD_END		(void*)0x11ff
#define OLED_ADDR_DATA_START	(void*)0x1200
#define OLED_ADDR_DATA_END		(void*)0x13ff
#define ADC_ADDR_START			(void*)0x1400
#define ADC_ADDR_END			(void*)0x17ff
#define SRAM_ADDR_START			(void*)0x1800
#define SRAM_ADDR_END			(void*)0x1fff

#endif /* MEM_MAP_H_ */

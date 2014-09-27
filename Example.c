#define F_CPU 4915200UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "oled.h"
#include "MicroMenu.h"


/*** DUMY CODE ***/
enum ButtonValues
{
	BUTTON_NONE,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
};

enum ButtonValues GetButtonPress(void)
{
	return BUTTON_NONE;
};
/*** END DUMY CODE ***/


/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
	puts("ENTER");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
	puts("SELECT");
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */

MENU_ITEM(Menu_1, Menu_2, Menu_3, NULL_MENU, Menu_1_1,  Level1Item1_Select, Level1Item1_Enter, "hello 1");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU, NULL, NULL, "not bad 2");
MENU_ITEM(Menu_3, Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL, NULL, "awesome 3");

MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "1.2");

int main(void)
{	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
	oled_init();
	//Menu_SetGenericWriteCallback(oled_putstr);
	//Menu_Navigate(&Menu_1);
	oled_goto_nextln();
	oled_putstr("hello world!");
	oled_goto_nextln();
	oled_putstr("hello world!");
	oled_goto_nextln();
	oled_putstr("hello world!");
	_delay_ms(100);
    while (1)
    { 
		int i;
		for (i = 0; i<= 50;i++){
			_delay_ms(50);
			oled_wr_d(0xf0);
		}
		oled_wr_cmd(0xa7);
		_delay_ms(1000);
		oled_wr_cmd(0xa6);
		_delay_ms(1000);
		oled_clear();
		#define START_COL 20
		#define END_COL 100
		#define START_PAGE 0
		#define END_PAGE 7
		#define LINE_LENGTH (END_COL - START_COL + 1)
		#define CHA_WIDTH 5
		#define MAX_CHARS_A_LINE (LINE_LENGTH/CHA_WIDTH)
			oled_wr_cmd(0xae); // off
			//
			oled_wr_cmd(0x20);
			oled_wr_cmd(0x02);
			oled_wr_cmd(0x21);
			oled_wr_cmd(12);
			oled_wr_cmd(100);
			oled_wr_cmd(0x22);
			oled_wr_cmd(0);
			oled_wr_cmd(7);
			oled_wr_cmd(0x49);
			int temp;
			for(temp = 0; temp < 8*256; temp++){
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
		/* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
		 * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
		 */
		/*
		switch (GetButtonPress())
		{
			case BUTTON_UP:
				Menu_Navigate(MENU_PREVIOUS);
				break;
			case BUTTON_DOWN:
				Menu_Navigate(MENU_NEXT);
				break;
			case BUTTON_LEFT:
				Menu_Navigate(MENU_PARENT);
				break;
			case BUTTON_RIGHT:
				Menu_Navigate(MENU_CHILD);
				break;
			case BUTTON_ENTER:
				Menu_EnterCurrentItem();
				break;
			default:
				break;
		}
		*/
    }
}

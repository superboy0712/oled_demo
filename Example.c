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
static void genric_draw_fun(uint8_t x, uint8_t y, const char *str){
	oled_goto_xy(x,y);
	oled_putstr_P(str);
}
/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
	oled_putstr_inverse("\nSELECTjkjlj");
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */

MENU_ITEM( Menu_1, (50), (3), Menu_2, Menu_3, NULL_MENU, NULL_MENU, NULL, NULL, "I love Zhang Yun!\n");
MENU_ITEM( Menu_2, (50), (4), Menu_3, Menu_1, NULL_MENU, NULL_MENU, NULL, NULL, "I am Second\n");
MENU_ITEM( Menu_3, (50), (5), Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL, NULL, "I am Third\n");
//MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");
//MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "1.2");

int main(void)
{	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
		oled_init();
		oled_clear();
		oled_putstr_inverse("hello world!!\n");
	Menu_SetGenericWriteCallback(genric_draw_fun);
	Menu_Navigate(&Menu_1);
	Menu_Navigate(MENU_NEXT);
	Menu_Navigate(MENU_NEXT);
	while (1)
    { 
	
	
		/* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
		 * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
		 */
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
		
    }
}

/*
 */

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
    LCD_init();
	LCD_displayStringRowColumn(0,2,"My LCD Driver");
	LCD_displayStringRowColumn(1,3,"Embedded WS");
	_delay_ms(4000);

	LCD_clearScreen();
	LCD_displayString("Tasneem");
    LCD_displayStringRowColumn(1,5,"Afify ");
	LCD_intgerToString(15);



DDRD |= (1<<PD7);
PORTD |= (1<<PD7);

while(1)
{
    PORTD |= (1<<PD7);
    _delay_ms(250);
    PORTD &= ~(1<<PD7);
    _delay_ms(250);
}


    return 0;
}

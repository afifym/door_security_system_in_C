/*
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include "keypad.h"
#include "lcd.h"

extern count_a;
void welcome_screen();

unsigned char access_allowed = 0;

int main(void)
{
DDRD |= (1<<3) | (1<<4);

adc0_init();
PWM_init0();
LCD_init();

Usonic_init();
_delay_ms(30);

while(1)
{

Usonic_trigger();  // send an ultrasonic trigger, reflection is measured with an interrupt

if(count_a<60)    // wait till the distance becomes 6cm
{
    welcome_screen();
    default_screen();
    while(1)
    {
        access_allowed = keypad_entry();
    }

}


}


    return 0;
}



void welcome_screen()  // Display and shift Welcome!
{
    unsigned char wel[22] = "!emocleW                 ";
    unsigned char a=0, s=0, d=0;

for(a=1 ; a<25 ;a++)
    {
    LCD_clearScreen();
    LCD_goToRowColumn(0,0);
    d = a;
    for(s=0 ; s<16 ;s++)
        {
        LCD_displayCharacter(wel[d-1]);
        d--;
        if(d==0) break;
        }
        _delay_ms(100);
    }
=
}

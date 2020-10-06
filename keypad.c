
#include "keypad.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

const unsigned char keyMap[16] = {0xEE, 0xED, 0xEB, 0xE7, 0xDE, 0xDD, 0xDB, 0xD7, 0xBE, 0xBD, 0xBB, 0xB7, 0x7E, 0x7D, 0x7B, 0x77};
const unsigned char arr[16] = {'#','*','0','=',
                                '~', '9','8','7',
                                '-', '6','5','4',
                                '+', '3','2','1'};

unsigned char key, entered[6], password[6] = "123456";
unsigned char double_time=1, enter_request=0, pass_change=0, request_change=0, change_allowed=0,  triers=0;
unsigned int countMe = 0;
unsigned char i=0, pos=0, nCorrect=0;

void key_capture()
{
// Change Port Here
DDRC = 0x0F; PORTC = 0xF0; _delay_us(5); key = PINC;
DDRC = 0xF0; PORTC = 0x0F; _delay_us(5); key |= PINC;
}

unsigned char keypad_entry()
{
DDRD |= (1<<6);
TCCR0 = (1<<WGM01);

while(1)
{

key_capture();

if(key != 0xFF)
    for(i=0; i<16;i++)
        if(keyMap[i] == key)
        {

            countMe = 0;
            TCNT0 = 0;
            TCCR0 = (1<<CS02) | (1<<CS00);           // Start the timer
            while(keyMap[i] == key)
            {

                key_capture();
                //----------------------------------- Record the time of each press
                if(TCNT0 == 47)
                {
                    countMe++;
                }
                if(countMe >= 5)  //3000
                {
                    request_change = 1;
                }
                //-----------------------------------
            }
            TCCR0 = 0;                              // Stop the timer

            if(arr[i] == '*')  // delete one character
            {
                LCD_goToRowColumn(1,pos-1);
                LCD_displayCharacter(' ');
                LCD_goToRowColumn(1,pos-2);
                pos--;
                continue;
            }

            if(arr[i] == '#')
            {
                if((request_change == 0) || (pass_change == 1)) // check if the user wants to test what he entered
                {
                    enter_request=1;
                    break;
                }
                else if( (request_change == 1) && (pass_change == 0) )  // check if the user wants to change his password
                {
                    password_change();
                    return 0;
                }
            }


            LCD_goToRowColumn(1,pos);
            LCD_displayCharacter('*');
            entered[pos] = arr[i];

            pos++;
            break;
        }

// Check The Entered Password
if((enter_request==1) & (pos==6))
{
    enter_request = 0;
    pos = 0;
    nCorrect=0;

    if(change_allowed == 1)  // change_allowed=1 in password_change()
    {
        for(i=0;i<6;i++) password[i] = entered[i];
        return 0;
    }

    for(i=0; i<6;i++) if(entered[i] == password[i]) nCorrect++;   // test the entered password

    if((nCorrect==6) & (request_change==0)){granted(); return 1;} // granted if correct password
    else if((nCorrect==6) & (request_change==1)) return 2;        // return 2 to password_change()
    else if((nCorrect!=6) & (request_change==1)) return 3;        // Entered wrong password when attempting password change
    else {denied(); return 0;}                                    // denied if incorrect password

}
}}


void granted()
{
    LCD_displayStringRowColumn(1,0,"Granted!");
    _delay_ms(500);
    motor_forward();
    //Check limit switch in a while loop
    //Wait 10 seconds
    _delay_ms(4000);
    motor_backward();
    //Check limit switch in a while loop
    _delay_ms(4000);
    motor_stop();
}

void denied()
{
    triers++;

/*
Send entered[6] and triers to the EEPROM, however, the eeprom gives an error in hardware,
but works in protous simulation with the same code
*/

    if(triers == 3)
    {
        triers = 0;
        LCD_displayStringRowColumn(1,0,"Wait!");
        _delay_ms(5000 / double_time);
        default_screen();
        double_time = double_time*2;
    }
    else
    {
        LCD_displayStringRowColumn(1,0,"Try Again!");
        _delay_ms(500);
        default_screen();
    }

}

void default_screen()
{
LCD_clearScreen();
LCD_displayStringRowColumn(0,0,"Enter Password:");
LCD_goToRowColumn(1,0);
}

void password_change()
{
    pass_change = 1;
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Old Password:");
    LCD_goToRowColumn(1,0);

    if(keypad_entry()==2)
    {
        change_allowed = 1;
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"New Password:");
        LCD_goToRowColumn(1,0);

        change_allowed = keypad_entry();

        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Password Updated");
        LCD_goToRowColumn(1,0);
    }
    else if(keypad_entry()==3)
    {
        LCD_clearScreen();
        LCD_displayStringRowColumn(0,0,"Wrong Password!");
        LCD_goToRowColumn(1,0);
    }
default_screen();
request_change = 0;
pass_change = 0;
}

void motor_forward()
{
    PORTD &= ~(1<<4);
    PORTD |= (1<<3);
}

void motor_backward()
{
    PORTD &= ~(1<<3);
    PORTD |= (1<<4);
}

void motor_stop()
{
    PORTD &= ~(1<<3);
    PORTD &= ~(1<<4);
}

void timer0_init()
{
    TCCR0 = (1<<WGM01);
    sei();
}


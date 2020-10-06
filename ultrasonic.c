
#include "ultrasonic.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile int pulse=0;
static volatile int i=0;

int16_t count_a = 0;
char show_a[16];

void Usonic_init(void)
{
    DDRD |= (1<<TRIG);
	DDRD &= ~(1<<ECHO);
	GICR |= 1<<INT0;
	MCUCR |= 1<<ISC00;
	sei();
}

void Usonic_trigger(void)
{
	PORTD |= 1<<TRIG;
	_delay_ms(15);
	PORTD &= ~(1<<TRIG);

}

ISR(INT0_vect)
{
	if(i==1)
	{
		TCCR1B = 0;             // Stop the timer
		pulse = TCNT1;          // Record the duration
		TCNT1 = 0;              // Clear the Timer
		i = 0;
	}
    if(i==0)
	{
		TCCR1B |= 1<<CS10;       // Start the timer
		i = 1;
	}
	count_a = pulse/58;           // get the distance
}

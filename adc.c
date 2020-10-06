
#include "adc.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

double dutyCycle=0;

void adc0_init()
{
    ADMUX = (1<<REFS0);   //Setting the Vcc as the reference and Enable ADC0
    ADCSRA =  (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    adc0_convert();
}

void adc0_convert()
{
    ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect)
{
    dutyCycle = ADC;
    adc0_convert();
}

void PWM_init0()
{
    DDRB |= (1<<3);
    DDRD |= (1<<7);
    TCCR2 = (1<<COM21) | (1<<WGM21) | (1<<WGM20); // p125
    TIMSK = (1<<TOIE2);
    OCR2 = dutyCycle;

    sei();
    TCCR2 |= (1<<CS22);  // p125
}

ISR(TIMER2_OVF_vect)   // Interrupt on Overflow
{
    OCR2 = dutyCycle;
}

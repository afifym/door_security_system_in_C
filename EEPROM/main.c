/*
 */

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/delay.h>

#define ACK_START					0x08
#define ACK_RESTART					0x10
#define ACK_FIRST_TRANS				0x18
#define ACK_TRANSMIT				0x28
#define ACK_FIRST_REC				0x40
#define ACK_RECEIVE					0x58
#define ERROR                          0
#define SUCCESS                        1
#define EEPROM_ADDRESS_WR		    0xA0
#define EEPROM_ADDRESS_RE		    0xA1

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(unsigned char data);
unsigned char i2c_read();
unsigned char i2c_status();

unsigned char eeprom16_write(unsigned char add, unsigned char data);
unsigned char eeprom16_read(unsigned char add, unsigned char *data);

unsigned char pass= 0;
unsigned char counts = '1';
unsigned char data = '9';

int main(void)
{
_delay_ms(30);
LCD_init();
i2c_init();

DDRD = 0xFF;
_delay_ms(1000);

LCD_displayCharacter(counts);  // 1
_delay_ms(1000);
LCD_clearScreen();
LCD_displayCharacter(data);    // 9
_delay_ms(1000);


// Writing---------------------
pass = eeprom16_write (0,counts);  // Stores 1 in location 0
_delay_ms(20);

if (pass == SUCCESS)
	{
		LCD_clearScreen();
		LCD_displayString("SUCCESS");
	}
	else if (pass == ERROR)
	{
		LCD_clearScreen();
		LCD_displayString("ERROR");
	}
	_delay_ms(1000);

	// Reading---------------------
	pass = eeprom16_read(0,&data);
	_delay_ms(20);
	if (pass == SUCCESS)
	{
		LCD_clearScreen();
		LCD_displayString("SUCCESS");
	}
	else if (pass == ERROR)
	{
		LCD_clearScreen();
		LCD_displayString("ERROR");
	}
	_delay_ms(1000);

	LCD_clearScreen();
	LCD_displayCharacter(counts);
	_delay_ms(1000);

	LCD_clearScreen();
	_delay_ms(1000);
	LCD_displayCharacter(data);
    _delay_ms(1000);

	LCD_clearScreen();
	LCD_displayCharacter(counts);
	_delay_ms(1000);

    while(1)
    {
        PORTD = 0xFF;
        _delay_ms(1000);
        PORTD = 0x00;
        _delay_ms(1000);
    }


    return 0;
}


unsigned char eeprom16_write(unsigned char add,unsigned char data)
{
    i2c_start();
    if(i2c_status() != ACK_START)  return ERROR;

    i2c_write(EEPROM_ADDRESS_WR);
    if(i2c_status() != ACK_FIRST_TRANS)  return ERROR;

    i2c_write(add);
    if(i2c_status() != ACK_TRANSMIT) return ERROR;

    i2c_write(data);
    if(i2c_status() != ACK_TRANSMIT) return ERROR;

    i2c_stop();
    return SUCCESS;
}

unsigned char eeprom16_read(unsigned char add,unsigned char *data)
{
    i2c_start();
	if(i2c_status() != ACK_START)  return ERROR;

	i2c_write(EEPROM_ADDRESS_WR);
	if(i2c_status() != ACK_FIRST_TRANS)  return ERROR;

	i2c_write(add);
	if(i2c_status() != ACK_TRANSMIT)  return ERROR;

	i2c_start();
	if(i2c_status() != ACK_RESTART)  return ERROR;

	i2c_write(EEPROM_ADDRESS_RE);
	if(i2c_status() != ACK_FIRST_REC)  return ERROR;

	*data = i2c_read();
	if(i2c_status() != ACK_RECEIVE)  return ERROR;

	i2c_stop();
	return SUCCESS;
}


void i2c_init()
{
    TWCR = (1U<<TWEA);
    TWBR = 0x02;
    TWSR = 0x00;
    TWCR |= (1U<<TWEN);
}

void i2c_start()
{
	TWCR  = (1U<<TWEN);
	TWCR |= (1U<<TWSTA);
	TWCR |= (1U<<TWINT);
	while((TWCR & (1<<TWINT)) == 0);
}

void i2c_stop()
{
	TWCR = (1U<<TWEN);
	TWCR |= (1U<<TWSTO);
	TWCR |= (1U<<TWINT);
}

void i2c_write(unsigned char data)
{
    TWDR = data;
	TWCR = (1U<<TWEN);
	TWCR |= (1U<<TWINT);
	while ( (TWCR & (1U<<TWINT)) == 0);
}

unsigned char i2c_read()
{
	TWCR = (1U<<TWEN);
	TWCR |= (1U<<TWINT);
	while ( (TWCR & (1U<<TWINT)) == 0);
	return TWDR;
}

unsigned char i2c_status()
{
    unsigned char status;
	status = (TWSR & 0xF8);
	return status;
}

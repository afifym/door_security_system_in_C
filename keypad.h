#ifndef keypad_H
#define keypad_H

void default_screen();
void key_capture();
unsigned char keypad_entry();

void password_change();
void granted();
void denied();

void motor_forward();
void motor_backward();
void motor_stop();


void timer0_init();
void check_duration();
#endif

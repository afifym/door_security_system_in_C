

#ifndef USONIC_H
#define USONIC_H

#define ECHO PD2
#define TRIG PD1

void Usonic_init();
void Usonic_trigger();
void Usonic_echo();

#endif

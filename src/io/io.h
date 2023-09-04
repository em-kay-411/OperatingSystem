#ifndef IO_H
#define IO_H

unsigned char insb(unsigned short port);        // INput 1 byte to the port
unsigned short insw(unsigned short port);       // Input 2 bytes to the port

void outb(unsigned short port, unsigned char val);   // Output 1 bytes to the port
void outw(unsigned short port, unsigned short val);  // Output 2 bytes to the port

#endif
#ifndef __COM_H
#define __COM_H

#include <Windows.h>
#include <stdio.h>
#include <string.h>

HANDLE _openPort(void** port);
int portWrite(void* port, const void* buf, size_t nbyte);
int portRead(void* port, void* buf, size_t nbyte, unsigned int timeout);
void portDelay(unsigned int ms);

#endif

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INLCUDED

#include <iostream>
#include <string.h>

class Exception{
    char catEx[50], tipEx[50];
public:
    Exception(char*, char*);
    void Print(void);
};

#endif
#include "EXCEPTIONS.h"

Exception::Exception(char* _catEx, char* _tipEx){
    strcpy(catEx, _catEx );
    strcpy(tipEx, _tipEx );
};

void Exception::Print(void){
    std::cerr<<"Exception ["<<catEx<<"] - "<<tipEx<<std::endl;
};
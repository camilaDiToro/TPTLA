// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdlib.h>
#include "include/itoa.h"

char * itoa(int i) {
    int length = snprintf(NULL, 0, "%d", i);
    char * str = malloc( length + 1 );
    snprintf(str, length + 1, "%d", i);
    return str; 
}
#include <stdlib.h>
#include "include/itoa.h"

char * itoa(int i) {
    int length = snprintf(NULL, 0, "%d", i);
    char * str = malloc( length + 1 );
    snprintf(str, length + 1, "%d", i);
    return str; 
}
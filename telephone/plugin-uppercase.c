/*
 * Uppercase Plugin - turns every second character to uppercase
 */
#include <ctype.h>

void process(char **message, int len)
{
    int i;
    char *msg = *message;

    for (i = 1; i < len; i += 2) {
        msg[i] = toupper(msg[i]);
    }
}
    

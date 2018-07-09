/*
 * Increase Plugin - increments every digit by one
 */
#include <ctype.h>

void process(char **message, int len)
{
    int i;
    char *msg = *message;

    for (i = 0; i < len; i++) {
        if (isdigit(msg[i])) {
            if (msg[i] == '9') {
                msg[i] = '0';
            } else {
                msg[i] = msg[i] + 1;
            }
        }
    }
}


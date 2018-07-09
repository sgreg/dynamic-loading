/*
 * Unleet Plugin - turns 1337 w0rd5 back into leet words
 */
#include <ctype.h>

void process(char **message, int len)
{
    int i;
    char *msg = *message;

    for (i = 0; i < len; i++) {
        switch (msg[i]) {
            case '0':
                msg[i] = 'o';
                break;
            case '1':
                msg[i] = 'i';
                break;
            case '2':
                msg[i] = 'z';
                break;
            case '3':
                msg[i] = 'e';
                break;
            case '4':
                msg[i] = 'a';
                break;
            case '5':
                msg[i] = 's';
                break;
            case '6':
                msg[i] = 'G';
                break;
            case '7':
                msg[i] = 't';
                break;
            case '8':
                msg[i] = 'b';
                break;
            case '9':
                msg[i] = 'g';
                break;
        }
    }
}


/*
 * Leet Plugin - turns leet words into 1337 w0rd5
 */
#include <ctype.h>

void process(char **message, int len)
{
    int i;
    char *msg = *message;

    for (i = 0; i < len; i++) {
        switch (msg[i]) {
            case 'O':
            case 'o':
                msg[i] = '0';
                break;
            case 'I':
            case 'i':
                msg[i] = '1';
                break;
            case 'Z':
            case 'z':
                msg[i] = '2';
                break;
            case 'E':
            case 'e':
                msg[i] = '3';
                break;
            case 'A':
            case 'a':
                msg[i] = '4';
                break;
            case 'S':
            case 's':
                msg[i] = '5';
                break;
            case 'G':
                msg[i] = '6';
                break;
            case 'T':
            case 't':
                msg[i] = '7';
                break;
            case 'B':
            case 'b':
                msg[i] = '8';
                break;
            case 'g':
                msg[i] = '9';
                break;
        }
    }
}


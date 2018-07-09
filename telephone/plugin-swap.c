/*
 * Swap Plugin - swaps every second letter with its predecessor
 */
void process(char **message, int len)
{
    int i;
    char tmp;
    char *msg = *message;

    for (i = 1; i < len; i += 2) {
        tmp = msg[i - 1];
        msg[i - 1] = msg[i];
        msg[i] = tmp;
    }
}


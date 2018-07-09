/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * Logger Plugin - logs all GPIO value changes to ./logger.out
 */
#include <stdio.h>
#include <time.h>
#include "plugin.h"

#define LOG_FILE "./logger.out"
FILE *fp;

int logger_setup(void)
{
    fp = fopen(LOG_FILE, "a");
    return (fp == NULL) ? -1 : 0;
}

void logger_teardown(void)
{
    fclose(fp);
}

void logger_handle(int gpio, int value)
{
    char buf[50];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    strftime(buf, sizeof(buf), "%a, %d %b %Y %T %z", tm);
    fprintf(fp, "[%s] GPIO %d changed to %d\n", buf, gpio, value);
    fflush(fp);
}

struct gpio_plugin logger_plugin = {
    .name = "Logger Plugin",
    .setup = logger_setup,
    .teardown = logger_teardown,
    .handle = logger_handle,
};

gpio_plugin_register(logger_plugin);


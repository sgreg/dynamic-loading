/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * Print Plugin - prints all GPIO value changes to stdout
 */
#include <stdio.h>
#include "plugin.h"

void print_handle(int gpio, int value)
{
    printf("GPIO %d changed to %d\n", gpio, value);
}

struct gpio_plugin print_plugin = {
    .name = "Print Plugin",
    .handle = print_handle,
};

gpio_plugin_register(print_plugin);


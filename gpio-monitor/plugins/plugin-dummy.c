/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * Dummy Plugin - does pretty much nothing
 */
#include <stdio.h>
#include "plugin.h"

int dummy_setup(void)
{
    puts("Setting up dummy plugin");
    return 0;
}

void dummy_teardown(void)
{
    puts("Tearing down dummy plugin");
}

struct gpio_plugin dummy_plugin = {
    .name = "Dummy Plugin",
    .setup = dummy_setup,
    .teardown = dummy_teardown,
    .handle = NULL,
};

gpio_plugin_register(dummy_plugin);


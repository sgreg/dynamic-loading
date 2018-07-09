/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * Plugin defitions
 */
#ifndef PLUGIN_H
#define PLUGIN_H

struct gpio_plugin {
    char *name;
    int (*setup)(void); // setup callback, called once after loading
    void (*teardown)(void); // teardown callback, called once before unloading
    void (*handle)(int, int);
};

#define gpio_plugin_register(gpio) \
    struct gpio_plugin *_plugin_get_data(void) {\
        return &gpio;\
    }
#endif /* PLUGIN_H */


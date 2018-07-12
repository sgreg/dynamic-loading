/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * Plugin defitions
 */
#ifndef PLUGIN_H
#define PLUGIN_H

/*
 * GPIO Plugin data structure
 *
 * Implement this struct with your own plugin name and callback functions.
 */
struct gpio_plugin {
    /*
     * Name of this plugin (must be set)
     */
    char *name;
    /*
     * Plugins setup function, called once after loading.
     * Should perform all initial steps required.
     * Should return 0 if setup succeeded and anything != 0 if it fails.
     * In case the setup fails, the plugin won't be enabled.
     * In case this is NULL / not implemented, the plugin will be still enabled
     */
    int (*setup)(void);
    /*
     * Plugins teardown function, called once when plugin is unloaded
     */
    void (*teardown)(void);
    /*
     * GPIO handle function, called whenever a GPIO state changes.
     * Receives the changed GPIO number as first parameter, and the new
     * value as second parameter:
     *      void my_handle(int gpio_number, int new_value) { ... }
     */
    void (*handle)(int, int);
};

/*
 * Register the plugin with the given struct gpio_plugin data.
 * Provides a function _plugin_get_data() the GPIO monitor itself is trying
 * to extract from the plugin, and uses it to retrieve the plugin's data
 * defined in its struct gpio_plugin implementation.
 *
 * Example:
 *      struct gpio_plugin my_plugin = {
 *          .name = "just an example"
 *      };
 *
 *      gpio_plugin_register(my_plugin)
 *
 */
#define gpio_plugin_register(gpio) \
    struct gpio_plugin *_plugin_get_data(void) {\
        return &gpio;\
    }

#endif /* PLUGIN_H */


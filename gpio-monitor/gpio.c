/*
 * Hackaday - Dynamic Loading
 * GPIO Monitor Plugin System
 *
 * GPIO monitor based in wiringPi - http://wiringpi.com/
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <dlfcn.h>
#include <wiringPi.h>
#include "plugin.h"

#define MAX_GPIO 8
#define MAX_PLUGINS 10
#define GPIO_PUD_MODE PUD_DOWN

/* internal plugin structure */
static struct plugin {
    /* libdl handle */
    void *dl_handle;
    /* the data retrieved from the plugin */
    struct gpio_plugin *data;
    /* flag if the plugin is actually enabled */
    int enabled;
} plugins[MAX_PLUGINS];

/* counter to keep track how many plugins are there */
static int plugin_count;
/* run the program while this is > 0 */
static int keep_going = 1;
/* flag to trigger a plugin reload, set by SIGUSR1 signal handler */
static int reload_plugins_triggered;
/* keeping track of current GPIO state values */
static int gpio_values[MAX_GPIO];

/*
 * File filter function for scandir().
 * Causes scandir() to return only files matching the pattern defined here:
 *   - filename longer than 7 characters
 *   - filename ending with ".plugin"
 */
int read_dir_filter(const struct dirent *dir)
{
    size_t len = strlen(dir->d_name);
    return (len > 7 && !strcmp(dir->d_name + len - 7, ".plugin"));
}

/*
 * Read and load in all plugins from the given dirname.
 * Reads given directory and loads all plugin files via dynamic loading.
 * If loading succeeds, the plugin data is retrieved.
 */
static int read_plugins(const char *dirname)
{
    struct dirent **dir;
    int index;
    int count;
    char path[PATH_MAX];

    void *dl_handle;
    struct gpio_plugin *(*get_data)(void);
    struct gpio_plugin *data;


    count = scandir(dirname, &dir, read_dir_filter, alphasort);
    if (count < 0) {
        fprintf(stderr, "cannot open %s: %s\n", dirname, strerror(errno));
        return -1;
    }

    fprintf(stderr, "checking %d possible plugins\n", count);
    for (index = 0; index < count; index++) {
        fprintf(stderr, "  [%d] checking %s\n    ", index, dir[index]->d_name);

        snprintf(path, PATH_MAX, "%s/%s", dirname, dir[index]->d_name);
        dl_handle = dlopen(path, RTLD_LAZY);

        if ( (dl_handle = dlopen(path, RTLD_NOW)) == NULL) {
            fprintf(stderr, "[-] error: %s\n", dlerror());
            free(dir[index]);
            continue;
        }

        if ( (get_data = dlsym(dl_handle, "_plugin_get_data")) == NULL) {
            fprintf(stderr, "[-] error: %s\n", dlerror());
            dlclose(dl_handle);
            free(dir[index]);
            continue;
        }

        if ( (data = get_data()) == NULL) {
            fputs("[-] error: cannot retrieve plugin data\n", stderr);
            dlclose(dl_handle);
            free(dir[index]);
            continue;
        }
        
        if (data->name == NULL || strlen(data->name) == 0) {
            fputs("[-] error: plugin has no name\n", stderr);
            dlclose(dl_handle);
            free(dir[index]);
            continue;
        }

        fprintf(stderr, "[+] loaded '%s'\n", data->name);

        plugins[plugin_count].dl_handle = dl_handle;
        plugins[plugin_count].data = data;
        plugin_count++;

        free(dir[index]);
    }

    fprintf(stderr, "\nloaded %d %s\n", plugin_count,
            ((plugin_count == 1) ? "plugin" : "plugins"));
    for (index = 0; index < plugin_count; index++) {
        fprintf(stderr, "  [%d] %s\n", index, plugins[index].data->name);
    }

    free(dir);
    return 0;
}

/*
 * Setup all loaded plugins.
 * Calls the setup() callback for every loaded plugin, if available, and
 * depending on the setup() callback's return value, the plugin is set as
 * enabled (return value == 0, or no setup() callback implemented) or as
 * disabled (return value != 0).
 */
void setup_plugins(void)
{
    int index;

    fprintf(stderr, "\nsetting up plugins\n");
    for (index = 0; index < plugin_count; index++) {
        if (plugins[index].data->setup) {
            plugins[index].enabled = (plugins[index].data->setup() == 0);
        } else {
            plugins[index].enabled = 1;
        }
        fprintf(stderr, "  [%d] %s - %s\n", index,
                plugins[index].data->name,
                ((plugins[index].enabled) ? "enabled" : "disabled"));
    }
    fputs("\n", stderr);
}

/*
 * Tear down all loaded and enabled plugins.
 * Calls the teardown() callback for every loaded plugin, if available.
 */
void teardown_plugins(void)
{
    int index;

    fprintf(stderr, "\ntearing down plugins\n");
    for (index = 0; index < plugin_count; index++) {
        if (plugins[index].enabled && plugins[index].data->teardown) {
            fprintf(stderr, "  [%d] %s\n", index, plugins[index].data->name);
            plugins[index].data->teardown();
        }
    }
    fputs("\n", stderr);
}

/*
 * Unloads all previously loaded plugins.
 * Calls dlclose() and removes plugins
 */
static void unload_plugins(void)
{
    while (plugin_count) {
        dlclose(plugins[--plugin_count].dl_handle);
        memset(&plugins[plugin_count], 0x00, sizeof(struct plugin));
    }
}

/*
 * Unloads and reloads all plugin from the given dirname
 */
static void reload_plugins(const char *dirname)
{
    teardown_plugins();
    unload_plugins();
    fputs("\nReloading plugins\n\n", stderr);
    read_plugins(dirname);
    setup_plugins();
}

/*
 * Initialize GPIO via wiringPi
 */
void init_gpios(void)
{
    int gpio;

    for (gpio = 0; gpio < MAX_GPIO; gpio++) {
        pinMode(gpio, INPUT);
        pullUpDnControl(gpio, GPIO_PUD_MODE);
        gpio_values[gpio] = digitalRead(gpio);
    }
}

/*
 * Handle GPIOs.
 * Called periodically to check if any of the GPIO input values has changed.
 * If one has changed, the plugins' handle() callback is called, if available.
 */
void handle_gpios(void)
{
    int gpio;
    int index;
    int value;

    for (gpio = 0; gpio < MAX_GPIO; gpio++) {
        value = digitalRead(gpio);
        if (value != gpio_values[gpio]) {
            for (index = 0; index < plugin_count; index++) {
                if (plugins[index].enabled && plugins[index].data->handle) {
                    plugins[index].data->handle(gpio, value);
                }
            }
            gpio_values[gpio] = value;
        }
    }
}

/*
 * SIGINT (CTRL+C) signal handler.
 * Stops the main loop.
 */
void sigint_handler(int signum __attribute__((unused)))
{
    keep_going = 0;
}

/*
 * SIGUSR1 signal handler.
 * Triggers reloading of all plugins
 */
void sigusr1_handler(int signum __attribute__((unused)))
{
    reload_plugins_triggered = 1;
}

/*
 * Main.
 * Sets up everything and runs the main loop.
 */
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <plugin directory>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    wiringPiSetup();
    init_gpios();

    if (read_plugins(argv[1]) < 0) {
        fprintf(stderr, "Reading plugins failed\n");
        return 2;
    }

    setup_plugins();

    while (keep_going) {
        if (reload_plugins_triggered) {
            reload_plugins_triggered = 0;
            reload_plugins(argv[1]);
        }
        handle_gpios();
        usleep(50000);
    }

    teardown_plugins();
    unload_plugins();

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char **argv) {
    void *handle;
    void (*process)(char **, int);
    int index;

    if (argc < 3) {
        printf("usage: %s <message> <plugin> [,<plugin>,...]\n", argv[0]);
        return 1;
    }

    // argv[1] is the message, start from index 2 for the plugin list
    for (index = 2; index < argc; index++) {
        // open next plugin
        handle = dlopen(argv[index], RTLD_NOW);
        if ( (handle = dlopen(argv[index], RTLD_NOW)) == NULL) {
            return 1;
        }

        // extract the process() function
        process = dlsym(handle, "process");
        if (dlerror() != NULL) {
            dlclose(handle);
            return 2;
        }

        // call the process function, modifying argv[1] directly
        process(&argv[1], strlen(argv[1]));

        // close the plugin
        dlclose(handle);
    }

    // print the resulting msesage
    printf("%s\n", argv[1]);

    return 0;
}

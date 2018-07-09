# Hackaday Dynamic Loading - GPIO Monitor

A basic GPIO monitor for Raspberry Pi as example for a plugin system based on [dynamic loading](https://hackaday.com/2018/07/12/its-all-in-the-libs-building-a-plugin-system-using-dynamic-loading/).

## Content
1. The main program, periodically checks the states of a Raspberry Pi's GPIO pins
    * `gpio.c`
2. A series of plugins to process the GPIO state changes
    * `plugins/plugin-dummy.c` - just a dummy plugin as code base, does pretty much nothing
    * `plugins/plugin-logger.c` - writes all GPIO state changes to a log file `logger.out`
    * `plugins/plugin-print.c` - prints all GPIO state changes to stdout
3. A shell script to compile all plugin sources inside `./plugins/`
    * `build_plugins.sh`
4. A Makefile
    * `Makefile`

## Building


The example uses [wiringPi](http://wiringpi.com/) to handle the Raspberry Pi's GPIO inputs. Its involvement is rather minimal, so integrating a different way to handle the GPIOs shouldn't be too difficult. Still, you will need to have `wiringPi` installed to make the compilation and execution work.

To build the main program and all the plugin, simply type `make` which should give you some output like this:
```
$ make
Building plugin dummy
Building plugin print
Building plugin logger
gcc -Wall -Wextra -o gpio gpio.c -ldl -lwiringPi
$
```
Check `make help` for some more information.

## GPIO Pins

Since [wiringPi](http://wiringpi.com/) is used to handle the GPIOs, the example uses [the wiringPi pin numbering scheme](http://wiringpi.com/pins/) and by default sets the first 8 GPIOs (check `MAX_GPIO` in `gpio.c`) as input with pull down resistor enabled, so they will read `0` by default, unless they are kept high externally of course.

## Usage
Call `./gpio` with a path to a plugin directory. The plugins shipped in this example are all inside the `./plugins` directory.

```
$ ./gpio ./plugins/
checking 3 possible plugins
  [0] checking dummy.plugin
    [+] loaded 'Dummy Plugin'
  [1] checking logger.plugin
    [+] loaded 'Logger Plugin'
  [2] checking print.plugin
    [+] loaded 'Print Plugin'

loaded 3 plugins
  [0] Dummy Plugin
  [1] Logger Plugin
  [2] Print Plugin

setting up plugins
Setting up dummy plugin
  [0] Dummy Plugin - enabled
  [1] Logger Plugin - enabled
  [2] Print Plugin - enabled

GPIO 1 changed to 1                 <-- pin was pulled high here, print plugin displayed it
GPIO 1 changed to 0                 <-- pin was released back to low here, also print plugin
^C                                  <-- CTRL+C was pressed
tearing down plugins
  [0] Dummy Plugin
Tearing down dummy plugin
  [1] Logger Plugin

$
```

Note, the GPIOs are polled in this example, and a main loop checks them periodically with a 50ms sleep.

### Reload plugin directory content at runtime

A signal handler for `SIGUSR1` is implemented to close and unload all plugins, and re-read them. This can be used to re-configure the plugin system at runtime by adding or removing plugin files in the initially given plugin directory.

```
$ ps aux |grep gpio
pi        1234  0.0  0.1   3236  1532 pts/0    T    16:28   0:00 ./gpio plugins/
#         ^^^^ the PID we need
```
Send `SIGUSR1` to the process ID of our GPIO monitor
```
$ kill -USR1 1234
```
If you deleted or added some `xxx.plugin` files to the `plugins` directory, they should be now added / removed in the GPIO monitor itself.

For a bit more flexibility, you can add a `plugins-enabled` directory with symlinks to the actual plugins
```
$ mkdir plugins-enabled
$ cd plugins-enabled
plugins-enabled $ ln -s ../plugins/dummy.plugin .
plugins-enabled $ cd ..
$ ./gpio ./plugins-enabled/
checking 1 possible plugins
  [0] checking dummy.plugin
    [+] loaded 'Dummy Plugin'

loaded 1 plugin
  [0] Dummy Plugin

setting up plugins
Setting up dummy plugin
  [0] Dummy Plugin - enabled
```

Adding another plugin as symlink and sending `SIGUSR1` to the `./gpio` process from another terminal:
```
$ cd plugins-enabled/
plugins-enabled $ ln -s ../plugins/print.plugin .
... 

$ ps aux |grep ./gpio
pi        1286  0.0  0.1   3096  1596 pts/1    S+   16:35   0:00 ./gpio plugins-enabled/
$ kill -USR1 1286
$
```

And in our terminal with `./gpio` running we will see:
```

tearing down plugins
  [0] Dummy Plugin
Tearing down dummy plugin


Reloading plugins

checking 2 possible plugins
  [0] checking dummy.plugin
    [+] loaded 'Dummy Plugin'
  [1] checking print.plugin
    [+] loaded 'Print Plugin'

loaded 2 plugins
  [0] Dummy Plugin
  [1] Print Plugin

setting up plugins
Setting up dummy plugin
  [0] Dummy Plugin - enabled
  [1] Print Plugin - enabled

```

And now we have both there.

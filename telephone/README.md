# Hackaday Dynamic Loading - Telephone

A take on the classic children's game [*Telephone*](https://en.wikipedia.org/wiki/Chinese_whispers) as example for a plugin system based on [dynamic loading](https://hackaday.com/2018/07/12/its-all-in-the-libs-building-a-plugin-system-using-dynamic-loading/).

## Content
1. The main program, processes a given message based on a series of given plugins
    * `telephone.c`
2. A series of plugins
    * `plugin-increase.c` - increments every digit by one
    * `plugin-leet.c` - turns leet words into 1337 w0rd5
    * `plugin-swap.c` - swap every second letter its predecessor
    * `plugin-unleet.c` - turns 1337 w0rd5 back into leet words
    * `plugin-uppercase.c` - turns every second character to uppercase
3. A simple Makefile (don't hold it against me)
    * `Makefile`

## Building

To build the main program and all the plugin, simply type `make` which should give you some output like this:
```
$ make
gcc -shared -fPIC -o increase.plugin plugin-increase.c -Wall -Wextra
gcc -shared -fPIC -o leet.plugin plugin-leet.c -Wall -Wextra
gcc -shared -fPIC -o swap.plugin plugin-swap.c -Wall -Wextra
gcc -shared -fPIC -o unleet.plugin plugin-unleet.c -Wall -Wextra
gcc -shared -fPIC -o uppercase.plugin plugin-uppercase.c -Wall -Wextra
gcc -o telephone telephone.c -ldl -Wall -Wextra
$
```

## Usage
Call `./telephone` with a message (enclosed in double quotes `""`) and an artibrary list of plugins.

```
$ ./telephone "just some random message" ./uppercase.plugin 
jUsT SoMe rAnDoM MeSsAgE
$ ./telephone "just some random message" ./leet.plugin 
ju57 50m3 r4nd0m m355493
$ ./telephone "just some random message" ./uppercase.plugin ./leet.plugin 
jU57 50M3 r4nD0M M355493
$
```


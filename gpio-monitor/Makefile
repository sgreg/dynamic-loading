#
# Hackaday - Dynamic Loading
# GPIO Monitor Plugin System
#
# Makefile - run `make help` for more information
#            ..or just keep reading
#
all: plugins gpio

gpio: gpio.c
	gcc -Wall -Wextra -o $@ $< -ldl -lwiringPi

.PHONY: plugins
plugins:
	@./build_plugins.sh

.PHONY: clean
clean:
	rm -f gpio plugins/*.plugin 

.PHONY: help
help:
	@echo ""
	@echo "          Hackaday - Dynamic Loading"
	@echo "          GPIO Monitor Plugin System"
	@echo ""
	@echo "  Requirements:  Raspberry Pi with wiringPi"
	@echo ""
	@echo "  GPIO monitor source:               gpio.c"
	@echo "  GPIO plugin sources:   plugins/plugin-*.c"
	@echo "  Plugin build script:     build_plugins.sh"
	@echo ""
	@echo "  Compile GPIO monitor:           make gpio"
	@echo "  Compile all plugins:         make plugins"
	@echo "  Compile everything:                  make"
	@echo ""
	@echo "  Run GPIO monitor:         ./gpio plugins/"
	@echo ""

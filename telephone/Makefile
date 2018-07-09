
all: plugins telephone

plugins:
	gcc -shared -fPIC -o increase.plugin plugin-increase.c -Wall -Wextra
	gcc -shared -fPIC -o leet.plugin plugin-leet.c -Wall -Wextra
	gcc -shared -fPIC -o swap.plugin plugin-swap.c -Wall -Wextra
	gcc -shared -fPIC -o unleet.plugin plugin-unleet.c -Wall -Wextra
	gcc -shared -fPIC -o uppercase.plugin plugin-uppercase.c -Wall -Wextra

telephone: telephone.c
	gcc -o $@ $^ -ldl -Wall -Wextra

clean:
	rm -f *.plugin
	rm -f telephone

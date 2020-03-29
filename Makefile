all: program

program : cJSON.o main.o
	gcc -o program cJSON.o main.o

main.o : main.c cJSON.h
	gcc -c -fPIC -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion -fstack-protector main.c

cJSON.o : cJSON.c cJSON.h
	gcc -c -fPIC -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion -fstack-protector cJSON.c



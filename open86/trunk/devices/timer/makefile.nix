

#-lSDL -lSDL_ttf -o myemu

_OBJS=.objs/main.o
_CFLAGS=-g -c
default:
	gcc $(_CFLAGS) main.c -o .objs/main.o

	g++ -shared -W1 $(_OBJS) -lSDL -o timer.so

clean:
	rm $(_OBJS)

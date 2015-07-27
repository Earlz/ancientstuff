#My SDL video and gui stuff

#-lSDL -lSDL_ttf -o myemu

_OBJS=.objs/main.o .objs/font.o
_CFLAGS=-g -c
default:
	gcc $(_CFLAGS) main.c -o .objs/main.o
	gcc $(_CFLAGS) font.c -o .objs/font.o

	g++ -shared -W1 $(_OBJS) -lSDL -o VideoCard.so

clean:
	rm $(_OBJS)

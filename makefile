cc = clang
CFLAGS = -O3 -Wall -c -std=c99
LDFLAGS = -lm -lncurses
EXECUTABLE=srn
SOURCES=main.c jsmn/jsmn.c game/EscapeScreen.c game/GraphicsCheck.c player/playerSpriteCreation.c graphics/graphics.c
COLLECTIONS_C = Collections-C/src/array.c Collections-C/src/common.c Collections-C/src/deque.c Collections-C/src/hashset.c Collections-C/src/hashtable.c Collections-C/src/list.c Collections-C/src/queue.c Collections-C/src/slist.c Collections-C/src/stack.c Collections-C/src/treeset.c Collections-C/src/treetable.c
ASPRINTF=asprintf.c/asprintf.c
OBJECTS=$(SOURCES:.c=.o) $(COLLECTIONS_C:.c=.o) $(ASPRINTF:.c=.o)



all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(cc)  $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(cc) $(CFLAGS) $< -o $@


clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

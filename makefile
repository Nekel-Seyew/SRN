cc = clang
CFLAGS = -O3 -Wall -c
LDFLAGS = -lm -lncurses
EXECUTABLE=srn
SOURCES=main.c jsmn/jsmn.c game/EscapeScreen.c game/GraphicsCheck.c player/playerSpriteCreation.c graphics/graphics.c asprintf/asprintf.c
OBJECTS=$(SOURCES:.c=.o)



all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(cc)  $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(cc) $(CFLAGS) $< -o $@


clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

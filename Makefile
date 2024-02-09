CC = gcc
CFLAGS = -c -ggdb
LDFLAGS =
SRCDIR = sources
INCDIR = headers
OBJDIR = objects
EXE = my_readelf

SOURCES=$(wildcard $(SRCDIR)/*.c)
HEADERS=$(wildcard $(INCDIR)/*.h)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(OBJDIR)


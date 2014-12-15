CC=gcc
CFLAGS=-D_GNU_SOURCE -O2 -Isrc -DNDEBUG $(OPTFLAGS)
LDFLAGS=-luuid

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=vhd-util

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f *.o $(TARGET)

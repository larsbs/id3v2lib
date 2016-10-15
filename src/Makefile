.PHONY: all clean

CPPFLAGS = -I../include -I../include/id3v2lib
CFLAGS = -g -Wall -std=c99

OBJS = frame.o \
       header.o \
       id3v2lib.o \
       types.o \
       utils.o

LIBID3V2=libid3v2.a

all .DEFAULT: $(LIBID3V2)

$(LIBID3V2): $(OBJS)
	ar -rcs $(LIBID3V2) $(OBJS)

clean:
	rm -rf $(LIBID3V2) $(OBJS) *~

#
# build libhelper.a
#

CC = gcc
CFLAGS = -Wall -std=gnu11 `pkg-config --cflags libcap-ng`
#LDLIBS = -lpthread -lrt `pkg-config --libs libcap-ng`
LDLIBS =

LIB = libhelper.a
OBJ = libhelper.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(LIB): $(OBJ)
	ar rcs $@ $<

install: $(LIB)
	install $(LIB) $(HOME)/lib/$(LIB)

uninstall:
	rm -rf $(HOME)/lib/$(LIB)

clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(LIB)

distclean: clean

.PHONY: clean install uninstall

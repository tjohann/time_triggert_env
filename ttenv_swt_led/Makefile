#
# build ttenv_swt_led.c
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 `pkg-config --cflags libcap-ng`
LDLIBS = -I../libttenv -I../libhelper -L../libhelper -L../libttenv -lhelper -lttenv -lpthread -lrt `pkg-config --libs libcap-ng`

PROG = ttenv_swt_led
OBJ = ttenv_swt_led.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(PROG): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDLIBS)

install: $(PROG)
	install $(PROG) $(HOME)/bin/$(PROG)

uninstall:
	rm -rf $(HOME)/bin/$(PROG)
clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(PROG)

distclean: clean

run_valgrind: $(PROG)
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./ttenv_swt_led

.PHONY: clean install uninstall

#
# Makefile                 
#

MODULES = Documentation pics

CFLAGS = -Wall -O2 -g
LDLIBS = -lpthread -lrt 
BASEFILES = conf_sched.c helper.c 

all: example1 example_gpio
	@echo "+-------------------------------------------+"
	@echo "|           finished build                  |"
	@echo "+-------------------------------------------+"	

example1: $(BASEFILES) example1.c conf_sched.h helper.h
	${CC} $(CFLAGS) -o example1 $(BASEFILES) example1.c $(LDLIBS)

example_gpio: $(BASEFILES) example_gpio.c conf_sched.h helper.h
	${CC} $(CFLAGS) -o example_gpio $(BASEFILES) example_gpio.c $(LDLIBS) 

clean::
	rm -f *~ .*~ *.o *.i *.s *.so *.so.* *.a
	rm -f example1
	rm -f example_gpio
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done


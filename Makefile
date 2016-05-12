#
# Makefile                 
#

MODULES = Documentation pics

CFLAGS = -Wall -O2 -g `pkg-config --cflags libcap-ng`
LDLIBS = -lpthread -lrt `pkg-config --libs libcap-ng`
BASEFILES = conf_sched.c helper.c 

all: example1 example_gpio
	@echo "+-------------------------------------------+"
	@echo "|           finished build                  |"
	@echo "+-------------------------------------------+"	

example1: $(BASEFILES) example1.c tt_env.h
	${CC} $(CFLAGS) -o example1 $(BASEFILES) example1.c $(LDLIBS)

example_gpio: $(BASEFILES) example_gpio.c tt_env.h
	${CC} $(CFLAGS) -o example_gpio $(BASEFILES) example_gpio.c $(LDLIBS) 

clean::
	rm -f *~ .*~ *.o *.i *.s *.so *.so.* *.a
	rm -f example1
	rm -f example_gpio
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done


#
# Makefile
#

MODULES = Documentation pics

CFLAGS = -Wall -O2 -g `pkg-config --cflags libcap-ng`
LDLIBS = -lpthread -lrt `pkg-config --libs libcap-ng`
BASEFILES = conf_sched.c helper.c

all: example1 example_gpio example_gpio2 example_swt_led
	@echo "+-------------------------------------------+"
	@echo "|           finished build                  |"
	@echo "+-------------------------------------------+"

example1: $(BASEFILES) example1.c tt_env.h
	${CC} $(CFLAGS) -o example1 $(BASEFILES) example1.c $(LDLIBS)

example_gpio: $(BASEFILES) example_gpio.c tt_env.h
	${CC} $(CFLAGS) -o example_gpio $(BASEFILES) example_gpio.c $(LDLIBS)

example_gpio2: $(BASEFILES) example_gpio2.c tt_env.h
	${CC} $(CFLAGS) -o example_gpio2 $(BASEFILES) example_gpio2.c $(LDLIBS)

example_swt_led: $(BASEFILES) example_swt_led.c tt_env.h
	${CC} $(CFLAGS) -o example_swt_led $(BASEFILES) example_swt_led.c $(LDLIBS)

clean::
	rm -f *~ .*~ *.o *.i *.s *.so *.so.* *.a
	rm -f example1
	rm -f example_gpio
	rm -f example_gpio2
	rm -f example_swt_led
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done


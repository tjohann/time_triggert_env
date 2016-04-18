#
# BSD 2-Clause
# (c) 2015, thorsten.johannvorderbrueggen@t-online.de                        
#

MODULES = Documentation 

CFLAGS = -Wall -O2 -g
LDLIBS = -lpthread -lrt 
EXAMPLE01 = conf_sched.c helper.c example1.c

all: example1
	@echo "+-------------------------------------------+"
	@echo "|           finished build                  |"
	@echo "+-------------------------------------------+"	

example1: $(EXAMPLE01) conf_sched.h helper.h
	${CC} $(CFLAGS) -o example1 $(EXAMPLE01)  $(LDLIBS) 	

clean::
	rm -f *~ .*~ *.o *.i *.s *.so *.so.* *.a
	rm -f example1
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done


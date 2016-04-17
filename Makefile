#
# BSD 2-Clause
# (c) 2015, thorsten.johannvorderbrueggen@t-online.de                        
#

CFLAGS = -Wall -O2 -g
LDLIBS = -lpthread -lrt 
EXAMPLE01 = conf_sched.c example1.c

example1: $(EXAMPLE01) conf_sched.h
	${CC} $(CFLAGS) -o example1 $(EXAMPLE01)  $(LDLIBS) 	

clean::
	rm -f *~ .*~ *.o *.i *.s *.so *.so.* *.a
	rm -f example1


time_triggert_env
=================

This repository is my playground around the linux realtime kernel scheduler. Micheal J. Pont (http://www.safetty.net/about) describes in his book ERES (http://www.safetty.net/publications/the-engineering-of-reliable-embedded-systems) a time triggert os/system/environment to build reliable embedded systems. As a linux and open-source developer i try to reflect the ideas to linux. The linux kernel already provides all needed parts like SCHED_FIFO/SCHED_RR and so the environment (time_triggert_env) is more a configuration of them and a little bit around it.

Dont expect to much from this repository because its only a starting point and more a playground than something serious. With this repository i also want to learn more about realtime and linux in general.


Basic idea configuration
------------------------

The basic idea is to split the task into different functions:

	int build_sched_table(fiber_element_t fiber_array[], int count);
	int set_sched_props(fiber_element_t fiber_array[], int count);
	int start_sched_table(fiber_element_t fiber_array[], int count);


The scheduling entity is a fiber (cooperative thread -> SCHED_FIFO)
	
	void *fiber(void *arg);
	

Example
-------

The basic example example1 shows the usage:

	
	size_t num_fiber_elements = 4;
	fiber_element_t fiber_array[] =
	{
		{
			.func = fiber_4,
			.sched_param = { .sched_priority = 90,
	    },
		.dt = MS_TO_NS(1),
	},
	...
	};


With fiber_array i define an array with all needed properties.

A fiber is defined like:

	static void
	fiber_4 (void)
	{
		function_1();
		function_2();
		function_3();
		function_4();
	}
	
With the properities defined in fiber_array[x] we have a SCHED_FIFO thread (fiber)	with priority 90 called every 1ms. Within this fiber we call function_1 to function_4.


Example for function_1:

	static void
	function_1()
	{
		printf("in function_1\n");
	}


Possible upgrades
-----------------

A possible extention would be to assign a fiber to a specific cpu. Another extention would be to use real threads instead of fiber. With that extentions its possible to use SCHED_RR or better SCHED_DEADLINE.

time_triggert_env
=================

This repository is my playground around the linux realtime kernel scheduler. Micheal J. Pont (http://www.safetty.net/about) describes in his book ERES (http://www.safetty.net/publications/the-engineering-of-reliable-embedded-systems) a time triggert os/system/environment to build reliable embedded systems. As a linux and open-source developer i try to reflect the ideas to linux. The linux kernel already provides all needed parts like SCHED_FIFO/SCHED_RR and so the environment (time_triggert_env) is more a configuration of them and a little bit around it.

Dont expect to much from this repository because its only a starting point and more a playground than something serious. With this repository I also want to learn more about linux realtime capabilities.


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
	
With the properities defined in fiber_array[x] we have a SCHED_FIFO thread (fiber) with priority 90 called every 1ms. Within this fiber we call function_1 to function_4.


Example for function_1:

	static void
	function_1()
	{
		printf("in function_1\n");
	}


Possible upgrades
-----------------

A possible extention would be to assign a fiber to a specific cpu (work in progess ... see TODO). Another extention would be to use SCHED_RR or better SCHED_DEADLINE (work in progress ... see TODO).


Test environment
----------------

Actually I use a void-linux (http://www.voidlinux.eu/) with PREEMPT kernel on a Bananapi-M1 (http://linux-sunxi.org/LeMaker_Banana_Pi). It has a dual-core armv7hf processor@1GHz. I support that device with a RT-PREEMPT patched kernel via my a20_sdk (https://github.com/tjohann/a20_sdk). When it comes to GPIO example I use my driver examples/scripts (https://github.com/tjohann/mydriver). On of the examples will be to toogle a PIN with a fixed sequence. For that I will use the my gpio driver. Here's my really simple workbench:

![Alt text](pics/gpio_led_01.jpg?raw=true "GPIO-LED")


Linux Realtime and Time-Triggert-System
=======================================

This repository is my playground around the linux realtime kernel scheduler. Micheal J. Pont (http://www.safetty.net/about) describes in his book ERES (http://www.safetty.net/publications/the-engineering-of-reliable-embedded-systems) a time triggert os/system/environment to build reliable embedded systems. As a linux and open-source developer i try to reflect the ideas to linux. The linux kernel already provides all needed parts like SCHED_FIFO/SCHED_RR and so the environment (time_triggert_env) is more a configuration of them and a little bit around it.

Dont expect to much from this repository because its only a starting point and more a playground than something serious (https://tjohann.wordpress.com/category/embedded-realtime/). With this repository I also want to learn more about linux realtime capabilities.


Prerequisites
-------------

I use libcap-ng to get rid of root rights. 


Basic idea configuration
------------------------

The basic idea is to split the task into different functions:

	/* build table of functions for a fiber */
	int build_sched_table(fiber_element_t fiber_array[], int count);
	/* set scheduling properties of a fiber */
	int set_sched_props(fiber_element_t fiber_array[], int count);
	/* start a fiber -> wait for pthread_exit */
	int start_sched_table(fiber_element_t fiber_array[], int count);


The scheduling entity is a fiber (cooperative thread or preemptive thread)
	
	void *fiber(void *arg);
	

Example
-------

The basic example example1 shows the usage:

	
	size_t num_fiber_elements = 2;
	fiber_element_t fiber_array[] =
	{
		{
			.func = fiber_1,
			.sched_param = { .sched_priority = 89,
			},
			.cpu = 1,
			.policy = SCHED_FIFO,
			.dt = MS_TO_NS(10),
		},
		{
			.func = fiber_2,
			.sched_param = { .sched_priority = 90,
			},
			.cpu = 2,
			.policy = SCHED_RR,
			.dt = MS_TO_NS(100),
		}
	};


With fiber_array i define an array with all needed properties.

A fiber is defined like:

	static void
	fiber_2 (void)
	{
		function_2();
		function_2();
		function_3();
		function_4();
	}
	
With the properities defined in fiber_array[x] we have a SCHED_FIFO/SCHED_RR thread (fiber_2) with priority 90 called every 100ms. Within this fiber we call function_2 to function_4.


Example for function_2:

	static void
	function_2()
	{
		printf("in function_2\n");
	}


Examples
--------

	example1.c -> simple example to show usage
	exampe_gpio.c -> let a PIN toggle (with LED)


Possible extenstion
-------------------

At the moment the maximal cyclic time is 999ms, but it should be possible have values >=1s. 


Test environment
----------------

Actually I use a void-linux (http://www.voidlinux.eu/) with PREEMPT kernel on a Bananapi-M1 (http://linux-sunxi.org/LeMaker_Banana_Pi). It has a dual-core armv7hf processor@1GHz. I support that device with a RT-PREEMPT patched kernel via my a20_sdk (https://github.com/tjohann/a20_sdk). 

When it comes to GPIO example I use my driver examples/scripts (https://github.com/tjohann/mydriver). On of the examples will be to toogle a PIN with a fixed sequence. For that I will use the my gpio driver. Here's my really simple workbench:

![Alt text](pics/gpio_led_01.jpg?raw=true "GPIO-LED")


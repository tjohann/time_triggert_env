/*
  Copyright (c) 2015-2016, thorsten.johannvorderbrueggen@t-online.de
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "conf_sched.h"
#include "helper.h"

static void
function_1()
{
	printf("in function_1\n");
}

static void
function_2()
{
	printf("in function_2\n");
}

static void
function_3()
{
	printf("in function_3\n");
}

static void
function_4()
{
	printf("in function_4\n");
}

static void
fiber_1 (void)
{
	function_1();
}

static void
fiber_2 (void)
{
	function_2();
}

static void
fiber_3 (void)
{
	function_4();
}

static void
fiber_4 (void)
{
	function_1();
	function_2();
	function_3();
}

size_t num_fiber_elements = 4;
fiber_element_t fiber_array[] =
{
	{
		.func = fiber_1,
		.sched_param = { .sched_priority = 90,
		},
		.dt = MS_TO_NS(1),
	},
	{
		.func = fiber_2,
		.sched_param = { .sched_priority = 89,
		},
		.dt = MS_TO_NS(10),
	},
	{
		.func = fiber_3,
		.sched_param = { .sched_priority = 88,
		},
		.dt = MS_TO_NS(50),
	},
	{
		.func = fiber_4,
		.sched_param = { .sched_priority = 87,
		},
		.dt = MS_TO_NS(100),
	},
};

int
main(int argc, char *argv[])
{
	show_clock_resolution();
	
	if (build_sched_table(fiber_array, num_fiber_elements) != 0) {
		printf("Could not build_sched_table\n");
		exit (EXIT_FAILURE);
	}

	if ( set_sched_props(fiber_array, num_fiber_elements) != 0) {
		printf("Could not set sched prop\n");
		exit (EXIT_FAILURE);
	}
	
	fflush(stdout);

	if (start_sched_table(fiber_array, num_fiber_elements) != 0) {
		printf("Could not start sched table");
		exit (EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

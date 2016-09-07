/*
  GPL
  (c) 2015-2016, thorsten.johannvorderbrueggen@t-online.de

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include "libttenv.h"

#define DEV_NAME "/dev/gpio_driver"

static int fd;
static ssize_t n;
static int value;
static size_t len;

static void
init_example()
{
	fd = open(DEV_NAME, O_WRONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	len = sizeof(value);
}

static void
function_1()
{
	n = write(fd, &value, len);
	if (n == -1)
		perror("write");

	if (value == 1)
		value = 0;
	else
		value = 1;
}

static void
fiber_1 (void)
{
	function_1();
}

size_t num_fiber_elements = 1;
fiber_element_t fiber_array[] =
{
	{
		.func = fiber_1,
		.sched_param = { .sched_priority = 90,
		},
		.cpu = 0,
		.policy = SCHED_FIFO,
		.dt = MS_TO_NS(500),
	}
};

int
main(int argc, char *argv[])
{
	show_clock_resolution();

	if (check_for_rtpreempt() == false) {
		printf("NO (RT-)PREEMPT support\n");
                exit(EXIT_FAILURE);
        } else {
                printf("(RT-)PREEMPT support\n");
	}

	if (drop_capability(CAP_SYS_NICE) == -1)
		exit(EXIT_FAILURE);

	if (build_sched_table(fiber_array, num_fiber_elements) != 0) {
		printf("Could not build_sched_table\n");
		exit (EXIT_FAILURE);
	}

	if (set_sched_props(fiber_array, num_fiber_elements) != 0) {
		printf("Could not set sched prop\n");
		exit(EXIT_FAILURE);
	}

	init_example();

	fflush(stdout);

	if (start_sched_table(fiber_array, num_fiber_elements) != 0) {
		printf("Could not start sched table\n");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

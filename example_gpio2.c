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

#include "tt_env.h"

#define DEV_NAME "/dev/gpio_driver"

/* ioctl's */
#define IOCTL_SET_WRITE_PIN 0x0001
#define IOCTL_SET_READ_PIN  0x0002

/* pin 273 */
static int pin_500 = 273;
static int fd_500;
static int value_500;

/* pin 275 */
static int fd_750;
static int value_750;

/* common for both functions */
static ssize_t n;
static size_t len;

static void
init_example()
{
	fd_500 = open(DEV_NAME, O_WRONLY);
	if (fd_500 == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if(ioctl(fd_500, IOCTL_SET_WRITE_PIN, &pin_500) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	fd_750 = open(DEV_NAME, O_WRONLY);
	if (fd_750 == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	len = sizeof(value_500);
}

static void
function_500()
{
	n = write(fd_500, &value_500, len);
	if (n == -1)
		perror("write");

	if (value_500 == 1)
		value_500 = 0;
	else
		value_500 = 1;
}

static void
function_750()
{
	n = write(fd_750, &value_750, len);
	if (n == -1)
		perror("write");

	if (value_750 == 1)
		value_750 = 0;
	else
		value_750 = 1;
}

static void
fiber_500 (void)
{
	function_500();
}

static void
fiber_750 (void)
{
	function_750();
}

size_t num_fiber_elements = 2;
fiber_element_t fiber_array[] =
{
	{
		.func = fiber_500,
		.sched_param = { .sched_priority = 90,
		},
		.cpu = 0,
		.policy = SCHED_FIFO,
		.dt = MS_TO_NS(500),
	},
	{
		.func = fiber_750,
		.sched_param = { .sched_priority = 90,
		},
		.cpu = 1,
		.policy = SCHED_FIFO,
		.dt = MS_TO_NS(750),
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

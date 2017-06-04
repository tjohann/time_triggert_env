/*
  GPL
  (c) 2015-2017, thorsten.johannvorderbrueggen@t-online.de

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

/* ioctl's */
#define IOCTL_SET_WRITE_PIN 0x0001
#define IOCTL_SET_READ_PIN  0x0002


/* pin 273 -> blinking led */
static int pin_750 = 273;
static int fd_750;
static int value_750;

/* pin 275 (default output of gpio_driver) -> output led */
//static int pin_led = 275;
static int fd_led;

/* pin 274 (default input of gpio_driver) -> swt */
//static int pin_swt = 274;
static int fd_swt;
static int value_swt;


/* common for all functions */
static size_t len;

static void
init_example()
{
	fd_750 = open(DEV_NAME, O_WRONLY);
	if (fd_750 == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if(ioctl(fd_750, IOCTL_SET_WRITE_PIN, &pin_750) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	fd_led = open(DEV_NAME, O_WRONLY);
	if (fd_led == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	fd_swt = open(DEV_NAME, O_RDONLY);
	if (fd_swt == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	len = sizeof(value_750);
}

static void
function_swt()
{
	ssize_t n = read(fd_swt, &value_swt, len);
	if (n == -1)
		perror("read");

	/* due to pullup */
	if (value_swt == 1)
		value_swt = 0;
	else
		value_swt = 1;

	n = write(fd_led, &value_swt, len);
	if (n == -1)
		perror("write");
}

static void
function_750()
{
	ssize_t n = write(fd_750, &value_750, len);
	if (n == -1)
		perror("write");

	if (value_750 == 1)
		value_750 = 0;
	else
		value_750 = 1;
}

static void
fiber_100 (void)
{
	function_swt();
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
		.func = fiber_100,
		.sched_param = { .sched_priority = 90,
		},
		.cpu = 0,
		.policy = SCHED_FIFO,
		.dt = MS_TO_NS(100),
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

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

#ifndef _CONF_SCHED_H_
#define _CONF_SCHED_H_

#ifndef __USE_GNU
#define __USE_GNU
#endif
#define _GNU_SOURCE

// more or less common inc
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <limits.h>
#include <linux/limits.h>
#include <sysexits.h>
#include <ctype.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <sys/times.h>
#include <sys/syscall.h>


#define MS_TO_NS(val) (val * 1000000)
#define NSEC_PER_SEC 1000000000

typedef struct {
	pid_t kernel_tid;
	pthread_t tid;
	void (*func) (void);
	struct sched_param sched_param;
	struct timespec t;
	int dt;
} fiber_element_t;


// see https://en.wikipedia.org/wiki/Fiber_%28computer_science%29
void *
fiber(void *arg);

int
build_sched_table(fiber_element_t fiber_array[], int count);

int
set_sched_props(fiber_element_t fiber_array[], int count);

int
start_sched_table(fiber_element_t fiber_array[], int count);

#endif

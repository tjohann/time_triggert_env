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

#ifndef _LIBTTENV_H_
#define _LIBTTENV_H_

#include "libhelper.h"

typedef struct {
	pid_t kernel_tid;
	pthread_t tid;
	void (*func) (void);
	struct sched_param sched_param;
	struct timespec t;
	int policy;
	int cpu;
	int dt;
} fiber_element_t;


/* see https://en.wikipedia.org/wiki/Fiber_%28computer_science%29 */
void *
fiber(void *arg);

/* build table of functions for a fiber */
int
build_sched_table(fiber_element_t fiber_array[], int count);

/* set scheduling properties of a fiber */
int
set_sched_props(fiber_element_t fiber_array[], int count);

/* start a fiber -> wait for pthread_exit */
int
start_sched_table(fiber_element_t fiber_array[], int count);

#endif

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

#include "conf_sched.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*
 * see https://en.wikipedia.org/wiki/Fiber_%28computer_science%29
 */
void *
fiber(void *arg)
{
	fiber_element_t *fiber = (fiber_element_t *) arg;
	pid_t kernel_tid = (pid_t) syscall(SYS_gettid);

	fiber->kernel_tid = kernel_tid;

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);

	clock_gettime(CLOCK_MONOTONIC, &fiber->t);
	fiber->t.tv_sec += 1;
	fiber->t.tv_nsec = 0;

	int s = 0;
	for (;;) {
		s = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
				    &fiber->t, NULL);
		if (s == EINTR) {
			printf("interrupted by signal\n");
			continue;
		}

		fiber->func();
		
		fiber->t.tv_nsec += fiber->dt;
		while (fiber->t.tv_nsec >= NSEC_PER_SEC) {
			fiber->t.tv_nsec -= NSEC_PER_SEC;
			fiber->t.tv_sec++;
		}
	}

	return NULL;
}

int
build_sched_table(fiber_element_t fiber_array[], int count)
{
	int ret = -1;
	int i = 0;
	
	for (i = 0; i < count; i++) {
		ret = pthread_create(&fiber_array[i].tid,
					 NULL,
					 fiber,
					 (void *) &fiber_array[i]);
		if (ret != 0) {
			errno = ret;
			perror("pthread_create");
			return -1;
		}
	}

	sleep (1);
	
	return 0;
}

int
set_sched_props(fiber_element_t fiber_array[], int count)
{
	fiber_element_t *fiber = NULL;
	
	int i = 0;
	for (i = 0; i < count; i++) {
		fiber = &fiber_array[i];

		if (sched_setscheduler(fiber->kernel_tid, SCHED_FIFO,
				       &fiber->sched_param)) {
			perror("could not set scheduling policy SCHED_FIFO");
			return -1;
		}
	}

	return 0;
}

int
start_sched_table(fiber_element_t fiber_array[], int count)
{
	int ret = -1;
	
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);

	int i = 0;
	for (i = 0; i < count; i++) {
		ret = pthread_join(fiber_array[i].tid, NULL);
		if (ret != 0) {
			errno = ret;
			perror("pthread_join");
			return -1;
		}
	}

	return 0;
}

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

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// see https://en.wikipedia.org/wiki/Fiber_%28computer_science%29
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
			printf("pthread_create\n");
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
			printf("could not set scheduling policy SCHED_FIFO\n");
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
			printf("pthread_join\n");
			return -1;
		}
	}

	return 0;
}

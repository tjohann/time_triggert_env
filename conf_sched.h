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
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <wait.h>
#include <limits.h>
#include <linux/limits.h>
#include <sysexits.h>
#include <ctype.h>
#include <pthread.h>
#include <bsd/stdlib.h>
#include <sys/utsname.h>
#include <sched.h>
#include <sys/un.h>
#include <sys/socket.h>
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

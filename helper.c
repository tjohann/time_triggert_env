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

void
show_clock_resolution(void)
{
	struct timespec res;

	if (clock_getres(CLOCK_MONOTONIC, &res) == 0)
		printf("Clock resolution is %lu nsec\n", res.tv_nsec);
	else
		perror("Can't get clock resolution");
}

void
stack_prefault(size_t size)
{
        unsigned char dummy[size * BASE_SAFE_SIZE];
        memset(dummy, 0, size * BASE_SAFE_SIZE);
}

int
heap_prefault(size_t size)
{
        unsigned char *dummy;
	size_t len = size * BASE_SAFE_SIZE;

	dummy = malloc(len);
	if (dummy == NULL) {
		perror("malloc");
		exit (EXIT_FAILURE);
	}

	memset(dummy, 0, len);

	return 0;
}

bool
check_for_rtpreempt()
{
	FILE *fd;
	struct utsname u;

	if (uname(&u) == -1) {
		perror("uname");
		return false;
	}

	if (strstr(u.version, "PREEMPT") == NULL) {
		printf("NO preempt kernel\n");
		return false;
	}

	int flag;
	if ((fd = fopen("/sys/kernel/realtime","r")) != NULL) {
		if ((fscanf(fd, "%d", &flag) == 1) && (flag == 1)) {
			printf("Kernel is a RT-PREEMPT kernel\n");
			goto out;
		} else {
			printf("Kernel is a PREEMPT kernel\n");
			goto out;
		}
	} else {
		printf("Kernel is a PREEMPT kernel\n");
		goto out;
	}

out:
	if (fd != NULL)
		fclose(fd);

	return true;
}

int
drop_capability(int hold_cap)
{
	capng_clear(CAPNG_SELECT_BOTH);

	if (capng_update(CAPNG_ADD, CAPNG_EFFECTIVE | CAPNG_PERMITTED,
			 hold_cap) != 0) {
		printf("could not set capability -> %s\n",
		       capng_print_caps_text(CAPNG_PRINT_STDOUT,
					     CAPNG_EFFECTIVE));
		return -1;
	}

	capng_apply(CAPNG_SELECT_BOTH);

	return 0;
}

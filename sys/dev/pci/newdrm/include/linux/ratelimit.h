/* Public domain. */

#ifndef _LINUX_RATELIMIT_H
#define _LINUX_RATELIMIT_H

#define DEFINE_RATELIMIT_STATE(name, interval, burst) \
	int name __used = 1;

#define __ratelimit(x)	(1)

#define ratelimit_state_init(x, y, z)
#define ratelimit_set_flags(x, y)

#endif

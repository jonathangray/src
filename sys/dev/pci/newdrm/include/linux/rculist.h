/* Public domain. */

#ifndef _LINUX_RCULIST_H
#define _LINUX_RCULIST_H

#include <linux/list.h>
#include <linux/rcupdate.h>

#define list_add_rcu(a, b)		list_add(a, b)
#define list_del_rcu(a)			list_del(a)

#endif

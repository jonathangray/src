/* Public domain. */

#ifndef _LINUX_IO_H
#define _LINUX_IO_H

#include <sys/systm.h>
#include <linux/types.h>
#include <linux/compiler.h>

#define memcpy_toio(d, s, n)	memcpy(d, s, n)
#define memcpy_fromio(d, s, n)	memcpy(d, s, n)
#define memset_io(d, b, n)	memset(d, b, n)

static inline u32
ioread32(const volatile void __iomem *addr)
{
	return (*(volatile uint32_t *)addr);
}

static inline u64
ioread64(const volatile void __iomem *addr)
{
	return (*(volatile uint64_t *)addr);
}

static inline void
iowrite32(u32 val, volatile void __iomem *addr)
{
	*(volatile uint32_t *)addr = val;
}

static inline void
iowrite64(u64 val, volatile void __iomem *addr)
{
	*(volatile uint64_t *)addr = val;
}

#define readl(p) ioread32(p)
#define writel(v, p) iowrite32(v, p)
#define readq(p) ioread64(p)
#define writeq(v, p) iowrite64(v, p)

#endif

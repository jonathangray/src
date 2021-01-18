// SPDX-License-Identifier: MIT
/*
 * Copyright © 2020 Intel Corporation
 */

#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/shmem_fs.h>

#include "gem/i915_gem_object.h"
#include "shmem_utils.h"

#ifdef __linux__

struct file *shmem_create_from_data(const char *name, void *data, size_t len)
{
	struct file *file;
	int err;

	file = shmem_file_setup(name, PAGE_ALIGN(len), VM_NORESERVE);
	if (IS_ERR(file))
		return file;

	err = shmem_write(file, 0, data, len);
	if (err) {
		fput(file);
		return ERR_PTR(err);
	}

	return file;
}

struct file *shmem_create_from_object(struct drm_i915_gem_object *obj)
{
	struct file *file;
	void *ptr;

	if (obj->ops == &i915_gem_shmem_ops) {
		file = obj->base.filp;
		atomic_long_inc(&file->f_count);
		return file;
	}

	ptr = i915_gem_object_pin_map(obj, I915_MAP_WB);
	if (IS_ERR(ptr))
		return ERR_CAST(ptr);

	file = shmem_create_from_data("", ptr, obj->base.size);
	i915_gem_object_unpin_map(obj);

	return file;
}

void *shmem_pin_map(struct file *file)
{
	struct page **pages;
	size_t n_pages, i;
	void *vaddr;

	n_pages = file->f_mapping->host->i_size >> PAGE_SHIFT;
	pages = kvmalloc_array(n_pages, sizeof(*pages), GFP_KERNEL);
	if (!pages)
		return NULL;

	for (i = 0; i < n_pages; i++) {
		pages[i] = shmem_read_mapping_page_gfp(file->f_mapping, i,
						       GFP_KERNEL);
		if (IS_ERR(pages[i]))
			goto err_page;
	}

	vaddr = vmap(pages, n_pages, VM_MAP_PUT_PAGES, PAGE_KERNEL);
	if (!vaddr)
		goto err_page;
	mapping_set_unevictable(file->f_mapping);
	return vaddr;
err_page:
	while (i--)
		put_page(pages[i]);
	kvfree(pages);
	return NULL;
}

void shmem_unpin_map(struct file *file, void *ptr)
{
	mapping_clear_unevictable(file->f_mapping);
	vfree(ptr);
}

static int __shmem_rw(struct file *file, loff_t off,
		      void *ptr, size_t len,
		      bool write)
{
	unsigned long pfn;

	for (pfn = off >> PAGE_SHIFT; len; pfn++) {
		unsigned int this =
			min_t(size_t, PAGE_SIZE - offset_in_page(off), len);
		struct vm_page *page;
		void *vaddr;

		page = shmem_read_mapping_page_gfp(file->f_mapping, pfn,
						   GFP_KERNEL);
		if (IS_ERR(page))
			return PTR_ERR(page);

		vaddr = kmap(page);
		if (write) {
			memcpy(vaddr + offset_in_page(off), ptr, this);
			set_page_dirty(page);
		} else {
			memcpy(ptr, vaddr + offset_in_page(off), this);
		}
		mark_page_accessed(page);
#ifdef __linux__
		kunmap(page);
#else
		kunmap_va(vaddr);
#endif
		put_page(page);

		len -= this;
		ptr += this;
		off = 0;
	}

	return 0;
}

int shmem_read(struct file *file, loff_t off, void *dst, size_t len)
{
	return __shmem_rw(file, off, dst, len, false);
}

int shmem_write(struct file *file, loff_t off, void *src, size_t len)
{
	return __shmem_rw(file, off, src, len, true);
}

#endif /* __linux__ */

struct uvm_object *
uobj_create_from_object(struct drm_i915_gem_object *obj)
{
	struct uvm_object *uobj;
	void *ptr;
	struct pglist from_plist, to_plist;
	struct vm_page *from_page;
	struct vm_page *to_page;
	int i;

#ifdef notyet
	if (obj->ops == &i915_gem_shmem_ops) {
		file = obj->base.filp;
		atomic_long_inc(&file->f_count);
		return file;
	}
#endif

	ptr = i915_gem_object_pin_map(obj, I915_MAP_WB);
	if (IS_ERR(ptr))
		return ERR_CAST(ptr);

	uobj = uao_create(obj->base.size, 0);

	TAILQ_INIT(&from_plist);
	if (uvm_objwire(obj->base.uao, 0, obj->base.size, &from_plist))
		return ERR_PTR(-ENOMEM);

	TAILQ_INIT(&to_plist);
	if (uvm_objwire(uobj, 0, obj->base.size, &to_plist))
		return ERR_PTR(-ENOMEM);

	from_page = TAILQ_FIRST(&from_plist);
	to_page = TAILQ_FIRST(&to_plist);
	for (i = 0; i < obj->base.size >> PAGE_SHIFT; ++i) {
		uvm_pagecopy(from_page, to_page);
		to_page = TAILQ_NEXT(to_page, pageq);
		from_page = TAILQ_NEXT(from_page, pageq);
	}

	uvm_objunwire(uobj, 0, obj->base.size);
	uvm_objunwire(obj->base.uao, 0, obj->base.size);

	i915_gem_object_unpin_map(obj);

	return uobj;
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "st_shmem_utils.c"
#endif

/*
 * Copyright (C) 2010, 2011, 2012, 2013 Mail.RU
 * Copyright (C) 2010, 2011, 2012, 2013 Yuriy Vostrikov
 * Copyright (C) 2012, 2013 Roman Tokarev
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _PALLOC_H_
#define _PALLOC_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct palloc_pool;
struct palloc_cut_point;
typedef void (* palloc_nomem_cb_t)(struct palloc_pool *, void *, size_t);
struct palloc_config {
	const char *name;
	const void *ctx;

	size_t size;

	palloc_nomem_cb_t nomem_cb;

	bool reuse;
	size_t release_after;
};

#ifdef PALLOC_CUSTOM_PREFIX
# define PALLOC_NAME_CAT(a_, b_) a_ ## b_
# define PALLOC_NAME_ECAT(a_, b_) PALLOC_NAME_CAT(a_, b_)
# define PALLOC_NAME(name_) PALLOC_NAME_ECAT(PALLOC_CUSTOM_PREFIX, name_)
#else
# define PALLOC_NAME(name_) name_
#endif

void *PALLOC_NAME(palloc)(struct palloc_pool *pool, size_t size);
void *PALLOC_NAME(prealloc)(struct palloc_pool *pool, void *oldptr, size_t oldsize, size_t size);
void *PALLOC_NAME(p0alloc)(struct palloc_pool *pool, size_t size);
void *PALLOC_NAME(palloca)(struct palloc_pool *pool, size_t size, size_t align);
void PALLOC_NAME(prelease)(struct palloc_pool *pool);
void PALLOC_NAME(prelease_after)(struct palloc_pool *pool, size_t after);
struct palloc_pool *PALLOC_NAME(palloc_create_pool)(struct palloc_config cfg);
struct palloc_pool *PALLOC_NAME(palloc_create_child_pool)(struct palloc_pool *pool, struct palloc_config cfg);
void PALLOC_NAME(palloc_destroy_pool)(struct palloc_pool *);
void PALLOC_NAME(palloc_unmap_unused)(void);
const char *PALLOC_NAME(palloc_name)(struct palloc_pool *, const char *);
void *PALLOC_NAME(palloc_ctx)(struct palloc_pool *, const void *);
palloc_nomem_cb_t PALLOC_NAME(palloc_nomem_cb)(struct palloc_pool *, palloc_nomem_cb_t);
size_t PALLOC_NAME(palloc_size)(struct palloc_pool *, size_t *size);
size_t PALLOC_NAME(palloc_allocated)(struct palloc_pool *);
bool PALLOC_NAME(palloc_child)(struct palloc_pool *parent_pool, struct palloc_pool *pool);

void PALLOC_NAME(palloc_register_gc_root)(struct palloc_pool *pool,
					  void *ptr, void (*copy)(struct palloc_pool *, void *));
void PALLOC_NAME(palloc_unregister_gc_root)(struct palloc_pool *pool, void *ptr);
void PALLOC_NAME(palloc_gc)(struct palloc_pool *pool);

struct palloc_cut_point *PALLOC_NAME(palloc_register_cut_point)(struct palloc_pool *pool);
// cut off to the latest cut point
void PALLOC_NAME(palloc_cutoff)(struct palloc_pool *pool);
// palloc_cutoff_to(pool, NULL) == palloc_cutoff
void PALLOC_NAME(palloc_cutoff_to)(struct palloc_pool *pool, struct palloc_cut_point *cut_point);

struct tbuf;
void PALLOC_NAME(palloc_stat_info)(struct tbuf *buf);
bool PALLOC_NAME(palloc_owner)(struct palloc_pool *pool, void *ptr);

#ifdef PALLOC_CUSTOM_PREFIX
# define palloc PALLOC_NAME(palloc)
# define prealloc PALLOC_NAME(prealloc)
# define p0alloc PALLOC_NAME(p0alloc)
# define palloca PALLOC_NAME(palloca)
# define prelease PALLOC_NAME(prelease)
# define prelease_after PALLOC_NAME(prelease_after)

# define palloc_create_pool PALLOC_NAME(palloc_create_pool)
# define palloc_create_child_pool PALLOC_NAME(palloc_create_child_pool)
# define palloc_destroy_pool PALLOC_NAME(palloc_destroy_pool)
# define palloc_unmap_unused PALLOC_NAME(palloc_unmap_unused)
# define palloc_name PALLOC_NAME(palloc_name)
# define palloc_ctx PALLOC_NAME(palloc_ctx)
# define palloc_nomem_cb PALLOC_NAME(palloc_nomem_cb)
# define palloc_size PALLOC_NAME(palloc_size)
# define palloc_allocated PALLOC_NAME(palloc_allocated)
# define palloc_child PALLOC_NAME(palloc_child)

# define palloc_register_gc_root PALLOC_NAME(palloc_register_gc_root)
# define palloc_unregister_gc_root PALLOC_NAME(palloc_unregister_gc_root)
# define palloc_gc PALLOC_NAME(palloc_gc)

# define palloc_register_cut_point PALLOC_NAME(palloc_register_cut_point)
# define palloc_cutoff PALLOC_NAME(palloc_cutoff)
# define palloc_cutoff_to PALLOC_NAME(palloc_cutoff_to)

# define palloc_stat_info PALLOC_NAME(palloc_stat_info)
# define palloc_owner PALLOC_NAME(palloc_owner)
#endif // PALLOC_CUSTOM_PREFIX

#endif // _PALLOC_H_

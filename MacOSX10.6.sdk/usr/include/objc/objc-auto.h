/*
 * Copyright (c) 2004-2007 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _OBJC_AUTO_H_
#define _OBJC_AUTO_H_

#include <objc/objc.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <Availability.h>
#include <AvailabilityMacros.h>
#include <TargetConditionals.h>

#if TARGET_OS_WIN32
    static __inline BOOL OSAtomicCompareAndSwapPtr(void *oldp, void *newp, void *volatile *dst) { void *original = InterlockedCompareExchangePointer(dst, newp, oldp); return (original == oldp); }
    static __inline BOOL OSAtomicCompareAndSwapPtrBarrier(void *oldp, void *newp, void *volatile *dst) { void *original = InterlockedCompareExchangePointer(dst, newp, oldp); return (original == oldp); }
#else
#   include <sys/types.h>
#   include <libkern/OSAtomic.h>
#endif


__BEGIN_DECLS

/* GC is unsupported on some architectures. */

#if TARGET_OS_EMBEDDED  ||  TARGET_OS_WIN32
#   define OBJC_NO_GC 1
#endif

#ifdef OBJC_NO_GC
#   define OBJC_GC_EXPORT static
#else
#   define OBJC_GC_EXPORT OBJC_EXPORT
#endif


/* objc_collect() options */
enum {
    // choose one
    OBJC_RATIO_COLLECTION        = (0 << 0),  // run "ratio" generational collections, then a full
    OBJC_GENERATIONAL_COLLECTION = (1 << 0),  // run fast incremental collection
    OBJC_FULL_COLLECTION         = (2 << 0),  // run full collection.
    OBJC_EXHAUSTIVE_COLLECTION   = (3 << 0),  // run full collections until memory available stops improving
    
    OBJC_COLLECT_IF_NEEDED       = (1 << 3), // run collection only if needed (allocation threshold exceeded)
    OBJC_WAIT_UNTIL_DONE         = (1 << 4), // wait (when possible) for collection to end before returning (when collector is running on dedicated thread)
};

/* objc_collect_if_needed() options */
enum {
    OBJC_GENERATIONAL = (1 << 0)
};

/* objc_clear_stack() options */
enum {
    OBJC_CLEAR_RESIDENT_STACK = (1 << 0)
};

/* objc_setAssociatedObject() options */
enum {
    OBJC_ASSOCIATION_ASSIGN = 0,
    OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1,
    OBJC_ASSOCIATION_COPY_NONATOMIC = 3,
    OBJC_ASSOCIATION_RETAIN = 01401,
    OBJC_ASSOCIATION_COPY = 01403
};
typedef uintptr_t objc_AssociationPolicy;


/* Collection utilities */

OBJC_GC_EXPORT void objc_collect(unsigned long options);
OBJC_GC_EXPORT BOOL objc_collectingEnabled(void);


/* GC configuration */

/* Tells collector to wait until specified bytes have been allocated before trying to collect again. */
OBJC_GC_EXPORT void objc_setCollectionThreshold(size_t threshold);

/* Tells collector to run a full collection for every ratio generational collections. */
OBJC_GC_EXPORT void objc_setCollectionRatio(size_t ratio);

/* Tells collector to start collecting on dedicated thread */
OBJC_GC_EXPORT void objc_startCollectorThread(void);

/* Atomic update, with write barrier. */
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapPtr(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_NA);
/* "Barrier" version also includes memory barrier. */
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapPtrBarrier(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_NA);

/* Write barriers.  Used by the compiler.  */
OBJC_GC_EXPORT id objc_assign_strongCast(id val, id *dest);
OBJC_GC_EXPORT id objc_assign_global(id val, id *dest);
OBJC_GC_EXPORT id objc_assign_ivar(id value, id dest, ptrdiff_t offset);
OBJC_GC_EXPORT void *objc_memmove_collectable(void *dst, const void *src, size_t size);

OBJC_GC_EXPORT id objc_read_weak(id *location);
OBJC_GC_EXPORT id objc_assign_weak(id value, id *location);

/* Associated Object support. */
OBJC_EXPORT void objc_setAssociatedObject(id object, void *key, id value, objc_AssociationPolicy policy);
OBJC_EXPORT id objc_getAssociatedObject(id object, void *key);
OBJC_EXPORT void objc_removeAssociatedObjects(id object);

//
// Deprecated.
// 

// atomic update of a global variable
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapGlobal(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapGlobalBarrier(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);
// atomic update of an instance variable
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapInstanceVariable(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);
OBJC_GC_EXPORT BOOL objc_atomicCompareAndSwapInstanceVariableBarrier(id predicate, id replacement, volatile id *objectLocation) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);


//
// SPI. The following routines are available as debugging and transitional aides.
//

/* Tells runtime to issue finalize calls on the main thread only. */
OBJC_GC_EXPORT void objc_finalizeOnMainThread(Class cls)
    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER_BUT_DEPRECATED;


/* ask if object is scheduled for finalization.  Safe only when called from within a finalizer.  */
OBJC_GC_EXPORT BOOL objc_is_finalized(void *ptr);

/* Stack management */
OBJC_GC_EXPORT void objc_clear_stack(unsigned long options);

//
// Obsolete.  Present only until all uses can be migrated to newer API.
//

OBJC_GC_EXPORT BOOL objc_collecting_enabled(void);
OBJC_GC_EXPORT void objc_set_collection_threshold(size_t threshold);
OBJC_GC_EXPORT void objc_set_collection_ratio(size_t ratio);
OBJC_GC_EXPORT void objc_start_collector_thread(void);

/* Memory management */
OBJC_EXPORT id objc_allocate_object(Class cls, int extra);

OBJC_GC_EXPORT void objc_collect_if_needed(unsigned long options);

/* Register/unregister the current thread with the garbage collector. */
OBJC_GC_EXPORT void objc_registerThreadWithCollector();
OBJC_GC_EXPORT void objc_unregisterThreadWithCollector();

/* To be called from code which must only execute on a registered thread. */
/* If the calling thread is unregistered then an error message is emitted and the thread is implicitly registered. */
OBJC_GC_EXPORT void objc_assertRegisteredThreadWithCollector();

#ifdef OBJC_NO_GC

/* Non-GC versions */

static OBJC_INLINE void objc_collect(unsigned long options) { }
static OBJC_INLINE BOOL objc_collectingEnabled(void) { return NO; }
static OBJC_INLINE void objc_setCollectionThreshold(size_t threshold) { }
static OBJC_INLINE void objc_setCollectionRatio(size_t ratio) { }
static OBJC_INLINE void objc_startCollectorThread(void) { }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapPtr(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtr((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapPtrBarrier(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtrBarrier((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapGlobal(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtr((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapGlobalBarrier(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtrBarrier((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapInstanceVariable(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtr((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }

static OBJC_INLINE BOOL objc_atomicCompareAndSwapInstanceVariableBarrier(id predicate, id replacement, volatile id *objectLocation) 
    { return OSAtomicCompareAndSwapPtrBarrier((void *)predicate, (void *)replacement, (void * volatile *)objectLocation); }


static OBJC_INLINE id objc_assign_strongCast(id val, id *dest) 
    { return (*dest = val); }

static OBJC_INLINE id objc_assign_global(id val, id *dest) 
    { return (*dest = val); }

static OBJC_INLINE id objc_assign_ivar(id val, id dest, ptrdiff_t offset) 
    { return (*(id*)((char *)dest+offset) = val); }

static OBJC_INLINE void *objc_memmove_collectable(void *dst, const void *src, size_t size) 
    { return memmove(dst, src, size); }

static OBJC_INLINE id objc_read_weak(id *location) 
    { return *location; }

static OBJC_INLINE id objc_assign_weak(id value, id *location) 
    { return (*location = value); }


static OBJC_INLINE void objc_finalizeOnMainThread(Class cls) { }
static OBJC_INLINE BOOL objc_is_finalized(void *ptr) { return NO; }
static OBJC_INLINE void objc_clear_stack(unsigned long options) { }

static OBJC_INLINE BOOL objc_collecting_enabled(void) { return NO; }
static OBJC_INLINE void objc_set_collection_threshold(size_t threshold) { } 
static OBJC_INLINE void objc_set_collection_ratio(size_t ratio) { } 
static OBJC_INLINE void objc_start_collector_thread(void) { }

OBJC_EXPORT id class_createInstance(Class cls, size_t extraBytes);
static OBJC_INLINE id objc_allocate_object(Class cls, int extra) 
    { return class_createInstance(cls, extra); }
static OBJC_INLINE void objc_collect_if_needed(unsigned long options) { }

static OBJC_INLINE void objc_registerGCThread() { }
static OBJC_INLINE void objc_unregisterGCThread() { }
static OBJC_INLINE void objc_assertRegisteredGCThread() { }

#endif

__END_DECLS

#endif

/*
 * Copyright (c) 2000-2007 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * NOTICE: This file was modified by SPARTA, Inc. in 2005 to introduce
 * support for mandatory and extensible security protections.  This notice
 * is included in support of clause 2.2 (b) of the Apple Public License,
 * Version 2.0.
 */

#ifndef    _MACH_KMOD_H_
#define    _MACH_KMOD_H_

#include <mach/kern_return.h>
#include <mach/mach_types.h>

#include <sys/cdefs.h>

/***********************************************************************
* kmod_control() commands. 1-5 are long-established. 6-8 are new in
* Leopard and used to reliably get and verify symbol information needed
* to link kexts against the running kernel, or to disable kmod loading
* if such symbol information cannot be found.
***********************************************************************/
#define KMOD_CNTL_START                   1    // call kmod's start routine
#define KMOD_CNTL_STOP                    2    // call kmod's stop routine
#define KMOD_CNTL_RETAIN                  3    // increase a kmod's reference count
#define KMOD_CNTL_RELEASE                 4    // decrease a kmod's reference count
#define KMOD_CNTL_GET_CMD                 5    // get kmod load cmd from kernel

#define KMOD_CNTL_GET_KERNEL_SYMBOLS      6  // get symfile as data buffer
#define KMOD_CNTL_FREE_LINKEDIT_DATA      7  // refuse to create new kmods
#define KMOD_CNTL_GET_KERNEL_UUID         8  // LC_UUID load command payload
#define KMOD_CNTL_GET_UUID                8  // LC_UUID load command payload
#define KMOD_CNTL_DISABLE_LOAD            9  // refuse to create new kmods
#define KMOD_CNTL_CUTOVER_SEND_PLIST      10  // xxx - for cutover only!
#define KMOD_CNTL_CUTOVER_SEND_LINKSTATE  11  // xxx - for cutover only!

#define KMOD_PACK_IDS(from, to)    (((unsigned long)from << 16) | (unsigned long)to)
#define KMOD_UNPACK_FROM_ID(i)     ((unsigned long)i >> 16)
#define KMOD_UNPACK_TO_ID(i)       ((unsigned long)i & 0xffff)

typedef int kmod_t;
typedef int kmod_control_flavor_t;
typedef void* kmod_args_t;

#define KMOD_MAX_NAME    64

#pragma pack(4)

/* LP64todo - not 64-bit safe */
typedef struct kmod_reference {
    struct kmod_reference * next;
    struct kmod_info      * info;
} kmod_reference_t;

#pragma pack()

/**********************************************************************/
/* warning any changes to this structure affect the following macros. */    
/**********************************************************************/

#define KMOD_RETURN_SUCCESS    KERN_SUCCESS
#define KMOD_RETURN_FAILURE    KERN_FAILURE

typedef kern_return_t kmod_start_func_t(struct kmod_info * ki, void * data);
typedef kern_return_t kmod_stop_func_t(struct kmod_info * ki, void * data);

#pragma pack(4)

/* LP64todo - not 64-bit safe */

typedef struct kmod_info {
    struct kmod_info  * next;
    int                 info_version;           // version of this structure
    int                 id;
    char                name[KMOD_MAX_NAME];
    char                version[KMOD_MAX_NAME];
    int                 reference_count;        // # refs to this 
    kmod_reference_t  * reference_list;         // who this refs
    vm_address_t        address;                // starting address
    vm_size_t           size;                   // total size
    vm_size_t           hdr_size;               // unwired hdr size
    kmod_start_func_t * start;
    kmod_stop_func_t  * stop;
} kmod_info_t;

#pragma pack()

typedef kmod_info_t *kmod_info_array_t;

#define KMOD_INFO_NAME       kmod_info
#define KMOD_INFO_VERSION    1

#define KMOD_DECL(name, version)                                  \
    static kmod_start_func_t name ## _module_start;               \
    static kmod_stop_func_t  name ## _module_stop;                \
    kmod_info_t KMOD_INFO_NAME = { 0, KMOD_INFO_VERSION, -1,      \
                       { #name }, { version }, -1, 0, 0, 0, 0,    \
                           name ## _module_start,                 \
                           name ## _module_stop };

#define KMOD_EXPLICIT_DECL(name, version, start, stop)            \
    kmod_info_t KMOD_INFO_NAME = { 0, KMOD_INFO_VERSION, -1,      \
                       { #name }, { version }, -1, 0, 0, 0, 0,    \
                           start, stop };

// the following is useful for libaries that don't need their own start and stop functions
#define KMOD_LIB_DECL(name, version)                              \
    kmod_info_t KMOD_INFO_NAME = { 0, KMOD_INFO_VERSION, -1,      \
                       { #name }, { version }, -1, 0, 0, 0, 0,    \
                           kmod_default_start,                    \
                       kmod_default_stop };

/* Helper structure definitions for working with kmod_info_t on non-native
 * architectures.
 */

#pragma pack(4)

typedef struct kmod_info_32_v1 {
    uint32_t            next_addr;
    uint32_t            info_version;
    uint32_t            id;
    uint8_t             name[KMOD_MAX_NAME];
    uint8_t             version[KMOD_MAX_NAME];
    uint32_t            reference_count;
    uint32_t            reference_list_addr;
    uint32_t            address;
    uint32_t            size;
    uint32_t            hdr_size;
    uint32_t            start_addr;
    uint32_t            stop_addr;
} kmod_info_32_v1_t;

typedef struct kmod_info_64_v1 {
    uint64_t            next_addr;
    uint32_t            info_version;
    uint32_t            id;
    uint8_t             name[KMOD_MAX_NAME];
    uint8_t             version[KMOD_MAX_NAME];
    uint32_t            reference_count;
    uint64_t            reference_list_addr;
    uint64_t            address;
    uint64_t            size;
    uint64_t            hdr_size;
    uint64_t            start_addr;
    uint64_t            stop_addr;
} kmod_info_64_v1_t;

#pragma pack()

// *************************************************************************************
// kmod kernel to user commands
// *************************************************************************************

#define KMOD_LOAD_EXTENSION_PACKET            1
#define KMOD_LOAD_WITH_DEPENDENCIES_PACKET    2

// for generic packets
#define KMOD_IOKIT_START_RANGE_PACKET         0x1000
#define KMOD_IOKIT_END_RANGE_PACKET           0x1fff

typedef struct kmod_load_extension_cmd {
    int   type;
    char  name[KMOD_MAX_NAME];
} kmod_load_extension_cmd_t;

typedef struct kmod_load_with_dependencies_cmd {
    int   type;
    char  name[KMOD_MAX_NAME];
    char  dependencies[1][KMOD_MAX_NAME];
} kmod_load_with_dependencies_cmd_t;

typedef struct kmod_generic_cmd {
    int   type;
    char  data[1];
} kmod_generic_cmd_t;


#endif    /* _MACH_KMOD_H_ */

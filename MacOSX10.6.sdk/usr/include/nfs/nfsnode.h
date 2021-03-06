/*
 * Copyright (c) 2000-2008 Apple Inc. All rights reserved.
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
/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */
/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)nfsnode.h	8.9 (Berkeley) 5/14/95
 * FreeBSD-Id: nfsnode.h,v 1.24 1997/10/28 14:06:25 bde Exp $
 */


#ifndef _NFS_NFSNODE_H_
#define _NFS_NFSNODE_H_

#include <sys/appleapiopts.h>

#ifdef __APPLE_API_PRIVATE
#ifndef _NFS_NFS_H_
#include <nfs/nfs.h>
#endif

/*
 * Silly rename structure that hangs off the nfsnode until the name
 * can be removed by nfs_vnop_inactive()
 */
struct nfs_sillyrename {
	kauth_cred_t	nsr_cred;
	struct nfsnode	*nsr_dnp;
	int		nsr_namlen;
	char		nsr_name[20];
};

/*
 * This structure is used to save the logical directory offset to
 * NFS cookie mappings.
 * The mappings are stored in a list headed
 * by n_cookies, as required.
 * There is one mapping for each NFS_DIRBLKSIZ bytes of directory information
 * stored in increasing logical offset byte order.
 */
#define NFSNUMCOOKIES		31

struct nfsdmap {
	LIST_ENTRY(nfsdmap)	ndm_list;
	int			ndm_eocookie;
	nfsuint64		ndm_cookies[NFSNUMCOOKIES];
};

/*
 * The nfsbuf is the nfs equivalent to a struct buf.
 */
struct nfsbuf {
	LIST_ENTRY(nfsbuf)	nb_hash;	/* hash chain */
	LIST_ENTRY(nfsbuf)	nb_vnbufs;	/* nfsnode's nfsbuf chain */
	TAILQ_ENTRY(nfsbuf)	nb_free;	/* free list position if not active. */
	volatile long		nb_flags;	/* NB_* flags. */
	volatile long		nb_lflags;	/* NBL_* flags. */
	volatile long		nb_refs;	/* outstanding references. */
	long			nb_bufsize;	/* buffer size */
	daddr64_t		nb_lblkno;	/* logical block number. */
	uint64_t		nb_verf;	/* V3 write verifier */
	int			nb_commitlevel;	/* lowest write commit level */
	time_t			nb_timestamp;	/* buffer timestamp */
	int			nb_error;	/* errno value. */
	u_int32_t		nb_valid;	/* valid pages in buf */
	u_int32_t		nb_dirty;	/* dirty pages in buf */
	int			nb_validoff;	/* offset in buffer of valid region. */
	int			nb_validend;	/* offset of end of valid region. */
	int			nb_dirtyoff;	/* offset in buffer of dirty region. */
	int			nb_dirtyend;	/* offset of end of dirty region. */
	int			nb_offio;	/* offset in buffer of I/O region. */
	int			nb_endio;	/* offset of end of I/O region. */
	int			nb_rpcs;	/* Count of RPCs remaining for this buffer. */
	caddr_t			nb_data;	/* mapped buffer */
	nfsnode_t		nb_np;		/* nfsnode buffer belongs to */
	kauth_cred_t		nb_rcred;	/* read credentials reference */
	kauth_cred_t		nb_wcred;	/* write credentials reference */
	void *			nb_pagelist;	/* upl */
};

#define NFS_MAXBSIZE	(32 * PAGE_SIZE)	/* valid/dirty page masks limit buffer size */

#define NFS_A_LOT_OF_NEEDCOMMITS	256			/* max# uncommitted buffers for a node */
#define NFS_A_LOT_OF_DELAYED_WRITES	MAX(nfsbufcnt/8,512)	/* max# "delwri" buffers in system */

/*
 * These flags are kept in b_lflags...
 * nfs_buf_mutex must be held before examining/updating
 */
#define	NBL_BUSY	0x00000001	/* I/O in progress. */
#define	NBL_WANTED	0x00000002	/* Process wants this buffer. */

/*
 * These flags are kept in nb_flags and they're (purposefully)
 * very similar to the B_* flags for struct buf.
 * nfs_buf_mutex is not needed to examine/update these.
 */
#define	NB_STALEWVERF	0x00000001	/* write verifier changed on us */
#define	NB_NEEDCOMMIT	0x00000002	/* buffer needs to be committed */
#define	NB_ASYNC	0x00000004	/* Start I/O, do not wait. */
#define	NB_CACHE	0x00000020	/* buffer data found in the cache */
#define	NB_STABLE	0x00000040	/* write FILESYNC not UNSTABLE */
#define	NB_DELWRI	0x00000080	/* delayed write: dirty range needs to be written */
#define	NB_DONE		0x00000200	/* I/O completed. */
#define	NB_EINTR	0x00000400	/* I/O was interrupted */
#define	NB_ERROR	0x00000800	/* I/O error occurred. */
#define	NB_INVAL	0x00002000	/* Does not contain valid info. */
#define	NB_NCRDAHEAD	0x00004000	/* "nocache readahead" data */
#define	NB_NOCACHE	0x00008000	/* Do not cache block after use. */
#define	NB_WRITE	0x00000000	/* Write buffer (pseudo flag). */
#define	NB_READ		0x00100000	/* Read buffer. */
#define	NB_MULTASYNCRPC	0x00200000	/* multiple async RPCs issued for buffer */
#define	NB_PAGELIST	0x00400000	/* Buffer describes pagelist I/O. */
#define	NB_WRITEINPROG	0x01000000	/* Write in progress. */
#define	NB_META		0x40000000	/* buffer contains meta-data. */

/* Flags for operation type in nfs_buf_get() */
#define	NBLK_READ	0x00000001	/* buffer for read */
#define	NBLK_WRITE	0x00000002	/* buffer for write */
#define	NBLK_META	0x00000004	/* buffer for metadata */
#define	NBLK_OPMASK	0x00000007	/* operation mask */
/* modifiers for above flags... */
#define NBLK_NOWAIT	0x40000000	/* don't wait on busy buffer */
#define NBLK_ONLYVALID	0x80000000	/* only return cached buffer */

/* These flags are used for nfsbuf iterating */
#define NBI_ITER		0x01	/* iteration in progress */
#define NBI_ITERWANT		0x02	/* waiting to iterate */
#define NBI_CLEAN		0x04	/* requesting clean buffers */
#define NBI_DIRTY		0x08	/* requesting dirty buffers */
#define NBI_NOWAIT		0x10	/* don't block on NBI_ITER */

/* Flags for nfs_buf_acquire */
#define NBAC_NOWAIT		0x01	/* Don't wait if buffer is busy */
#define NBAC_REMOVE		0x02	/* Remove from free list once buffer is acquired */

/* some convenience macros...  */
#define NBOFF(BP)			((off_t)(BP)->nb_lblkno * (off_t)(BP)->nb_bufsize)
#define NBPGVALID(BP,P)			(((BP)->nb_valid >> (P)) & 0x1)
#define NBPGDIRTY(BP,P)			(((BP)->nb_dirty >> (P)) & 0x1)
#define NBPGVALID_SET(BP,P)		((BP)->nb_valid |= (1 << (P)))
#define NBPGDIRTY_SET(BP,P)		((BP)->nb_dirty |= (1 << (P)))

#define NBUFSTAMPVALID(BP)		((BP)->nb_timestamp != ~0)
#define NBUFSTAMPINVALIDATE(BP)		((BP)->nb_timestamp = ~0)

#define NFS_BUF_MAP(BP) \
	do { \
		if (!(BP)->nb_data && nfs_buf_map(BP)) \
			panic("nfs_buf_map failed"); \
	} while (0)

LIST_HEAD(nfsbuflists, nfsbuf);
TAILQ_HEAD(nfsbuffreehead, nfsbuf);

#define NFSNOLIST ((struct nfsbuf *)0xdeadbeef)

__private_extern__ lck_mtx_t *nfs_buf_mutex;
__private_extern__ int nfsbufcnt, nfsbufmin, nfsbufmax, nfsbufmetacnt, nfsbufmetamax;
__private_extern__ int nfsbuffreecnt, nfsbuffreemetacnt, nfsbufdelwricnt, nfsneedbuffer;
__private_extern__ int nfs_nbdwrite;
__private_extern__ struct nfsbuffreehead nfsbuffree, nfsbufdelwri;

#ifdef NFSBUFDEBUG
#define NFSBUFCNTCHK() \
	do { \
	if (	(nfsbufcnt < 0) || \
		(nfsbufcnt > nfsbufmax) || \
		(nfsbufmetacnt < 0) || \
		(nfsbufmetacnt > nfsbufmetamax) || \
		(nfsbufmetacnt > nfsbufcnt) || \
		(nfsbuffreecnt < 0) || \
		(nfsbuffreecnt > nfsbufmax) || \
		(nfsbuffreecnt > nfsbufcnt) || \
		(nfsbuffreemetacnt < 0) || \
		(nfsbuffreemetacnt > nfsbufmax) || \
		(nfsbuffreemetacnt > nfsbufcnt) || \
		(nfsbuffreemetacnt > nfsbufmetamax) || \
		(nfsbuffreemetacnt > nfsbufmetacnt) || \
		(nfsbufdelwricnt < 0) || \
		(nfsbufdelwricnt > nfsbufmax) || \
		(nfsbufdelwricnt > nfsbufcnt) || \
		(nfs_nbdwrite < 0) || \
		(nfs_nbdwrite > nfsbufcnt) || \
		0) \
		panic("nfsbuf count error: max %d meta %d cnt %d meta %d free %d meta %d delwr %d bdw %d\n", \
			nfsbufmax, nfsbufmetamax, nfsbufcnt, nfsbufmetacnt, nfsbuffreecnt, nfsbuffreemetacnt, \
			nfsbufdelwricnt, nfs_nbdwrite); \
	} while (0)
#else
#define NFSBUFCNTCHK()
#endif

/*
 * NFS vnode attribute structure
 */
#define NFSTIME_ACCESS	0	/* time of last access */
#define NFSTIME_MODIFY	1	/* time of last modification */
#define NFSTIME_CHANGE	2	/* time file changed */
#define NFSTIME_CREATE	3	/* time file created */
#define NFSTIME_BACKUP	4	/* time of last backup */
#define NFSTIME_COUNT	5

#define NFS_COMPARE_MTIME(TVP, NVAP, CMP) \
	(((TVP)->tv_sec == (NVAP)->nva_timesec[NFSTIME_MODIFY]) ?	\
	 ((TVP)->tv_nsec CMP (NVAP)->nva_timensec[NFSTIME_MODIFY]) :	\
	 ((TVP)->tv_sec CMP (NVAP)->nva_timesec[NFSTIME_MODIFY]))
#define NFS_COPY_TIME(TVP, NVAP, WHICH) \
	do { \
	(TVP)->tv_sec = (NVAP)->nva_timesec[NFSTIME_##WHICH]; \
	(TVP)->tv_nsec = (NVAP)->nva_timensec[NFSTIME_##WHICH]; \
	} while (0)

struct nfs_vattr {
	enum vtype	nva_type;	/* vnode type (for create) */
	uint32_t	nva_mode;	/* files access mode (and type) */
	uid_t		nva_uid;	/* owner user id */
	gid_t		nva_gid;	/* owner group id */
	nfs_specdata	nva_rawdev;	/* device the special file represents */
	uint32_t	nva_flags;	/* file flags */
	uint32_t	nva_maxlink;	/* maximum # of links (v4) */
	uint64_t	nva_nlink;	/* number of references to file */
	uint64_t	nva_fileid;	/* file id */
	nfs_fsid	nva_fsid;	/* file system id */
	uint64_t	nva_size;	/* file size in bytes */
	uint64_t	nva_bytes;	/* bytes of disk space held by file */
	uint64_t	nva_change;	/* change attribute */
	int64_t		nva_timesec[NFSTIME_COUNT];
	int32_t		nva_timensec[NFSTIME_COUNT];
	uint32_t 	nva_bitmap[NFS_ATTR_BITMAP_LEN]; /* attributes that are valid */
};

#define NFS_FFLAG_ARCHIVED	0x0001
#define NFS_FFLAG_HIDDEN	0x0002
#define NFS_FFLAG_NAMED_ATTR	0x0004	/* file has named attributes */

/*
 * macros for detecting node changes
 *
 * These macros help us determine if a file has been changed on the server and
 * thus whether or not we need to invalidate any cached data.
 *
 * For NFSv2/v3, the modification time is used.
 * For NFSv4, the change attribute is used.
 */
#define NFS_CHANGED(VERS, NP, NVAP) \
		(((VERS) >= NFS_VER4) ? \
			((NP)->n_change != (NVAP)->nva_change) : \
			NFS_COMPARE_MTIME(&(NP)->n_mtime, (NVAP), !=))
#define NFS_CHANGED_NC(VERS, NP, NVAP) \
		(((VERS) >= NFS_VER4) ? \
			((NP)->n_ncchange != (NVAP)->nva_change) : \
			NFS_COMPARE_MTIME(&(NP)->n_ncmtime, (NVAP), !=))
#define NFS_CHANGED_UPDATE(VERS, NP, NVAP) \
	do { \
		if ((VERS) >= NFS_VER4) \
			(NP)->n_change = (NVAP)->nva_change; \
		else \
			NFS_COPY_TIME(&(NP)->n_mtime, (NVAP), MODIFY); \
	} while (0)
#define NFS_CHANGED_UPDATE_NC(VERS, NP, NVAP) \
	do { \
		if ((VERS) >= NFS_VER4) \
			(NP)->n_ncchange = (NVAP)->nva_change; \
		else \
			NFS_COPY_TIME(&(NP)->n_ncmtime, (NVAP), MODIFY); \
	} while (0)


__private_extern__ lck_grp_t *nfs_open_grp;
__private_extern__ uint32_t nfs_open_owner_seqnum, nfs_lock_owner_seqnum;

/*
 * NFSv4 open owner structure - one per cred per mount
 */
struct nfs_open_owner {
	TAILQ_ENTRY(nfs_open_owner)	noo_link;	/* List of open owners (per-mount) */
	lck_mtx_t			noo_lock;	/* owner mutex */
	struct nfsmount *		noo_mount;	/* NFS mount */
	uint32_t			noo_refcnt;	/* # outstanding references */
	uint32_t			noo_flags;	/* see below */
	kauth_cred_t			noo_cred;	/* credentials of open owner */
	uint32_t			noo_name;	/* unique name used otw */
	uint32_t			noo_seqid;	/* client-side sequence ID */
};
/* noo_flags */
#define NFS_OPEN_OWNER_LINK	0x1	/* linked into mount's open owner list */
#define NFS_OPEN_OWNER_BUSY	0x2	/* open state-modifying operation in progress */
#define NFS_OPEN_OWNER_WANT	0x4	/* someone else wants to mark busy */

/*
 * NFS open file structure - one per open owner per nfsnode
 */
struct nfs_open_file {
	lck_mtx_t			nof_lock;		/* open file mutex */
	TAILQ_ENTRY(nfs_open_file)	nof_link;		/* list of open files */
	struct nfs_open_owner *		nof_owner;		/* open owner */
	nfs_stateid			nof_stateid;		/* open stateid */
	uint32_t			nof_opencnt;		/* open file count */
	uint8_t				nof_flags;		/* see below */
	uint8_t				nof_access;		/* access mode for this open */
	uint8_t				nof_deny;		/* deny mode for this open */
	uint8_t				nof_mmap_access;	/* mmap access mode */
	uint32_t			nof_rcnt;		/* # read opens */
	uint32_t			nof_wcnt;		/* # write opens */
	uint32_t			nof_dwcnt;		/* # deny write opens */
};
/* nof_flags */
#define NFS_OPEN_FILE_BUSY	0x01	/* open state-modifying operation in progress */
#define NFS_OPEN_FILE_WANT	0x02	/* someone else wants to mark busy */
#define NFS_OPEN_FILE_CREATE	0x04	/* has an open(RW) from a VNOP_CREATE call */
#define NFS_OPEN_FILE_NEEDCLOSE	0x08	/* has an open(R) from an (unopen) VNOP_READ call */
#define NFS_OPEN_FILE_READTOO	0x10	/* open(W) also grabbed R access */
#define NFS_OPEN_FILE_SETATTR	0x20	/* has an open(W) to perform a SETATTR(size) */
#define NFS_OPEN_FILE_POSIXLOCK	0x40	/* server supports POSIX locking semantics */

struct nfs_lock_owner;
/*
 * NFS file lock
 *
 * Each lock request (pending or granted) has an
 * nfs_file_lock structure representing its state.
 */
struct nfs_file_lock {
	TAILQ_ENTRY(nfs_file_lock)	nfl_link;	/* List of locks on nfsnode */
	TAILQ_ENTRY(nfs_file_lock)	nfl_lolink;	/* List of locks held by locker */
	struct nfs_lock_owner *		nfl_owner;	/* lock owner that holds this lock */
	uint64_t			nfl_start;	/* starting offset */
	uint64_t			nfl_end;	/* ending offset (inclusive) */
	uint32_t			nfl_blockcnt;	/* # locks blocked on this lock */
	uint16_t			nfl_flags;	/* see below */
	uint8_t				nfl_type;	/* lock type: read/write */
};
/* nfl_flags */
#define NFS_FILE_LOCK_ALLOC		0x01	/* lock was allocated */
#define NFS_FILE_LOCK_STYLE_POSIX	0x02	/* POSIX-style fcntl() lock */
#define NFS_FILE_LOCK_STYLE_FLOCK	0x04	/* flock(2)-style lock */
#define NFS_FILE_LOCK_STYLE_MASK	0x06	/* lock style mask */
#define NFS_FILE_LOCK_WAIT		0x08	/* may block on conflicting locks */
#define NFS_FILE_LOCK_BLOCKED		0x10	/* request is blocked */
#define NFS_FILE_LOCK_DEAD		0x20	/* lock (request) no longer exists */

TAILQ_HEAD(nfs_file_lock_queue, nfs_file_lock);

/*
 * Calculate length of lock range given the endpoints.
 * Note that struct flock has "to EOF" reported as 0 but
 * the NFSv4 protocol has "to EOF" reported as UINT64_MAX.
 */
#define NFS_FLOCK_LENGTH(S, E)	(((E) == UINT64_MAX) ? 0 : ((E) - (S) + 1))
#define NFS_LOCK_LENGTH(S, E)	(((E) == UINT64_MAX) ? UINT64_MAX : ((E) - (S) + 1))

/*
 * NFSv4 lock owner structure - per open owner per process per nfsnode
 *
 * A lock owner is a process + an nfsnode.
 *
 * Note that flock(2) locks technically should have the lock owner be
 * an fglob pointer instead of a process.  However, implementing that
 * correctly would not be trivial.  So, for now, flock(2) locks are
 * treated like POSIX locks.
 */
struct nfs_lock_owner {
	lck_mtx_t			nlo_lock;	/* owner mutex */
	TAILQ_ENTRY(nfs_lock_owner)	nlo_link;	/* List of lock owners (per-mount) */
	struct nfs_open_owner *		nlo_open_owner;	/* corresponding open owner */
	struct nfs_file_lock_queue	nlo_locks;	/* list of locks held */
	struct nfs_file_lock		nlo_alock;	/* most lockers will only ever have one */
	struct timeval			nlo_pid_start;	/* Start time of process id */
	pid_t				nlo_pid;	/* lock-owning process ID */
	uint32_t			nlo_refcnt;	/* # outstanding references */
	uint32_t			nlo_flags;	/* see below */
	uint32_t			nlo_name;	/* unique name used otw */
	uint32_t			nlo_seqid;	/* client-side sequence ID */
	nfs_stateid			nlo_stateid;	/* lock stateid */
};
/* nlo_flags */
#define NFS_LOCK_OWNER_LINK	0x1	/* linked into mount's lock owner list */
#define NFS_LOCK_OWNER_BUSY	0x2	/* lock state-modifying operation in progress */
#define NFS_LOCK_OWNER_WANT	0x4	/* someone else wants to mark busy */
#define NFS_LOCK_OWNER_NEW	0x8	/* this lock owner isn't known to the server yet */

/*
 * The nfsnode is the NFS equivalent of an inode.
 * There is a unique nfsnode for each NFS vnode.
 * An nfsnode is 'named' by its file handle. (nget/nfs_node.c)
 * NB: Hopefully the current order of the fields is such that everything will
 *     be well aligned and, therefore, tightly packed.
 */

#define NFS_ACCESS_CACHE_SIZE	3

struct nfsnode {
	lck_rw_t		n_lock;		/* nfs node lock */
	void			*n_lockowner;	/* nfs node lock owner (exclusive) */
	lck_rw_t		n_datalock;	/* nfs node data lock */
	void			*n_datalockowner;/* nfs node data lock owner (exclusive) */
	LIST_ENTRY(nfsnode)	n_hash;		/* Hash chain */
	u_quad_t		n_size;		/* Current size of file */
	u_quad_t		n_newsize;	/* new size of file (pending update) */
	u_int64_t		n_xid;		/* last xid to loadattr */
	struct nfs_vattr	n_vattr;	/* Vnode attribute cache */
	time_t			n_attrstamp;	/* Attr. cache timestamp */
	u_int8_t		n_mode[NFS_ACCESS_CACHE_SIZE+1];	/* ACCESS mode cache */
	uid_t                   n_modeuid[NFS_ACCESS_CACHE_SIZE];	/* credentials having mode */
	time_t                  n_modestamp[NFS_ACCESS_CACHE_SIZE];	/* mode cache timestamp */
	union {
	    struct {
		struct timespec	n3_mtime;	/* Prev modify time. */
		struct timespec	n3_ncmtime;	/* namecache modify time. */
	    } v3;
	    struct {
		uint64_t	n4_change;	/* prev change attribute */
		uint64_t	n4_ncchange;	/* namecache change attribute */
	    } v4;
	} n_un4;
	vnode_t			n_parent;	/* this node's parent */
	u_char			*n_fhp;		/* NFS File Handle */
	vnode_t			n_vnode;	/* associated vnode */
	mount_t			n_mount;	/* associated mount (NHINIT) */
	int			n_error;	/* Save write error value */
	union {
		struct timespec	nf_atim;	/* Special file times */
		daddr64_t	nf_lastread;	/* last block# read from (for readahead) */
		nfsuint64	nd_cookieverf;	/* Cookie verifier (dir only) */
	} n_un1;
	union {
		struct timespec	nf_mtim;	/* Special file times */
		daddr64_t	nf_lastrahead;	/* last block# read ahead */
		off_t		nd_direof;	/* Dir. EOF offset cache */
	} n_un2;
	union {
		struct nfs_sillyrename *nf_silly;/* Ptr to silly rename struct */
		LIST_HEAD(, nfsdmap) nd_cook;	/* cookies */
	} n_un3;
	u_short			n_fhsize;	/* size in bytes, of fh */
	u_short			n_flag;		/* node flags */
	u_short			n_hflag;	/* node hash flags */
	u_short			n_bflag;	/* node buffer flags */
	u_char			n_fh[NFS_SMALLFH];/* Small File Handle */
	struct nfsbuflists	n_cleanblkhd;	/* clean blocklist head */
	struct nfsbuflists	n_dirtyblkhd;	/* dirty blocklist head */
	int			n_wrbusy;	/* # threads in write/fsync */
	int			n_needcommitcnt;/* # bufs that need committing */
	int			n_bufiterflags;	/* buf iterator flags */
	/* open state */
	lck_mtx_t		n_openlock;	/* nfs node open lock */
	uint32_t		n_openflags;	/* open state flags */
	uint32_t		n_openrefcnt;	/* # non-file opens */
	TAILQ_HEAD(,nfs_open_file) n_opens;	/* list of open files */
	/* lock state */
	TAILQ_HEAD(, nfs_lock_owner) n_lock_owners; /* list of lock owners */
	struct nfs_file_lock_queue n_locks;	/* list of locks */
};

#define NFS_NODE_LOCK_SHARED	1
#define NFS_NODE_LOCK_EXCLUSIVE	2
#define NFS_NODE_LOCK_FORCE	3

#define nfstimespeccmp(tvp, uvp, cmp)		\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?	\
	 ((tvp)->tv_nsec cmp (uvp)->tv_nsec) :	\
	 ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define CHECK_NEEDCOMMITCNT(np) \
	do { \
		if ((np)->n_needcommitcnt < 0) { \
			printf("nfs: n_needcommitcnt negative\n"); \
			(np)->n_needcommitcnt = 0; \
		} \
	} while (0)

#define n_atim			n_un1.nf_atim
#define n_mtim			n_un2.nf_mtim
#define n_lastread		n_un1.nf_lastread
#define n_lastrahead		n_un2.nf_lastrahead
#define n_sillyrename		n_un3.nf_silly
#define n_cookieverf		n_un1.nd_cookieverf
#define n_direofoffset		n_un2.nd_direof
#define n_cookies		n_un3.nd_cook
#define n_mtime			n_un4.v3.n3_mtime
#define n_ncmtime		n_un4.v3.n3_ncmtime
#define n_change		n_un4.v4.n4_change
#define n_ncchange		n_un4.v4.n4_ncchange

/*
 * Flags for n_flag
 */
#define	NUPDATESIZE	0x0001	/* size of file needs updating */
#define	NMODIFIED	0x0004	/* Might have a modified buffer in bio */
#define	NWRITEERR	0x0008	/* Flag write errors so close will know */
#define	NNEEDINVALIDATE	0x0010	/* need to call vinvalbuf() */
#define	NACC		0x0100	/* Special file accessed */
#define	NUPD		0x0200	/* Special file updated */
#define	NCHG		0x0400	/* Special file times changed */
#define	NNEGNCENTRIES	0x0800	/* directory has negative name cache entries */

/*
 * Flags for n_hflag
 * Note: protected by nfs_node_hash_mutex
 */
#define NHHASHED	0x0001  /* node is in hash table */
#define NHINIT		0x0002  /* node is being initialized */
#define NHLOCKED	0x0004  /* node is locked (initting or deleting) */
#define NHLOCKWANT	0x0008  /* someone wants to lock */

/*
 * Flags for n_bflag
 * Note: protected by nfs_buf_mutex
 */
#define	NBFLUSHINPROG	0x0001	/* Avoid multiple calls to nfs_flush() */
#define	NBFLUSHWANT	0x0002	/* waiting for nfs_flush() to complete */
#define	NBINVALINPROG	0x0004	/* Avoid multiple calls to nfs_vinvalbuf() */
#define	NBINVALWANT	0x0008	/* waiting for nfs_vinvalbuf() to complete */

/*
 * n_openflags
 * Note: protected by n_openlock
 */
#define N_OPENBUSY		0x0001	/* open state is busy - being updated */
#define N_OPENWANT		0x0002	/* someone wants to mark busy */

/* attr/mode timestamp macros */
#define NATTRVALID(np)		((np)->n_attrstamp != ~0)
#define NATTRINVALIDATE(np)	((np)->n_attrstamp = ~0)
#define NMODEVALID(np, slot)	(((slot) >= 0) && ((slot) < 3) && ((np)->n_modestamp[(slot)] != ~0))
#define NMODEINVALIDATE(np) \
	do { \
		(np)->n_modestamp[0] = ~0; \
		(np)->n_modestamp[1] = ~0; \
		(np)->n_modestamp[2] = ~0; \
		(np)->n_mode[3] = 0; \
	} while (0)

/*
 * NFS-specific flags for nfs_vinvalbuf/nfs_flush
 */
#define V_IGNORE_WRITEERR	0x8000

/*
 * Flags for nfs_nget()
 */
#define	NG_MARKROOT	0x0001	/* mark vnode as root of FS */
#define	NG_MAKEENTRY	0x0002	/* add name cache entry for vnode */

/*
 * Convert between nfsnode pointers and vnode pointers
 */
#define VTONFS(vp)	((nfsnode_t)vnode_fsnode(vp))
#define NFSTOV(np)	((np)->n_vnode)

/* nfsnode hash table mutex */
__private_extern__ lck_mtx_t *nfs_node_hash_mutex;

/*
 * nfsiod structures
 */
struct nfsiod {
	TAILQ_ENTRY(nfsiod)	niod_link;	/* List of nfsiods */
	struct nfsmount *	niod_nmp;	/* mount point for this nfsiod */
};
TAILQ_HEAD(nfsiodlist, nfsiod);
TAILQ_HEAD(nfsiodmountlist, nfsmount);
__private_extern__ struct nfsiodlist nfsiodfree, nfsiodwork;
__private_extern__ struct nfsiodmountlist nfsiodmounts;
__private_extern__ lck_mtx_t *nfsiod_mutex;


#endif /* __APPLE_API_PRIVATE */
#endif /* _NFS_NFSNODE_H_ */

/*
 * Copyright (c) 2003-2005 The Regents of The University of Michigan
 * Copyright (c) 2007-2008 The Florida State University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Stephen Hines
 */

#ifndef __ARCH_ARM_LINUX_LINUX_HH__
#define __ARCH_ARM_LINUX_LINUX_HH__

#include "kern/linux/linux.hh"

class ArmLinux : public Linux
{
  public:

    /// This table maps the target open() flags to the corresponding
    /// host open() flags.
    static OpenFlagTransTable openFlagTable[];

    /// Number of entries in openFlagTable[].
    static const int NUM_OPEN_FLAGS;

    //@{
    /// open(2) flag values.
    static const int TGT_O_RDONLY    = 00000000; //!< O_RDONLY
    static const int TGT_O_WRONLY    = 00000001; //!< O_WRONLY
    static const int TGT_O_RDWR      = 00000002; //!< O_RDWR
    static const int TGT_O_CREAT     = 00000100; //!< O_CREAT
    static const int TGT_O_EXCL      = 00000200; //!< O_EXCL
    static const int TGT_O_NOCTTY    = 00000400; //!< O_NOCTTY
    static const int TGT_O_TRUNC     = 00001000; //!< O_TRUNC
    static const int TGT_O_APPEND    = 00002000; //!< O_APPEND
    static const int TGT_O_NONBLOCK  = 00004000; //!< O_NONBLOCK
    static const int TGT_O_SYNC      = 00010000; //!< O_SYNC
    static const int TGT_FASYNC      = 00020000; //!< FASYNC
    static const int TGT_O_DIRECTORY = 00040000; //!< O_DIRECTORY
    static const int TGT_O_NOFOLLOW  = 00100000; //!< O_NOFOLLOW
    static const int TGT_O_DIRECT    = 00200000; //!< O_DIRECT
    static const int TGT_O_LARGEFILE = 00400000; //!< O_LARGEFILE
    static const int TGT_O_NOATIME   = 01000000; //!< O_NOATIME
    //@}

    /// For mmap().
    static const unsigned TGT_MAP_ANONYMOUS = 0x20;

    //@{
    /// For getrusage().
    static const int TGT_RUSAGE_SELF = 0;
    static const int TGT_RUSAGE_CHILDREN = -1;
    static const int TGT_RUSAGE_BOTH = -2;
    //@}

    //@{
    /// ioctl() command codes.
    static const unsigned TIOCGETP_   = 0x5401;
    static const unsigned TIOCSETP_   = 0x80067409;
    static const unsigned TIOCSETN_   = 0x8006740a;
    static const unsigned TIOCSETC_   = 0x80067411;
    static const unsigned TIOCGETC_   = 0x40067412;
    static const unsigned FIONREAD_   = 0x4004667f;
    static const unsigned TIOCISATTY_ = 0x2000745e;
    static const unsigned TIOCGETS_   = 0x402c7413;
    static const unsigned TIOCGETA_   = 0x40127417;
    static const unsigned TCSETAW_    = 0x5407;     // 2.6.15 kernel
    //@}

    /// For table().
    static const int TBL_SYSINFO = 12;

    /// Resource enumeration for getrlimit().
    enum rlimit_resources {
        TGT_RLIMIT_CPU = 0,
        TGT_RLIMIT_FSIZE = 1,
        TGT_RLIMIT_DATA = 2,
        TGT_RLIMIT_STACK = 3,
        TGT_RLIMIT_CORE = 4,
        TGT_RLIMIT_RSS = 5,
        TGT_RLIMIT_NPROC = 6,
        TGT_RLIMIT_NOFILE = 7,
        TGT_RLIMIT_MEMLOCK = 8,
        TGT_RLIMIT_AS = 9,
        TGT_RLIMIT_LOCKS = 10
    };

    typedef struct {
        uint32_t st_dev;
        uint32_t st_ino;
        uint16_t st_mode;
        uint16_t st_nlink;
        uint16_t st_uid;
        uint16_t st_gid;
        uint32_t st_rdev;
        uint32_t st_size;
        uint32_t st_blksize;
        uint32_t st_blocks;
        uint32_t st_atimeX;
        uint32_t st_atime_nsec;
        uint32_t st_mtimeX;
        uint32_t st_mtime_nsec;
        uint32_t st_ctimeX;
        uint32_t st_ctime_nsec;
    } tgt_stat;

    typedef struct {
        uint64_t  st_dev;
        uint8_t   __pad0[4];
        uint32_t  __st_ino;
        uint32_t  st_mode;
        uint32_t  st_nlink;
        uint32_t  st_uid;
        uint32_t  st_gid;
        uint64_t  st_rdev;
        uint8_t   __pad3[4];
        int64_t   __attribute__ ((aligned (8))) st_size;
        uint32_t  st_blksize;
        uint64_t  __attribute__ ((aligned (8))) st_blocks;
        uint32_t  st_atimeX;
        uint32_t  st_atime_nsec;
        uint32_t  st_mtimeX;
        uint32_t  st_mtime_nsec;
        uint32_t  st_ctimeX;
        uint32_t  st_ctime_nsec;
        uint64_t  st_ino;
    } tgt_stat64;

    typedef struct {
        int32_t  uptime;    /* Seconds since boot */
        uint32_t loads[3];  /* 1, 5, and 15 minute load averages */
        uint32_t totalram;  /* Total usable main memory size */
        uint32_t freeram;   /* Available memory size */
        uint32_t sharedram; /* Amount of shared memory */
        uint32_t bufferram; /* Memory used by buffers */
        uint32_t totalswap; /* Total swap space size */
        uint32_t freeswap;  /* swap space still available */
        uint16_t procs;     /* Number of current processes */
        uint32_t totalhigh; /* Total high memory size */
        uint32_t freehigh;  /* Available high memory size */
        uint32_t mem_unit;  /* Memory unit size in bytes */
    } tgt_sysinfo;
   

};

#endif

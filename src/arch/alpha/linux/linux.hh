/*
 * Copyright (c) 2003-2005 The Regents of The University of Michigan
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
 * Authors: Korey Sewell
 */

#ifndef __ALPHA_ALPHA_LINUX_LINUX_HH__
#define __ALPHA_ALPHA_LINUX_LINUX_HH__

#include "kern/linux/linux.hh"

/* AlphaLinux class contains static constants/definitions/misc.
 * structures which are specific to the Linux OS AND the Alpha
 * architecture
 */
class AlphaLinux : public Linux
{
  public:

    /// This table maps the target open() flags to the corresponding
    /// host open() flags.
    static OpenFlagTransTable openFlagTable[];

    /// Number of entries in openFlagTable[].
    static const int NUM_OPEN_FLAGS;

    //@{
    /// open(2) flag values.
    static const int TGT_O_RDONLY       = 00000000;     //!< O_RDONLY
    static const int TGT_O_WRONLY       = 00000001;     //!< O_WRONLY
    static const int TGT_O_RDWR         = 00000002;     //!< O_RDWR
    static const int TGT_O_NONBLOCK     = 00000004;     //!< O_NONBLOCK
    static const int TGT_O_APPEND       = 00000010;     //!< O_APPEND
    static const int TGT_O_CREAT        = 00001000;     //!< O_CREAT
    static const int TGT_O_TRUNC        = 00002000;     //!< O_TRUNC
    static const int TGT_O_EXCL         = 00004000;     //!< O_EXCL
    static const int TGT_O_NOCTTY       = 00010000;     //!< O_NOCTTY
    static const int TGT_O_SYNC         = 00040000;     //!< O_SYNC
    static const int TGT_O_DRD          = 00100000;     //!< O_DRD
    static const int TGT_O_DIRECTIO     = 00200000;     //!< O_DIRECTIO
    static const int TGT_O_CACHE        = 00400000;     //!< O_CACHE
    static const int TGT_O_DSYNC        = 02000000;     //!< O_DSYNC
    static const int TGT_O_RSYNC        = 04000000;     //!< O_RSYNC
    //@}

    /// For mmap().
    static const unsigned TGT_MAP_ANONYMOUS = 0x10;

    //@{
    /// For getsysinfo().
    static const unsigned GSI_PLATFORM_NAME = 103; //!< platform name as string
    static const unsigned GSI_CPU_INFO      = 59; //!< CPU information
    static const unsigned GSI_PROC_TYPE     = 60; //!< get proc_type
    static const unsigned GSI_MAX_CPU       = 30; //!< max # CPUs on machine
    static const unsigned GSI_CPUS_IN_BOX   = 55; //!< number of CPUs in system
    static const unsigned GSI_PHYSMEM       = 19; //!< Physical memory in KB
    static const unsigned GSI_CLK_TCK       = 42; //!< clock freq in Hz
    static const unsigned GSI_IEEE_FP_CONTROL = 45;
    //@}

    //@{
    /// For getrusage().
    static const int TGT_RUSAGE_SELF     = 0;
    static const int TGT_RUSAGE_CHILDREN = -1;
    static const int TGT_RUSAGE_BOTH     = -2;
    //@}

    //@{
    /// For setsysinfo().
    static const unsigned SSI_IEEE_FP_CONTROL = 14; //!< ieee_set_fp_control()
    //@}

    //@{
    /// ioctl() command codes.
    static const unsigned TIOCGETP_   = 0x40067408;
    static const unsigned TIOCSETP_   = 0x80067409;
    static const unsigned TIOCSETN_   = 0x8006740a;
    static const unsigned TIOCSETC_   = 0x80067411;
    static const unsigned TIOCGETC_   = 0x40067412;
    static const unsigned FIONREAD_   = 0x4004667f;
    static const unsigned TIOCISATTY_ = 0x2000745e;
    static const unsigned TIOCGETS_   = 0x402c7413;
    static const unsigned TIOCGETA_   = 0x40127417;
    static const unsigned TCSETAW_    = 0x80147419; // 2.6.15 kernel
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
        TGT_RLIMIT_NOFILE = 6,
        TGT_RLIMIT_AS = 7,
        TGT_RLIMIT_VMEM = 7,
        TGT_RLIMIT_NPROC = 8,
        TGT_RLIMIT_MEMLOCK = 9,
        TGT_RLIMIT_LOCKS = 10
    };
   
    typedef struct {
       int64_t  uptime;    /* Seconds since boot */
       uint64_t loads[3];  /* 1, 5, and 15 minute load averages */
       uint64_t totalram;  /* Total usable main memory size */
       uint64_t freeram;   /* Available memory size */
       uint64_t sharedram; /* Amount of shared memory */
       uint64_t bufferram; /* Memory used by buffers */
       uint64_t totalswap; /* Total swap space size */
       uint64_t freeswap;  /* swap space still available */
       uint16_t procs;     /* Number of current processes */
       uint64_t totalhigh; /* Total high memory size */
       uint64_t freehigh;  /* Available high memory size */
       uint64_t mem_unit;  /* Memory unit size in bytes */
    } tgt_sysinfo;
};

#endif // __ALPHA_ALPHA_LINUX_LINUX_HH__

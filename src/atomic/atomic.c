/*
 *
 * Copyright (c) 2011 - 2015
 *   University of Houston System and UT-Battelle, LLC.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * o Neither the name of the University of Houston System, Oak Ridge
 *   National Laboratory nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */



#include <sys/types.h>

#include "state.h"
#include "utils.h"
#include "atomic.h"

#include "shmem.h"

#include "comms/comms.h"


#ifdef HAVE_FEATURE_PSHMEM
#include "pshmem.h"
#endif /* HAVE_FEATURE_PSHMEM */


/*
 * placeholders: no init/final required (so far)
 */

void
shmemi_atomic_init (void)
{
}

void
shmemi_atomic_finalize (void)
{
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_swap = pshmem_int_swap
#define shmem_int_swap pshmem_int_swap
#pragma weak shmem_long_swap = pshmem_long_swap
#define shmem_long_swap pshmem_long_swap
#pragma weak shmem_longlong_swap = pshmem_longlong_swap
#define shmem_longlong_swap pshmem_longlong_swap
#pragma weak shmem_float_swap = pshmem_float_swap
#define shmem_float_swap pshmem_float_swap
#pragma weak shmem_double_swap = pshmem_double_swap
#define shmem_double_swap pshmem_double_swap
#pragma weak shmem_swap = pshmem_swap
#define shmem_swap pshmem_swap
#endif /* HAVE_FEATURE_PSHMEM */

/**
 * shmem_swap performs an atomic swap operation. It writes value
 * "value" into target on processing element (PE) pe and returns the
 * previous contents of target as an atomic operation.
 */

#define SHMEM_TYPE_SWAP(Name, Type, Size)                               \
    Type                                                                \
    shmem_##Name##_swap (Type *target, Type value, int pe)              \
    {                                                                   \
        Type retval;                                                    \
        INIT_CHECK ();                                                  \
        PE_RANGE_CHECK (pe, 3);                                         \
        shmemi_comms_swap_request##Size (target, &value, sizeof (Type), \
                                         pe, &retval);                  \
        return retval;                                                  \
    }

/* SHMEM_TYPE_SWAP(short, short, 32) !! */
SHMEM_TYPE_SWAP (int, int, 32);
SHMEM_TYPE_SWAP (long, long, 64);
SHMEM_TYPE_SWAP (longlong, long long, 64);
SHMEM_TYPE_SWAP (double, double, 64);
SHMEM_TYPE_SWAP (float, float, 32);

/**
 * untyped variant
 */
long
shmem_swap (long *target, long value, int pe)
{
    return shmem_long_swap (target, value, pe);
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_cswap = pshmem_int_cswap
#define shmem_int_cswap pshmem_int_cswap
#pragma weak shmem_long_cswap = pshmem_long_cswap
#define shmem_long_cswap pshmem_long_cswap
#pragma weak shmem_longlong_cswap = pshmem_longlong_cswap
#define shmem_longlong_cswap pshmem_longlong_cswap
#endif /* HAVE_FEATURE_PSHMEM */

/**
 * The conditional swap routines conditionally update a target data
 * object on an arbitrary processing element (PE) and return the prior
 * contents of the data object in one atomic operation.
 */

#define SHMEM_TYPE_CSWAP(Name, Type, Size)                              \
    Type                                                                \
    shmem_##Name##_cswap (Type *target, Type cond, Type value, int pe)  \
    {                                                                   \
        Type retval;                                                    \
        INIT_CHECK ();                                                  \
        PE_RANGE_CHECK (pe, 4);                                         \
        shmemi_comms_cswap_request##Size (target, &cond, &value,        \
                                          sizeof (Type),                \
                                          pe, &retval);                 \
        return retval;                                                  \
    }

SHMEM_TYPE_CSWAP (int, int, 32);
SHMEM_TYPE_CSWAP (long, long, 64);
SHMEM_TYPE_CSWAP (longlong, long long, 64);

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_fadd = pshmem_int_fadd
#define shmem_int_fadd pshmem_int_fadd
#pragma weak shmem_long_fadd = pshmem_long_fadd
#define shmem_long_fadd pshmem_long_fadd
#pragma weak shmem_longlong_fadd = pshmem_longlong_fadd
#define shmem_longlong_fadd pshmem_longlong_fadd
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_TYPE_FADD(Name, Type, Size)                               \
    Type                                                                \
    shmem_##Name##_fadd (Type *target, Type value, int pe)              \
    {                                                                   \
        Type retval;                                                    \
        INIT_CHECK ();                                                  \
        PE_RANGE_CHECK (pe, 3);                                         \
        shmemi_comms_fadd_request##Size (target, &value, sizeof (Type), \
                                         pe, &retval);                  \
        return retval;                                                  \
    }

SHMEM_TYPE_FADD (int, int, 32);
SHMEM_TYPE_FADD (long, long, 64);
SHMEM_TYPE_FADD (longlong, long long, 64);


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_finc = pshmem_int_finc
#define shmem_int_finc pshmem_int_finc
#pragma weak shmem_long_finc = pshmem_long_finc
#define shmem_long_finc pshmem_long_finc
#pragma weak shmem_longlong_finc = pshmem_longlong_finc
#define shmem_longlong_finc pshmem_longlong_finc
#endif /* HAVE_FEATURE_PSHMEM */

/**
 * finc performs an atomic fetch-and-increment at an address
 * on another PE
 */

#define SHMEM_TYPE_FINC(Name, Type, Size)                       \
    Type                                                        \
    shmem_##Name##_finc (Type *target, int pe)                  \
    {                                                           \
        Type retval;                                            \
        INIT_CHECK ();                                          \
        PE_RANGE_CHECK (pe, 2);                                 \
        shmemi_comms_finc_request##Size (target, sizeof (Type), \
                                         pe, &retval);          \
        return retval;                                          \
    }

SHMEM_TYPE_FINC (int, int, 32);
SHMEM_TYPE_FINC (long, long, 64);
SHMEM_TYPE_FINC (longlong, long long, 64);

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_add = pshmem_int_add
#define shmem_int_add pshmem_int_add
#pragma weak shmem_long_add = pshmem_long_add
#define shmem_long_add pshmem_long_add
#pragma weak shmem_longlong_add = pshmem_longlong_add
#define shmem_longlong_add pshmem_longlong_add
#endif /* HAVE_FEATURE_PSHMEM */

/**
 * remote atomic increment/add
 *
 */
#define SHMEM_TYPE_ADD(Name, Type, Size)                                \
    void                                                                \
    shmem_##Name##_add (Type *target, Type value, int pe)               \
    {                                                                   \
        INIT_CHECK ();                                                  \
        PE_RANGE_CHECK (pe, 3);                                         \
        shmemi_comms_add_request##Size (target, &value, sizeof (Type),  \
                                        pe);                            \
    }

SHMEM_TYPE_ADD (int, int, 32);
SHMEM_TYPE_ADD (long, long, 64);
SHMEM_TYPE_ADD (longlong, long long, 64);


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int_inc = pshmem_int_inc
#define shmem_int_inc pshmem_int_inc
#pragma weak shmem_long_inc = pshmem_long_inc
#define shmem_long_inc pshmem_long_inc
#pragma weak shmem_longlong_inc = pshmem_longlong_inc
#define shmem_longlong_inc pshmem_longlong_inc
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_TYPE_INC(Name, Type, Size)                        \
    void                                                        \
    shmem_##Name##_inc (Type *target, int pe)                   \
    {                                                           \
        INIT_CHECK ();                                          \
        PE_RANGE_CHECK (pe, 2);                                 \
        shmemi_comms_inc_request##Size (target, sizeof (Type),  \
                                        pe);                    \
    }

SHMEM_TYPE_INC (int, int, 32);
SHMEM_TYPE_INC (long, long, 64);
SHMEM_TYPE_INC (longlong, long long, 64);

/* --------------------------------------------------------------- */

#if defined(HAVE_FEATURE_EXPERIMENTAL)

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_int_xor = pshmemx_int_xor
#define shmemx_int_xor pshmemx_int_xor
#pragma weak shmemx_long_xor = pshmemx_long_xor
#define shmemx_long_xor pshmemx_long_xor
#pragma weak shmemx_longlong_xor = pshmemx_longlong_xor
#define shmemx_longlong_xor pshmemx_longlong_xor
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEMX_TYPE_XOR(Name, Type, Size)                               \
    void                                                                \
    shmemx_##Name##_xor(Type *target, Type value, int pe)               \
    {                                                                   \
        INIT_CHECK ();                                                  \
        PE_RANGE_CHECK (pe, 3);                                         \
        shmemi_comms_xor_request##Size (target, &value, sizeof (Type),  \
                                        pe);                            \
    }

SHMEMX_TYPE_XOR (int, int, 32);
SHMEMX_TYPE_XOR (long, long, 64);
SHMEMX_TYPE_XOR (longlong, long long, 64);

#endif /* HAVE_FEATURE_EXPERIMENTAL */

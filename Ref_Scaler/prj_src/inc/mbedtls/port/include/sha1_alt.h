/**
 * \file sha1.h
 *
 * \brief SHA-1 cryptographic hash function
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#include <stddef.h>
#include <stdint.h>

#if defined(MBEDTLS_SHA1_ALT)

#ifdef __cplusplus
extern "C" {
#endif

#include "scrt_patch.h"

#define SHA1_BLOCK_SIZE       (SCRT_SHA_1_STEP_SIZE)

/**
 * \brief          SHA-1 context structure
 */
typedef struct
{
    uint32_t total[2];                      /*!< number of bytes processed  */
    uint32_t state[5];                      /*!< intermediate digest state  */
    unsigned char buffer[SHA1_BLOCK_SIZE];  /*!< data block being processed */
    uint8_t u8HasInterMac;
}
mbedtls_sha1_context;

/**
 * \brief          Initialize SHA-1 context
 *
 * \param ctx      SHA-1 context to be initialized
 */
void mbedtls_sha1_init( mbedtls_sha1_context *ctx );

/**
 * \brief          Clear SHA-1 context
 *
 * \param ctx      SHA-1 context to be cleared
 */
void mbedtls_sha1_free( mbedtls_sha1_context *ctx );

/**
 * \brief          Clone (the state of) a SHA-1 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src );

/**
 * \brief          SHA-1 context setup
 *
 * \param ctx      context to be initialized
 */
void mbedtls_sha1_starts( mbedtls_sha1_context *ctx );

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 */
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] );

/* Internal use */
void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] );

#ifdef __cplusplus
}
#endif


#endif /* MBEDTLS_SHA1_ALT */

#endif /* mbedtls_sha1.h */


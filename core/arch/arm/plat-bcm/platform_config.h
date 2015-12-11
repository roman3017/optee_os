/*
 * Copyright (c) 2014, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#define PLATFORM_FLAVOR_ID_rpi	        0
#define PLATFORM_FLAVOR_ID_cygnus	1

#define PLATFORM_FLAVOR_IS(flav) \
	(PLATFORM_FLAVOR == PLATFORM_FLAVOR_ID_ ## flav)

/* Make stacks aligned to data cache line length */
#define STACK_ALIGNMENT		32

#ifdef CFG_WITH_PAGER
#error "Pager not supported for platform BCM"
#endif
#ifdef CFG_WITH_LPAE
#error "LPAE not supported for platform BCM"
#endif

#define HEAP_SIZE		(24 * 1024)

/*
 * TEE/TZ RAM layout:
 *
 *  +---------------------------------------+ 0xDBC00000
 *  | TEETZ private RAM  |  TEE_RAM         |
 *  |                    +------------------+ 0xDBD00000
 *  |                    |  TA_RAM          |
 *  +---------------------------------------+ 0xDBF00000
 *  |                    |    teecore alloc |
 *  |  TEE/TZ and NSec   |  PUB_RAM   ------|
 *  |   shared memory    |       NSec alloc |
 *  +---------------------------------------+ 0xDC000000
 *
 *  TEE_RAM : 1MByte
 *  PUB_RAM : 1MByte
 *  TA_RAM  : all what is left (at least 2MByte !)
 */

#if PLATFORM_FLAVOR_IS(rpi)

#define CFG_TEE_CORE_NB_CORE	1
#define DRAM0_BASE		0xC0000000
#define DRAM0_SIZE		0x1C000000
/* PL011 UART */
#define UART0_BASE	        0x20201000
#define CONSOLE_UART_BASE	UART0_BASE
#define CONSOLE_UART_CLK_IN_HZ	3000000
#define CONSOLE_BAUDRATE	115200

#elif PLATFORM_FLAVOR_IS(cygnus)

#define CFG_TEE_CORE_NB_CORE	1
#define DRAM0_BASE		0x60000000
#define DRAM0_SIZE		0x20000000
/* 8250 UART */
#define UART3_BASE	        0x18023000
#define CONSOLE_UART_BASE	UART3_BASE
#define CONSOLE_UART_CLK_IN_HZ	100000000
#define CONSOLE_BAUDRATE	115200

#else

#error "Unknown platform flavor"

#endif

/* define the several memory area sizes */
#if (CFG_DDR_TEETZ_RESERVED_SIZE < 0x400000)
#error "Invalid CFG_DDR_TEETZ_RESERVED_SIZE: at least 4MB expected"
#endif

#define CFG_SHMEM_SIZE		0x100000
#define CFG_TEE_RAM_PH_SIZE	0x100000

#define CFG_TEE_RAM_VA_SIZE	0x100000

#define CFG_TA_RAM_SIZE		(CFG_DDR_TEETZ_RESERVED_SIZE - \
				 CFG_TEE_RAM_PH_SIZE - CFG_SHMEM_SIZE)

/* define the secure memory area */
#define TZDRAM_BASE		(CFG_DDR_TEETZ_RESERVED_START)
#define TZDRAM_SIZE		(CFG_TEE_RAM_PH_SIZE + CFG_TA_RAM_SIZE)

/* define the memory areas (TEE_RAM must start at reserved DDR start addr */
#define CFG_TEE_RAM_START	(TZDRAM_BASE)
#define CFG_TA_RAM_START	(CFG_TEE_RAM_START + CFG_TEE_RAM_PH_SIZE)
#define CFG_SHMEM_START		(CFG_TA_RAM_START + CFG_TA_RAM_SIZE)

#ifndef CFG_TEE_LOAD_ADDR
#define CFG_TEE_LOAD_ADDR	CFG_TEE_RAM_START
#endif

#define DEVICE0_BASE		ROUNDDOWN(CONSOLE_UART_BASE, \
					  CORE_MMU_DEVICE_SIZE)
#define DEVICE0_SIZE		CORE_MMU_DEVICE_SIZE
#define DEVICE0_TYPE		MEM_AREA_IO_NSEC

#endif /*PLATFORM_CONFIG_H*/

/*
 * Copyright (C) 2015 Freescale Semiconductor, Inc.
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

#define PLATFORM_FLAVOR_ID_rpi	0
#define PLATFORM_FLAVOR_IS(flav) \
	(PLATFORM_FLAVOR_ID_ ## flav == PLATFORM_FLAVOR)

#define STACK_ALIGNMENT			64

#if  PLATFORM_FLAVOR_IS(rpi)
#define UART0_BASE			0x20201000

#define DRAM0_SIZE			0x1C000000
#define DRAM0_BASE			0x00000000

#define CFG_TEE_CORE_NB_CORE		1

#define DDR_SIZE			DRAM0_SIZE
#define DDR_PHYS_START			DRAM0_BASE

#define CFG_DDR_SIZE			DDR_SIZE
#define CFG_DDR_START			DDR_PHYS_START

#define CFG_SHMEM_SIZE			0x100000
#define CFG_SHMEM_START			(TZDRAM_BASE - CFG_SHMEM_SIZE)

#else
#error "Unknown platform flavor"
#endif

#define HEAP_SIZE			(24 * 1024)

/* console uart define */
#define CONSOLE_UART_BASE		UART0_BASE
#define CONSOLE_UART_CLK_IN_HZ	        3000000
#define CONSOLE_BAUDRATE	        115200

#define TZDRAM_SIZE			(0x00300000)
#define TZDRAM_BASE			(0x1BD00000)


#ifndef CFG_TEE_LOAD_ADDR
#define CFG_TEE_LOAD_ADDR		0x1BD00000
//#define CFG_TEE_LOAD_ADDR		0x00008000
#endif

/*
 * Everything is in TZDRAM.
 * +------------------+ dc00
 * |        | TA_RAM  | 2M
 * + TZDRAM +---------+ dbe0
 * |        | TEE_RAM | 1M
 * +--------+---------+ dbd0
 * | SHMEM            | 1M
 * +--------+---------+ dbc0
 * | LINUX            | 444M
 * +--------+---------+ c000
 */

#define CFG_TEE_RAM_VA_SIZE	(0x00100000)
#define CFG_TEE_RAM_PH_SIZE	CFG_TEE_RAM_VA_SIZE
#define CFG_TEE_RAM_START	TZDRAM_BASE
#define CFG_TA_RAM_SIZE		ROUNDDOWN((TZDRAM_SIZE - CFG_TEE_RAM_VA_SIZE), \
					  CORE_MMU_DEVICE_SIZE)
#define CFG_TA_RAM_START	ROUNDUP((TZDRAM_BASE + CFG_TEE_RAM_VA_SIZE), \
					CORE_MMU_DEVICE_SIZE)

#define DEVICE0_BASE		ROUNDDOWN(CONSOLE_UART_BASE, \
					  CORE_MMU_DEVICE_SIZE)
#define DEVICE0_SIZE		CORE_MMU_DEVICE_SIZE
#define DEVICE0_TYPE		MEM_AREA_IO_NSEC

#endif /*PLATFORM_CONFIG_H*/

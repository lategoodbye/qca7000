/*
 *   Copyright (c) 2011, 2012, Qualcomm Atheros Communications Inc.
 *   Copyright (c) 2014, I2SE GmbH
 *
 *   Permission to use, copy, modify, and/or distribute this software
 *   for any purpose with or without fee is hereby granted, provided
 *   that the above copyright notice and this permission notice appear
 *   in all copies.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*   Qualcomm Atheros SPI register definition.
 *
 *   This module is designed to define the Qualcomm Atheros SPI register
 *   placeholders;
 */

#ifndef QCA_SPI_HEADER
#define QCA_SPI_HEADER

#include <linux/spi/spi.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "qca_framing.h"

#define QCASPI_GOOD_SIGNATURE 0xAA55

#define TX_QUEUE_LEN 10

/* sync related constants */
#define QCASPI_SYNC_UNKNOWN 0
#define QCASPI_SYNC_RESET   1
#define QCASPI_SYNC_READY   2
#define QCASPI_RESET_TIMEOUT 10

/* sync events */
#define QCASPI_SYNC_UPDATE 0
#define QCASPI_SYNC_CPUON  1

struct txq {
	struct sk_buff *skb[TX_QUEUE_LEN];
	u32 head;
	u32 tail;
};

struct qcaspi {
	struct net_device *net_dev;
	struct spi_device *spi_dev;
	struct task_struct *spi_thread;

	struct txq txq;

	u8 *rx_buffer;
	u32 buffer_size;
	u8 sync;

	struct qcafrm_handle frm_handle;
	struct sk_buff *rx_skb;

	int irq;
	volatile unsigned int intr_req;
	volatile unsigned int intr_svc;

#ifdef CONFIG_DEBUG_FS
	struct dentry *device_root;
#endif

	/* user configurable options */
	u32 clkspeed;
	u8 legacy_mode;
	u16 burst_len;
};

#endif

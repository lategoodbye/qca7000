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
 *
 */

/*   Qualcomm Atheros SPI register definition.
 *
 *   This module is designed to define the Qualcomm Atheros SPI
 *   register placeholders.
 */

#ifndef QCA7K_HEADER
#define QCA7K_HEADER

#include <linux/types.h>

#include "qca_spi.h"

#define QCA7K_SPI_READ (1 << 15)
#define QCA7K_SPI_WRITE (0 << 15)
#define QCA7K_SPI_INTERNAL (1 << 14)
#define QCA7K_SPI_EXTERNAL (0 << 14)

#define QCASPI_CMD_LEN 2
#define QCASPI_HW_PKT_LEN 4
#define QCASPI_HW_BUF_LEN 0xC5B

/*   SPI registers;                               */
#define	SPI_REG_BFR_SIZE        0x0100
#define SPI_REG_WRBUF_SPC_AVA   0x0200
#define SPI_REG_RDBUF_BYTE_AVA  0x0300
#define SPI_REG_SPI_CONFIG      0x0400
#define SPI_REG_INTR_CAUSE      0x0C00
#define SPI_REG_INTR_ENABLE     0x0D00
#define SPI_REG_RDBUF_WATERMARK 0x1200
#define SPI_REG_WRBUF_WATERMARK 0x1300
#define SPI_REG_SIGNATURE       0x1A00
#define SPI_REG_ACTION_CTRL     0x1B00

/*   SPI_CONFIG register definition;             */
#define QCASPI_SLAVE_RESET_BIT (1 << 6)

/*   INTR_CAUSE/ENABLE register definition.      */
#define SPI_INT_WRBUF_BELOW_WM (1 << 10)
#define SPI_INT_CPU_ON         (1 << 6)
#define SPI_INT_ADDR_ERR       (1 << 3)
#define SPI_INT_WRBUF_ERR      (1 << 2)
#define SPI_INT_RDBUF_ERR      (1 << 1)
#define SPI_INT_PKT_AVLBL      (1 << 0)

/*   ACTION_CTRL register definition.            */
#define SPI_ACTRL_PKT_AVA_SPLIT_MODE (1 << 8)
#define SPI_ACTRL_PKT_AVA_INTR_MODE (1 << 0)

u16 qcaspi_read_register(struct qcaspi *qca, u16 reg);
void qcaspi_write_register(struct qcaspi *qca, u16 reg, u16 value);
int qcaspi_tx_cmd(struct qcaspi *qca, u16 cmd);

#endif

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

/*   This file contains debugging routines for use in the QCA7K driver.
 */

#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include "qca_spi.h"
#include "qca_7k.h"

#ifdef CONFIG_DEBUG_FS

/*   Dumps the contents of all SPI slave registers.        */
static int
qcaspi_regs_dump(struct seq_file *s, void *what)
{
	struct reg {
		char *name;
		u32 address;
	};

	static struct reg regs[] = {
		{ "SPI_REG_BFR_SIZE", SPI_REG_BFR_SIZE },
		{ "SPI_REG_WRBUF_SPC_AVA", SPI_REG_WRBUF_SPC_AVA },
		{ "SPI_REG_RDBUF_BYTE_AVA", SPI_REG_RDBUF_BYTE_AVA },
		{ "SPI_REG_SPI_CONFIG", SPI_REG_SPI_CONFIG },
		{ "SPI_REG_SPI_STATUS", SPI_REG_SPI_STATUS },
		{ "SPI_REG_INTR_CAUSE", SPI_REG_INTR_CAUSE },
		{ "SPI_REG_INTR_ENABLE", SPI_REG_INTR_ENABLE },
		{ "SPI_REG_RDBUF_WATERMARK", SPI_REG_RDBUF_WATERMARK },
		{ "SPI_REG_WRBUF_WATERMARK", SPI_REG_WRBUF_WATERMARK },
		{ "SPI_REG_SIGNATURE", SPI_REG_SIGNATURE },
		{ "SPI_REG_ACTION_CTRL", SPI_REG_ACTION_CTRL }
	};
	
	struct qcaspi *qca = s->private;
	int i;

	for (i = 0; i < (sizeof(regs) / sizeof(struct reg)); ++i) {
		u32 value;
		value = qcaspi_read_register(qca, regs[i].address);
		seq_printf(s, "%-25s(0x%04x): 0x%08x\n",
			regs[i].name, regs[i].address, value);
	}

	return 0;
}

static int
qcaspi_stats_show(struct seq_file *s, void *what)
{
	struct qcaspi *qca = s->private;

	seq_printf(s, "Triggered resets    : %lu\n", qca->stats.trig_reset);
	seq_printf(s, "Device resets       : %lu\n", qca->stats.device_reset);
	seq_printf(s, "Reset timeouts      : %lu\n", qca->stats.reset_timeout);
	seq_printf(s, "Read buffer errors  : %lu\n", qca->stats.read_buf_err);
	seq_printf(s, "Write buffer errors : %lu\n", qca->stats.write_buf_err);
	seq_printf(s, "Out of memory       : %lu\n", qca->stats.out_of_mem);

	return 0;
}

static int
qcaspi_regs_open(struct inode *inode, struct file *file)
{
	return single_open(file, qcaspi_regs_dump, inode->i_private);
}

static int
qcaspi_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, qcaspi_stats_show, inode->i_private);
}

static const struct file_operations qcaspi_regs_ops = {
	.open = qcaspi_regs_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations qcaspi_stats_ops = {
	.open = qcaspi_stats_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

void 
qcaspi_init_device_debugfs(struct qcaspi *qca)
{
	struct dentry *device_root;

	device_root = debugfs_create_dir(dev_name(&qca->net_dev->dev), NULL);
	qca->device_root = device_root;

	if (IS_ERR(device_root) || !device_root) {
		pr_warn("failed to create debugfs directory for %s\n",
			dev_name(&qca->net_dev->dev));
		return;
	}
	debugfs_create_file("regs", S_IFREG | S_IRUGO, device_root, qca,
			&qcaspi_regs_ops);

	debugfs_create_file("stats", S_IFREG | S_IRUGO, device_root, qca,
			&qcaspi_stats_ops);
}

void
qcaspi_remove_device_debugfs(struct qcaspi *qca)
{
	debugfs_remove_recursive(qca->device_root);
}

#else /* CONFIG_DEBUG_FS */

void
qcaspi_init_device_debugfs(struct qcaspi *qca)
{
}

void
qcaspi_remove_device_debugfs(struct qcaspi *qca)
{
}

#endif


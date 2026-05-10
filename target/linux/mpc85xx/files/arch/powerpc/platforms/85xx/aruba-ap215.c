/*
 * Aruba AP-215 Board Setup
 * Codename: Ardmore
 *
 * Copyright (C) 2024
 *
 * Based on hiveap-330.c / msm460.c
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "smp.h"

#include "mpc85xx.h"

static void __init aruba_ap215_pic_init(void)
{
	struct mpic *mpic;

	mpic = mpic_alloc(NULL, 0,
	  MPIC_BIG_ENDIAN |
	  MPIC_SINGLE_DEST_CPU,
	  0, 256, " OpenPIC  ");

	BUG_ON(mpic == NULL);
	mpic_init(mpic);
}

static void __init aruba_ap215_setup_arch(void)
{
	if (ppc_md.progress)
		ppc_md.progress("aruba_ap215_setup_arch()", 0);

	mpc85xx_smp_init();

	fsl_pci_assign_primary();

	pr_info("Aruba AP-215 (Ardmore) board\n");
}

machine_arch_initcall(aruba_ap215, mpc85xx_common_publish_devices);

define_machine(aruba_ap215) {
	.name			= "P1020 RDB",
	.compatible		= "aruba,ap-215",
	.setup_arch		= aruba_ap215_setup_arch,
	.init_IRQ		= aruba_ap215_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
	.pcibios_fixup_phb	= fsl_pcibios_fixup_phb,
#endif
	.get_irq		= mpic_get_irq,
	.progress		= udbg_progress,
};

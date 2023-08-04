/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2023 TC <tc@monkeytropolis.com>
 *
 * Based on axp305.h
 * (C) Copyright 2020 Jernej Skrabec <jernej.skrabec@siol.net>
 */

enum axp313_reg {
	AXP313_BOOT_SOURCE = 0x00,
	AXP313_CHIP_VERSION = 0x03,
	AXP313_OUTPUT_CTRL = 0x10,
	AXP313_DCDC_PWM_CTRL = 0x12,
	AXP313_DCDC1_VOLT = 0x13,
	AXP313_DCDC2_VOLT = 0x14,
	AXP313_DCDC3_VOLT = 0x15,
	AXP313_ALDO1_VOLT = 0x16,
	AXP313_DLDO1_VOLT = 0x17,
	AXP313_SHUTDOWN = 0x1A,
	AXP313_SHUTDOWN_SEQ = 0x1B,
	AXP313_WAKE_CTRL = 0x1C,
	AXP313_OUTPUT_MON = 0x1D,
	AXP313_POK_PARAM = 0x1E,
	AXP313_IRQ_CTRL = 0x20,
	AXP313_IRQ_STATUS = 0x21,
};

#define AXP1530_CHIP_ID 0x48
#define AXP313A_CHIP_ID 0x4B
#define AXP313B_CHIP_ID 0x4C

#define AXP313_DCDC1_EN (1 << 0)
#define AXP313_DCDC2_EN (1 << 1)
#define AXP313_DCDC3_EN (1 << 2)
#define AXP313_ALDO1_EN (1 << 3)
#define AXP313_DLDO1_EN (1 << 4)

#define AXP313_POWEROFF (1 << 7)

#define AXP_POWER_STATUS                0x00
#define AXP_POWER_STATUS_ALDO_IN                BIT(0)

// SPDX-License-Identifier: GPL-2.0+
/*
 * AXP313 driver
 *
 * (C) Copyright 2023 TC <tc@monkeytropolis.com>
 *
 * Based on axp221.c
 * (C) Copyright 2020 Jernej Skrabec <jernej.skrabec@siol.net>
 * (C) Copyright 2014 Hans de Goede <hdegoede@redhat.com>
 * (C) Copyright 2013 Oliver Schinagl <oliver@schinagl.nl>
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <asm/arch/pmic_bus.h>
#include <axp_pmic.h>

static u8 axp313_mvolt_to_cfg(int mvolt, int min, int max, int div)
{
	if (mvolt < min)
		mvolt = min;
	else if (mvolt > max)
		mvolt = max;

	return  (mvolt - min) / div;
}

int axp_set_dcdc1(unsigned int mvolt)
{
        //DCDC1 voltage select algo - 6bit
        //0.5-1.2V, 10mV/step, 71 steps
        //1.22-1.54V, 20mV/step, 17 steps
        //1.6-3.4V, 100mV/step, 19 steps

        int ret;
        u8 cfg;

        if (mvolt >= 1600)
                cfg = 87 + axp313_mvolt_to_cfg(mvolt, 1600, 3400, 100);
        else if (mvolt >= 1220)
                cfg = 70 + axp313_mvolt_to_cfg(mvolt, 1220, 1540, 20);
        else
                cfg = axp313_mvolt_to_cfg(mvolt, 500, 1200, 10);

        if (mvolt == 0)
                return pmic_bus_clrbits(AXP313_OUTPUT_CTRL, AXP313_DCDC1_EN);

        ret = pmic_bus_write(AXP313_DCDC1_VOLT, cfg);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313_OUTPUT_CTRL, AXP313_DCDC1_EN);
}

int axp_set_dcdc2(unsigned int mvolt)
{
        //DCDC2 voltage select algo - 6bit
        //0.5-1.2V, 10mv/step, 71 steps
        //1.22-1.54V, 20mV/step, 17 steps

        int ret;
        u8 cfg;

        if (mvolt >= 1220)
                cfg = 70 + axp313_mvolt_to_cfg(mvolt, 1220, 1300, 20);
        else
                cfg = axp313_mvolt_to_cfg(mvolt, 500, 1200, 10);

        if (mvolt == 0)
                return pmic_bus_clrbits(AXP313_OUTPUT_CTRL, AXP313_DCDC2_EN);

        ret = pmic_bus_write(AXP313_DCDC2_VOLT, cfg);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313_OUTPUT_CTRL, AXP313_DCDC2_EN);
}

int axp_set_dcdc3(unsigned int mvolt)
{
        //The DCDC3 algo specified in the datasheet is WRONG. The actual tested ranges are:
        //DCDC3 voltage select algo - 6bit
        //0.5-1.2V, 10mv/step, 71 steps
        //1.22-1.84V, 20mV/step, 31 steps

        int ret;
        u8 cfg;

        if (mvolt >= 1200)
                cfg = 70 + axp313_mvolt_to_cfg(mvolt, 1220, 1840, 20);
        else
                cfg = axp313_mvolt_to_cfg(mvolt, 500, 1200, 10);

        if (mvolt == 0)
                return pmic_bus_clrbits(AXP313_OUTPUT_CTRL, AXP313_DCDC3_EN);

        ret = pmic_bus_write(AXP313_DCDC3_VOLT, cfg);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313_OUTPUT_CTRL, AXP313_DCDC3_EN);
}

int axp_set_aldo1(unsigned int mvolt)
{
        //ALDO1 voltage select algo - 5bit
        //0.5-3.5V, 100mV/step - 31 steps

        int ret;
        u8 cfg;

        if (mvolt == 0)
                return pmic_bus_clrbits(AXP313_OUTPUT_CTRL, AXP313_ALDO1_EN);

        cfg = axp313_mvolt_to_cfg(mvolt, 500, 3500, 100);
        ret = pmic_bus_write(AXP313_ALDO1_VOLT, cfg);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313_OUTPUT_CTRL, AXP313_ALDO1_EN);
}

int axp_set_dldo(int dldo_num, unsigned int mvolt)
{
        //DLDO1 voltage select algo - 5 bit
        //0.5-3.5V, 100mV/step - 31 steps

        int ret;
        u8 cfg;

        if (dldo_num != 1)
                return -EINVAL;

        if (mvolt == 0)
                return pmic_bus_clrbits(AXP313_OUTPUT_CTRL, AXP313_DLDO1_EN);

        cfg = axp313_mvolt_to_cfg(mvolt, 500, 3500, 100);
        ret = pmic_bus_write(AXP313_DLDO1_VOLT, cfg);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313_OUTPUT_CTRL, AXP313_DLDO1_EN);
}

int axp_init(void)
{
	u8 axp_chip_id;
	int ret;

	ret = pmic_bus_init();
	if (ret)
		return ret;

	ret = pmic_bus_read(AXP313_CHIP_VERSION, &axp_chip_id);
	if (ret)
		return ret;

	if (axp_chip_id != AXP1530_CHIP_ID && axp_chip_id != AXP313A_CHIP_ID && axp_chip_id != AXP313B_CHIP_ID)
		return -ENODEV;

	return ret;
}

#if !CONFIG_IS_ENABLED(ARM_PSCI_FW) && !IS_ENABLED(CONFIG_SYSRESET_CMD_POWEROFF)
int do_poweroff(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	pmic_bus_write(AXP313_SHUTDOWN, AXP313_POWEROFF);

	/* infinite loop during shutdown */
	while (1) {}

	/* not reached */
	return 0;
}
#endif

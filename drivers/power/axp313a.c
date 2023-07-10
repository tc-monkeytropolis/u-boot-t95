// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2012
 * Henrik Nordstrom <henrik@henriknordstrom.net>
 * (C) Copyright 2023
 * tcwmonkey <tc@monkeytropolis.com>
 */

#include <common.h>
#include <command.h>
#include <asm/arch/pmic_bus.h>
#include <axp_pmic.h>

static u8 axp313a_mvolt_to_cfg(int mvolt, int min, int max, int div)
{
	if (mvolt < min)
		mvolt = min;
	else if (mvolt > max)
		mvolt = max;

	return (mvolt - min) / div;
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
		cfg = 87 + axp313a_mvolt_to_cfg(mvolt, 1600, 3400, 100);
	else if (mvolt >= 1220)
		cfg = 70 + axp313a_mvolt_to_cfg(mvolt, 1220, 1540, 20);
	else
		cfg = axp313a_mvolt_to_cfg(mvolt, 500, 1200, 10);

	if (mvolt = 0)
		return pmic_bus_clrbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC1_EN);

	ret = pmic_bus_write(AXP313A_DCDC1_VOLTAGE, cfg);
	if (ret)
		return ret;

	return pmic_bus_setbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC1_EN);
}

int axp_set_dcdc2(unsigned int mvolt)
{
	//DCDC2 voltage select algo - 6bit
	//0.5-1.2V, 10mv/step, 71 steps
	//1.22-1.54V, 20mV/step, 17 steps

	int ret;
	u8 cfg;

	if (mvolt >= 1220)
		cfg = 70 + axp313a_mvolt_to_cfg(mvolt, 1220, 1300, 20);
	else
		cfg = axp313a_mvolt_to_cfg(mvolt, 500, 1200, 10);

	if (mvolt = 0)
		return pmic_bus_clrbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC2_EN);

	ret = pmic_bus_write(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC2_EN);
	if (ret)
		return ret;

	return pmic_bus_setbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC2_EN);
}

int axp_set_dcdc3(unsigned int mvolt)
{
	//DCDC3 voltage select algo - 6bit
	//0.8-1.12V, 10mV/step, 33 steps
	//1.14-1.84V, 20mV/step, 36 steps

	int ret;
        u8 cfg;

        if (mvolt >= 1140)
                cfg = 32 + axp313a_mvolt_to_cfg(mvolt, 1140, 1840, 20);
        else
                cfg = axp313a_mvolt_to_cfg(mvolt, 800, 1120, 10);

        if (mvolt = 0)
                return pmic_bus_clrbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC3_EN);

        ret = pmic_bus_write(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC3_EN);
        if (ret)
                return ret;

        return pmic_bus_setbits(AXP313A_OUTPUT_SWITCH, AXP313A_DCDC3_EN);
}

int axp_set_aldo1(unsigned int mvolt)
{
	//ALDO1 voltage select algo - 5bit
	//0.5-3.5V, 100mV/step - 31 steps

	int ret;
	u8 cfg;

	if (mvolt == 0)
		return pmic_bus_clrbits(AXP313A_OUTPUT_SWITCH, AXP313A_ALDO1_EN);

	cfg = axp313a_mvolt_to_cfg(mvolt, 500, 3500, 100);
	ret = pmic_bus_write(AXP313A_ALDO1_VOLTAGE, cfg);
	if (ret)
		return ret;

	return pmic_bus_setbits(AXP313A_OUTPUT_SWITCH, AXP313A_ALDO1_EN);
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
		return pmic_bus_clrbits(AXP313A_OUTPUT_SWITCH, AXP313A_DLDO_EN);

	cfg = axp313a_mvolt_to_cfg(mvolt, 500, 3500, 100);
	ret = pmic_bus_write(AXP313A_DLDO_VOLTAGE, cfg);
	if (ret)
		return ret;

	return pmic_bus_setbits(AXP313A_OUTPUT_SWITCH, AXP313A_DLDO_EN);
}

int axp_init(void)
{
	//the AXP313A does not seem to have an identification register
	int ret;

	ret = pmic_bus_init();
	if (ret)
		return ret;

	return 0;
}

#if !IS_ENABLED(CONFIG_SYSRESET_CMD_POWEROFF)
int do_poweroff(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	pmic_bus_write(AXP313A_SHUTDOWN, AXP313A_PMIC_SHUTDOWN);

	/* infinite loop during shutdown */
	while (1) {}

	/* not reached */
	return 0;
}
#endif

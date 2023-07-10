/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 */

#include <linux/bitops.h>

//Registers
enum axp313a_reg {
	AXP313A_BOOT_SOURCE_INDICATOR = 0x00,
	AXP313A_OUTPUT_SWITCH = 0x10,
	AXP313A_DCDC_PWM_CONTROL = 0x12,
	AXP313A_DCDC1_VOLTAGE = 0x13,
	AXP313A_DCDC2_VOLTAGE = 0x14,
	AXP313A_DCDC3_VOLTAGE = 0x15,
	AXP313A_ALDO1_VOLTAGE = 0x16,
	AXP313A_DLDO1_VOLTAGE = 0x17,
	AXP313A_SHUTDOWN = 0x1a,
	AXP313A_SHUTDOWN_PWROK = 0x1b,
	AXP313A_WAKEUP = 0x1c,
	AXP313A_MONITOR = 0x1d,
	AXP313A_POK_PARAM = 0x1e,
	AXP313A_IRQ = 0x20,
	AXP313A_IRQ_STATUS = 0x21
};

//bitmasks
//Boot Source Indicator
#define AXP313A_EN_VIN_HIGH BIT(4)
#define AXP313A_IRQ_LOW_LEVEL_POWER_ON BIT(3)
#define AXP313A_PWRON_BUTTON BIT(2)
#define AXP313A_BOOT BIT(0)

//Output Switch
#define AXP313A_DLDO1_EN BIT(4)
#define AXP313A_ALDO1_EN BIT(3)
#define AXP313A_DCDC3_EN BIT(2)
#define AXP313A_DCDC2_EN BIT(1)
#define AXP313A_DCDC1_EN BIT(0)

//DCDC PWM Controls
#define AXP313A_DCDC_SPREAD_SPECTRUM_EN BIT(7)
#define AXP313A_DCDC_SPREAD_SPECTRUM_FREQ BIT(6) //0 = 50kHz, 1 = 100kHz
#define AXP313A_DCDC3_PFM_PWM BIT(2) //0 = Auto, 1 = Fixed PWM
#define AXP313A_DCDC2_PFM_PWM BIT(1)
#define AXP313A_DCDC1_PFM_PWM BIT(0)

//Shutdown
#define AXP313A_PMIC_SHUTDOWN BIT(7) //writing 1 shuts down the PMIC
#define AXP313A_PMIC_RESTART BIT(6) //writing 1 restarts the PMIC
#define AXP313A_PWROK_MONITOR BIT(5) //0 = Monitor Disabled, 1 = Monitor Enabled
#define AXP313A_PWROK_LOW_RESTART BIT(4) //Pull PWROK low to restart, 0 = Disabled, 1 = Enabled
#define AXP313A_THERMAL_SHUTDOWN BIT(1) //Thermal shutdown protection, 0 = Disabled, 1 = Enabled
#define AXP313A_THERMAL_SHUTDOWN_THRESH BIT(0) //0 = 125C, 1 = 145C

//Shutdown/PWROK
#define AXP313A_SHUTDOWN_SEQ BIT(3) //Shutdown sequence, 0 = All off at once, 1 = Opposite of boot sequence
#define AXP313A_SHUTDOWN_DELAY BIT(2) //4ms delay after pulling PWROK low and turning off each output, 0 = No Delay, 1 = 4ms
#define AXP313A_SHUTDOWN_KEY BIT(1) //0 = Disabled, 1 = Enabled
#define AXP313A_SHUTDOWN_KEY_RESTART BIT(0) //0 = No Automatic restart, 1 = Automatic Restart

//Wakeup
#define AXP313A_WAKEUP_IRQ_PIN BIT(4) //Whether to wake up when the IRQ pin is low, 0 = Disabled, 1 = Enabled
#define AXP313A_WAKEUP_PWROK_PULL BIT(3) //Whether the PWROK pin is pulled low during wakeup
#define AXP313A_WAKEUP_VOLTAGE_RECOVERY BIT(2) //Whether the voltages are set back to programmed defaults on wakeup, 0 = Yes, 1 = No
#define AXP313A_WAKEUP BIT(1) //Writing 1 to this bit wakes the PMIC and restores outputs
#define AXP313A_WAKEUP_EN BIT(0) //Is the wake-up function enabled from sleep mode? 0 = Off, 1 = On

//Monitor
#define AXP313A_MONITOR_DLDO1_OVERCURRENT BIT(4) //DLDO1 Overcurrent protection, 0 = Off, 1 = On
#define AXP313A_MONITOR_DCDC3_UNDERVOLT BIT(3) //DCDC3 Undervolt protection, 0 = Off, 1 = On
#define AXP313A_MONITOR_DCDC2_UNDERVOLT BIT(2) //DCDC2 Undervolt protection, 0 = Off, 1 = On
#define AXP313A_MONITOR_DCDC1_UNDERVOLT BIT(1) //DCDC1 Undervolt protection, 0 = Off, 1 = On
#define AXP313A_MONITOR_DCDC_OVERVOLT BIT(0) //DCDC Overvolt protection, 0 = Off, 1 = On

//POK Parameters
#define AXP313A_POK_OFFLEVEL BIT(1) //OFFLEVEL time, 0 = 6s, 1 = 10s
#define AXP313A_POK_IRQLEVEL BIT(0) //IRQLEVEL time, 0 = 1.5s, 1 = 2.5s

//IRQs
#define AXP313A_IRQ_PWRON_RISING_EDGE BIT(7)
#define AXP313A_IRQ_PWRON_FALLING_EDGE BIT(6)
#define AXP313A_IRQ_PWRON_SHORT_PRESS BIT(5)
#define AXP313A_IRQ_PWRON_LONG_PRESS BIT(4)
#define AXP313A_IRQ_DCDC3_UNDERVOLT BIT(3)
#define AXP313A_IRQ_DCDC2_UNDERVOLT BIT(2)
#define AXP313A_IRQ_OVERHEAT BIT(0)

//IRQ flags (write 1 to clear)
#define AXP313A_IRQ_FLAG_PWRON_RISING_EDGE BIT(7)
#define AXP313A_IRQ_FLAG_PWRON_FALLING_EDGE BIT(6)
#define AXP313A_IRQ_FLAG_PWRON_SHORT_PRESS BIT(5)
#define AXP313A_IRQ_FLAG_PWRON_LONG_PRESS BIT(4)
#define AXP313A_IRQ_FLAG_DCDC3_UNDERVOLT BIT(3)
#define AXP313A_IRQ_FLAG_DCDC2_UNDERVOLT BIT(2)
#define AXP313A_IRQ_FLAG_OVERHEAT BIT(0)

//DCDC1 voltage select algo - 6bit
//0.5-1.2V, 10mV/step, 71 steps
//1.22-1.54V, 20mV/step, 17 steps
//1.6-3.4V, 100mV/step, 19 steps

//DCDC2 voltage select algo - 6bit
//0.5-1.2V, 10mv/step, 71 steps
//1.22-1.54V, 20mV/step, 17 steps

//DCDC3 voltage select algo - 6bit
//0.8-1.12V, 10mV/step, 33 steps
//1.14-1.84V, 20mV/step, 36 steps

//ALDO1 voltage select algo - 5bit
//0.5-3.5V, 100mV/step - 31 steps

//DLDO1 voltage select algo - 5 bit
//0.5-3.5V, 100mV/step - 31 steps

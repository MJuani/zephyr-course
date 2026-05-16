/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

#define BLINK_TIME_ms		500

static const struct device *sensor_device = DEVICE_DT_GET_ANY(our_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int cmd_sensor_info(const struct shell *sh, size_t argc, char** argv){
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "name: %s - state: %s", DEVICE_DT_NAME(	DT_NODELABEL(led_sensor) ), ((device_is_ready(sensor_device)) ? "Ready" : "Not Ready"));

	return 0;
}

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char** argv){
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "calling sensor_sample_fetch");

	sensor_sample_fetch(sensor_device);

	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char** argv){
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct sensor_value sensor_val;

	sensor_channel_get(sensor_device, 0, &sensor_val);

	shell_print(sh, "sample result: %d", sensor_val.val1);

	return 0;
}

int main(void)
{
	if(!device_is_ready(sensor_device)){
		return 0;
	}

	LOG_INF("Hello World! %s\n", CONFIG_BOARD_TARGET);

	while(1){
		k_msleep(BLINK_TIME_ms);
	}

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sensor_sub_cmds,
	SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch", cmd_sensor_fetch),
	SHELL_CMD(read,  NULL, "Calls sensor_channel_get",  cmd_sensor_read),
	SHELL_CMD(info,  NULL, "Prints sensor name and state",  cmd_sensor_info),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_sub_cmds, "Sensor cmds", NULL);
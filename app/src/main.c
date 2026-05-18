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

#include "our_sensor.h"

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

static int cmd_sensor_ext_api (const struct shell *sh, size_t argc, char** argv){

	uint16_t param = 0;

	if(argc != 2){
		shell_error(sh, "set: wrong parameter count");
		return 0;
	}

	param = atoi(argv[1]);

	if(param >= 2048){
		shell_error(sh, "set: value must be below 2048");
		return 0;
	}

	shell_print(sh, "calling sensor_extension_api with value %d", param);

	const struct custom_sensor_api_extension *api = sensor_device->api;
	
	if(api->ext_api){
		api->ext_api(sensor_device, param);
	}	

	return 0;
}

int main(void)
{
	if(!device_is_ready(sensor_device)){
		return 0;
	}

	LOG_INF("Hello World! %s\n", CONFIG_BOARD_TARGET);

	const struct custom_sensor_api_extension *api = sensor_device->api;
	
	if(api->ext_api){
		api->ext_api(sensor_device, 1024);
	}

	while(1){
		k_msleep(BLINK_TIME_ms);
	}

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sensor_sub_cmds,
	SHELL_CMD		(fetch, NULL, "Calls sensor_sample_fetch", 		cmd_sensor_fetch),
	SHELL_CMD		(read,  NULL, "Calls sensor_channel_get", 		cmd_sensor_read),
	SHELL_CMD		(info,  NULL, "Prints sensor name and state",  	cmd_sensor_info),
	SHELL_CMD_ARG 	(set,  	NULL, "Calls sensor_ext_api",		  	cmd_sensor_ext_api, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_sub_cmds, "Sensor cmds", NULL);

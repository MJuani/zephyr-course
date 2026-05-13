/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#define BLINK_TIME_ms		500

static const struct device *sensor_device = DEVICE_DT_GET_ANY(our_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	if(!device_is_ready(sensor_device)){
		return 0;
	}

	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	while(1){
		sensor_sample_fetch(sensor_device);
		k_msleep(BLINK_TIME_ms);
		sensor_channel_get(sensor_device,0,NULL);
		k_msleep(BLINK_TIME_ms);
	}

	return 0;
}

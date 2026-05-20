/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define BLINK_TIME_ms		500

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{

	LOG_INF("Hello World! %s\n", CONFIG_BOARD_TARGET);

	while(1){
		k_msleep(BLINK_TIME_ms);
	}

	return 0;
}

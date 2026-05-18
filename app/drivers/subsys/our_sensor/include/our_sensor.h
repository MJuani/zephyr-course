#pragma once

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C"{
#endif
    
/* custom_sensor.h */
typedef int (*custom_extension_api_t)(const struct device *dev, uint32_t new_val);

struct custom_sensor_api_extension {
    struct sensor_driver_api common; // Keep the standard one first
    custom_extension_api_t ext_api; // Your new function
};



#ifdef __cplusplus
}
#endif

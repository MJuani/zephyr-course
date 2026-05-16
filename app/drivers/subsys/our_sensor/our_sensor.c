#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_sensor

LOG_MODULE_REGISTER(our_sensor, CONFIG_SENSOR_LOG_LEVEL);

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "Our Sensor driver enabled without any devices"
#endif

struct our_sensor_data{
    int led_state;
};

struct our_sensor_config{
    struct gpio_dt_spec pin;
};

static int our_sensor_init (const struct device *dev){
    const struct our_sensor_config *config = dev->config;

    if (!gpio_is_ready_dt(&config->pin)) {
        return 0;
    }

    gpio_pin_configure_dt(&config->pin, GPIO_OUTPUT_INACTIVE);

    return 0;    
}

static int our_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan){
    const struct our_sensor_config *config = dev->config;
    struct our_sensor_data *data = dev->data;
    data->led_state = 1;
    return gpio_pin_set_dt(&config->pin, 1);
}

static int our_sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val){
    const struct our_sensor_config *config = dev->config;
    struct our_sensor_data *data = dev->data;
    data->led_state = 0;
    val->val1 = 1024;
    val->val2 = 0;
    return gpio_pin_set_dt(&config->pin, 0);
}

static const struct sensor_driver_api our_sensor_api = {
    .sample_fetch = &our_sensor_sample_fetch,
    .channel_get = &our_sensor_channel_get,
};

#define OUR_SENSOR_DEFINE(inst)                                  \
    static struct our_sensor_data data_##inst;                   \
    static const struct our_sensor_config config_##inst = {      \
        .pin = GPIO_DT_SPEC_INST_GET(inst, gpios),                     \
    };                                                           \
                                                                 \
    DEVICE_DT_INST_DEFINE(inst,                                  \
                our_sensor_init,                                 \
                NULL,                                            \
                &data_##inst,                                    \
                &config_##inst,                                  \
                POST_KERNEL,                                     \
                CONFIG_SENSOR_INIT_PRIORITY,                     \
                &our_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_SENSOR_DEFINE)

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>

/* Constants ----------------------------------- */

/* The devicetree node identifier for the "pwm_led0" alias. */
#define PWM_NODE DT_ALIAS(pwm_led0)

#define NUM_STEPS	        50U
#define SLEEP_MSEC	        (CONFIG_LED_FADE_DURATION/NUM_STEPS)

#define MAX_BRIGHTNESS      CONFIG_LED_BRIGHTNESS/100

/* Types --------------------------------------- */

typedef enum{
	TURNING_OFF,
	TURNING_ON
}pwm_led_state_e;

typedef enum{
	LED_OFF,
	LED_PWM
}led_state_e;

/* Variables ----------------------------------- */
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_NODE);


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Functions ------------------------------------*/

int main(void)
{
	uint32_t pulse_width = 0;
	uint32_t steps = (pwm_led.period * MAX_BRIGHTNESS) / NUM_STEPS;
	pwm_led_state_e dir = TURNING_ON;
	led_state_e 	led_s = LED_PWM;
    int ret;

	if (!pwm_is_ready_dt(&pwm_led)) return 0;

    while (1) {

        switch(led_s){
			case LED_PWM:

				ret = pwm_set_pulse_dt(&pwm_led, pulse_width);
				if (ret) {
					LOG_ERR("Error %d: failed to set pulse width for PWM LED \r\n", ret);
				}
			
        		if(IS_ENABLED(CONFIG_LED_DEBUG)){
				    LOG_INF("PWM LED: Using pulse width %d%% \r\n", 100 * pulse_width / pwm_led.period);
        		}
			
				if (dir == TURNING_ON) {
					if (pulse_width + steps >= (pwm_led.period * MAX_BRIGHTNESS)) {
						pulse_width = (pwm_led.period * MAX_BRIGHTNESS);
						dir = TURNING_OFF;
					} else {
						pulse_width += steps;
					}
				} else {
					if (pulse_width <= steps) {
						pulse_width = 0;
						dir = TURNING_ON;
						led_s = LED_OFF;
					} else {
						pulse_width -= steps;
					}
				}        
			
        		k_sleep(K_MSEC(SLEEP_MSEC));
				break;

			case LED_OFF:

				ret = pwm_set_pulse_dt(&pwm_led, pulse_width);
				if (ret) {
					LOG_ERR("Error %d: failed to set pulse width for PWM LED \r\n", ret);
				}
				
				if(IS_ENABLED(CONFIG_LED_DEBUG)){
				    LOG_INF("PWM LED: Led OFF \r\n");
        		}

				led_s = LED_PWM;

				k_sleep(K_MSEC(CONFIG_LED_BLINK_SLEEP));
				
				break;

			default:
				break;
		}
    }

    return 0;
}

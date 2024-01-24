#ifndef __LED_MODULE_H
#define __LED_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

void led_init(void);

void single_led_on(uint16_t GPIO_Pin);
void single_led_off(uint16_t GPIO_Pin);
void blink_control(uint16_t GPIO_Pin);
void light_water_control(uint8_t min, uint8_t max, uint16_t GPIO_Pin);
void leds_task(void *param);
void do_create_led_task(void);


#ifdef __cplusplus
}
#endif

#endif /* __LED_MODULE_H */



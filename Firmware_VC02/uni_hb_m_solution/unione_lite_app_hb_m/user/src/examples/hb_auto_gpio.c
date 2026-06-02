#include "user_config.h"
#include "user_event.h"
#include "user_gpio.h"
#include "user_player.h"
#include "user_pwm.h"
#include "user_timer.h"
#include "user_uart.h"

#define TAG "auto_gpio"

#define UART_SEND_MAX      16

typedef struct {
  char  data[UART_SEND_MAX];
  int   len;
}uart_data_t;

const uart_data_t g_uart_buf[] = {
  {{0x01}, 1}, //MOVE_FRONT
  {{0x02}, 1}, //MOVE_BACK
  {{0x03}, 1}, //TURN_LEFT
  {{0x04}, 1}, //TURN_RIGHT
  {{0x05}, 1}, //STOP
  {{0x06}, 1}, //LED_ON
  {{0x07}, 1}, //LED_OFF
  {{0x08}, 1}, //FAN_ON
  {{0x09}, 1}, //FAN_OFF
  {{0x0A}, 1}, //Bed
  {{0x0B}, 1}, //chair
  {{0x0C}, 1}, //Toilet
  {{0x0D}, 1}, //close_toilet
};

static void _custom_setting_cb(USER_EVENT_TYPE event,
                               user_event_context_t *context) {
  event_custom_setting_t *setting = NULL;
  if (context) {
    setting = &context->custom_setting;
    LOGT(TAG, "user command: %s", setting->cmd);
    if (0 == uni_strcmp(setting->cmd, "MOVE_FRONT")) {
      user_uart_send(g_uart_buf[0].data, g_uart_buf[0].len);
    } else if (0 == uni_strcmp(setting->cmd, "MOVE_BACK")) {
      user_uart_send(g_uart_buf[1].data, g_uart_buf[1].len);
    } else if (0 == uni_strcmp(setting->cmd, "TURN_LEFT")) {
      user_uart_send(g_uart_buf[2].data, g_uart_buf[2].len);
    } else if (0 == uni_strcmp(setting->cmd, "TURN_RIGHT")) {
      user_uart_send(g_uart_buf[3].data, g_uart_buf[3].len);
    } else if (0 == uni_strcmp(setting->cmd, "STOP")) {
      user_uart_send(g_uart_buf[4].data, g_uart_buf[4].len);
    } else if (0 == uni_strcmp(setting->cmd, "LED_ON")) {
      user_uart_send(g_uart_buf[5].data, g_uart_buf[5].len);
    } else if (0 == uni_strcmp(setting->cmd, "LED_OFF")) {
      user_uart_send(g_uart_buf[6].data, g_uart_buf[6].len);
    } else if (0 == uni_strcmp(setting->cmd, "FAN_ON")) {
      user_uart_send(g_uart_buf[7].data, g_uart_buf[7].len);
    } else if (0 == uni_strcmp(setting->cmd, "FAN_OFF")) {
      user_uart_send(g_uart_buf[8].data, g_uart_buf[8].len);
    } else if (0 == uni_strcmp(setting->cmd, "Bed")) {
      user_uart_send(g_uart_buf[9].data, g_uart_buf[9].len);
    } else if (0 == uni_strcmp(setting->cmd, "chair")) {
      user_uart_send(g_uart_buf[10].data, g_uart_buf[10].len);
    } else if (0 == uni_strcmp(setting->cmd, "Toilet")) {
      user_uart_send(g_uart_buf[11].data, g_uart_buf[11].len);
    } else if (0 == uni_strcmp(setting->cmd, "close_toilet")) {
      user_uart_send(g_uart_buf[12].data, g_uart_buf[12].len);
    } else {
      LOGT(TAG, "Unconcerned command: %s", setting->cmd);
    }
    user_player_reply_list_random(setting->reply_files);
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
}

int hb_auto_gpio(void) {
  user_gpio_init();
  user_gpio_set_mode(GPIO_NUM_A25, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A25, 0);
  user_gpio_set_mode(GPIO_NUM_A26, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A26, 0);
  user_gpio_set_mode(GPIO_NUM_A27, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A27, 0);
  user_gpio_set_mode(GPIO_NUM_A28, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A28, 0);
  user_gpio_set_mode(GPIO_NUM_B2, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B2, 0);
  user_gpio_set_mode(GPIO_NUM_B3, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B3, 0);
  user_uart_init(NULL);
  _register_event_callback();
  return 0;
}


#ifndef INC_UNI_CMD_CODE_H_
#define INC_UNI_CMD_CODE_H_

typedef struct {
  uni_u8      cmd_code; /* cmd code fro send base on SUCP */
  const char  *cmd_str; /* action string on UDP */;
} cmd_code_map_t;

const cmd_code_map_t g_cmd_code_arry[] = {
  {0x0, "wakeup_uni"},
  {0x1, "exitUni"},
  {0x2, "MOVE_FRONT"},
  {0x3, "MOVE_BACK"},
  {0x4, "TURN_LEFT"},
  {0x5, "TURN_RIGHT"},
  {0x6, "STOP"},
  {0x7, "Love_You"},
  {0x8, "LED_ON"},
  {0x9, "LED_OFF"},
  {0xa, "FAN_ON"},
  {0xb, "FAN_OFF"},
  {0xc, "Bed"},
  {0xd, "chair"},
  {0xe, "Toilet"},
  {0xf, "close_toilet"},
};

#endif

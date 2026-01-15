#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

// system state which is using by sensor task, display task
typedef struct {
  // update by sensor task
  float humidity;
  float temperature;

  // update by sensor task
  int humidity_alarm;
  int temperature_alarm;

  // update by rx from pc task
  int sht30_measurement_period;
  int lcd_brightness;
  int lcd_display_mode;
} SYSTEM_STATE;
#endif // __SYSTEM_STATE_H__
/*
 * main.h
 *
 *  Created on: 2016/08/22
 *      Author: dfm1

  (CC at)2016 by D.F.Mac. @TripArts Music
 */

#ifndef MAIN_H_
#define MAIN_H_

#define LEDTIMEMAX 512
#define WAIT_TIME_MS 2

#define CAN_TIMEBASE 1664
#define CAN_TIMESTEP 16


void SystemClock_Config(void);
void Error_Handler(int num);
static void MX_GPIO_Init(void);

static int getNoteCh(uint8_t noteNum);

static void onNoteOn(uint8_t ch, uint8_t note, uint8_t vel);
static void onNoteOff(uint8_t ch, uint8_t note, uint8_t vel);
static void onCtlChange(uint8_t ch, uint8_t num, uint8_t value);

void CanStart(int ch);
void CanEnd(int ch);

#define CAN_CHANNELS 6

typedef struct _TCanGPIO{
  GPIO_TypeDef *pOutGPIOPort;
  uint16_t OutPin;
  uint16_t dummy;
} TCanGPIO;

void trigNoteOff();
int main(void);

#endif /* MAIN_H_ */

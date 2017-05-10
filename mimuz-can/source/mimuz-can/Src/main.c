//
// mi:muz:can v0.20
//
//  (CC at)2017 by D.F.Mac. @TripArts Music
//
//
// PIN Configuration
//
// PA0 : CAN OUT #1  (OUT)
// PA1 : CAN OUT #2  (OUT)
// PA2 : CAN OUT #3  (OUT)
// PA3 : CAN OUT #4  (OUT)
// PA4 : CAN OUT #5  (OUT)
// PA5 : CAN OUT #6  (OUT)
// PB1 : LED (OUT)
// PA8 : N/A
// PA9 : N/A
// PA10: N/A
// PA11: USB D-
// PA12: USB D+
// PA13: SWDIO
// PA14: SWCLK
// PA15: N/A
// PB3 : N/A
// PB4 : N/A
// PB5 : N/A
// PB6 : N/A
// PB7 : N/A
// PB8 : BOOT0
// PF0 : N/A
// PF1 : N/A

#include "stm32f0xx_hal.h"
#include "usb_device.h"
#include "usbd_midi_if.h"
#include "eeprom.h"
#include "main.h"

// Can GPIO Table
TCanGPIO tCanGpio[CAN_CHANNELS]={
  {GPIOA, GPIO_PIN_0},
  {GPIOA, GPIO_PIN_1},
  {GPIOA, GPIO_PIN_2},
  {GPIOA, GPIO_PIN_3},
  {GPIOA, GPIO_PIN_4},
  {GPIOA, GPIO_PIN_5} 
};

// EEPROM
uint16_t VirtAddVarTab[NB_OF_VAR] = {
  0x1000, 0x1001, 0x1002, 0x1003,
  0x1004, 0x1005, 0x1006, 0x1007
};

uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0, 0, 0, 0, 0, 0};

// MIDI
int val[] = {0,0,0,0,0,0};

//uint8_t notes[] = {36,38,40,43,45,48}; // C2 D2 E2 G2 A2 C3
uint8_t notes[] = {60,62,64,67,69,72}; // C4 D4 E4 G4 A4 C5
int onoff[] = {0,0,0,0,0,0};

uint16_t ledTime = 0;

void debug_led(int times){
  int cnt;
  while(1){
    for(cnt = 0;cnt < times;cnt ++){
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
      HAL_Delay(200);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
      HAL_Delay(200);
    }
    HAL_Delay(1000);
  }
}

int debug_status = 0;


void led_on(void){
  ledTime = LEDTIMEMAX;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void led_off(void){
  ledTime = 0;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void onNoteOn(uint8_t ch, uint8_t note, uint8_t vel){
  int canNumber;
  led_on();

  canNumber = getNoteCh(note);
  if(canNumber != -1){
	  if(vel != 0){
	    onoff[canNumber] = CAN_TIMEBASE + (CAN_TIMESTEP * vel);
	    CanStart(canNumber);
	  }
  }
}

static void onNoteOff(uint8_t ch, uint8_t note, uint8_t vel){
}


static int getNoteCh(uint8_t noteNum){
  int cnt;
  for(cnt=0;cnt<CAN_CHANNELS;cnt++){
    if(notes[cnt] == noteNum){
      return cnt;
    }
  }
  return -1;
}

uint8_t mode = 0; // 0: none 1: write 2: read

static void onCtlChange(uint8_t ch, uint8_t num, uint8_t value){
  uint8_t func;

  led_on();
  if(ch == 15){
    func = num - 110;
    if(func == 9){        // 119
      mode = value;
    }else if(func < 6){  // Preset-number 110-115
      if(mode == 1){
        EE_WriteVariable(VirtAddVarTab[func],value);
        notes[func] = value;
      }else{  // mode == 2
        sendCtlChange(15,119,1);
        processMidiMessage();
        sendCtlChange(15,num,notes[func]);
        processMidiMessage();
      }
      mode = 0;
    }
  }
}

void loadNums(){
  uint8_t cnt;
  uint8_t cntmax = CAN_CHANNELS; // note numbers
  uint16_t chk;
  chk = EE_ReadVariable(VirtAddVarTab[7],&VarDataTab[7]);
  if(chk == 0){
    for(cnt = 0;cnt < cntmax;cnt ++){
      EE_ReadVariable(VirtAddVarTab[cnt],&VarDataTab[cnt]);
      notes[cnt] = (uint8_t)VarDataTab[cnt];
    }
  }else{
    for(cnt = 0;cnt < cntmax;cnt ++){
      EE_WriteVariable(VirtAddVarTab[cnt],(uint16_t)(notes[cnt]));
    }
    EE_WriteVariable(VirtAddVarTab[7],8);
  }
}


void CanStart(int ch){
  HAL_GPIO_WritePin(tCanGpio[ch].pOutGPIOPort, tCanGpio[ch].OutPin, GPIO_PIN_SET);
}

void CanEnd(int ch){
  HAL_GPIO_WritePin(tCanGpio[ch].pOutGPIOPort, tCanGpio[ch].OutPin, GPIO_PIN_RESET);
}

int old_button = GPIO_PIN_RESET;


void trigNoteOff(){
  int cnt;

  for(cnt=0;cnt<CAN_CHANNELS;cnt++){
    if(onoff[cnt] > 0){
      onoff[cnt] --;
      if(onoff[cnt] == 0){
        CanEnd(cnt);
      }
    }
  }

  if(ledTime > 0){
    ledTime --;
    if(ledTime == 0){
      led_off();
    }
  }


}

int main(void){
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_Delay(600);

  MX_USB_DEVICE_Init();
  HAL_FLASH_Unlock();

  EE_Init();
  loadNums();

  // mimuz
  mimuz_init();
  setHdlNoteOff(onNoteOff);
  setHdlNoteOn(onNoteOn);
  setHdlCtlChange(onCtlChange);

  while (1){
    processMidiMessage();
    trigNoteOff();

/*
    if(debug_status == 0){
      debug_led(debug_status);
    }
*/
//    HAL_Delay(WAIT_TIME_MS);

  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(4);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler(5);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler(6);
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


// PA0 : CAN OUT #1  (OUT)
// PA1 : CAN OUT #2  (OUT)
// PA2 : CAN OUT #3  (OUT)
// PA3 : CAN OUT #4  (OUT)
// PA4 : CAN OUT #5  (OUT)
// PA5 : CAN OUT #6  (OUT)
// PB1 : LED (OUT)

static void MX_GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 PA4 PA5 */ 
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(int num){
  sendCtlChange(15,4,num); // debug
  led_on();
  while(1){
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){}
#endif

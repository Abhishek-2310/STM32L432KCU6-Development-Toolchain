/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command.
 *   Also established an I2C communication protocol between a master (STM32F411RE) 
 *   and a slave (STM32L432KC)
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"

// extern UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;
uint8_t buffer[4];

void I2CSlaveInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    printf("RCCEx Peripheral Clock Config Failed!\r\n");
  }

  __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PA9     ------> I2C1_SCL
    PA10     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
  __HAL_RCC_I2C1_CLK_ENABLE();

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0x5A;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    printf("I2C init Failed!\r\n");
  }

  /** Configure Analogue filter
  */
  // if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  // {
  //   printf("I2CEx Analog Filter Config Failed!\r\n");
  // }

  // /** Configure Digital filter
  // */
  // if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  // {
  //   printf("I2CEx Digital Filter Config Failed!\r\n");
  // }

  if (HAL_I2C_Slave_Receive(&hi2c1, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK)
  {
	  asm("bkpt 255");
  }
}

void I2CSlaveTask(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
}

ADD_TASK(I2CSlaveTask,  /* This is the name of the function for the task */
	 I2CSlaveInit,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t I2CSlaveExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  // uint8_t data[] = "HELLO WORLD \r\n";
  // HAL_UART_Transmit(&huart2, (uint8_t *) data, sizeof(data), 10);

  printf("I2C Master Command\n");

  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("i2c_slave",I2CSlaveExample,"          I2C Slave Command")

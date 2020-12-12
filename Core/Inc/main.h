/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define JOYSTICK_SELECT_Pin GPIO_PIN_13
#define JOYSTICK_SELECT_GPIO_Port GPIOC
#define JOYSTICK_SELECT_EXTI_IRQn EXTI15_10_IRQn
#define BUCKBOOST_LOAD_1_Pin GPIO_PIN_14
#define BUCKBOOST_LOAD_1_GPIO_Port GPIOC
#define BUCKBOOST_LOAD_2_Pin GPIO_PIN_15
#define BUCKBOOST_LOAD_2_GPIO_Port GPIOC
#define BUCKBOOST_USBPD_EN_Pin GPIO_PIN_3
#define BUCKBOOST_USBPD_EN_GPIO_Port GPIOC
#define BUCKBOOST_VIN_Pin GPIO_PIN_1
#define BUCKBOOST_VIN_GPIO_Port GPIOA
#define BUCKBOOST_I_IN_AVG_Pin GPIO_PIN_2
#define BUCKBOOST_I_IN_AVG_GPIO_Port GPIOA
#define BUCKBOOST_VOUT_Pin GPIO_PIN_3
#define BUCKBOOST_VOUT_GPIO_Port GPIOA
#define JOYSTICK_LEFT_Pin GPIO_PIN_4
#define JOYSTICK_LEFT_GPIO_Port GPIOC
#define JOYSTICK_LEFT_EXTI_IRQn EXTI4_IRQn
#define JOYSTICK_DOWN_Pin GPIO_PIN_5
#define JOYSTICK_DOWN_GPIO_Port GPIOC
#define JOYSTICK_DOWN_EXTI_IRQn EXTI9_5_IRQn
#define LED_LEFT_ORANGE_Pin GPIO_PIN_1
#define LED_LEFT_ORANGE_GPIO_Port GPIOB
#define JOYSTICK_RIGHT_Pin GPIO_PIN_2
#define JOYSTICK_RIGHT_GPIO_Port GPIOB
#define JOYSTICK_RIGHT_EXTI_IRQn EXTI2_IRQn
#define JOYSTICK_UP_Pin GPIO_PIN_10
#define JOYSTICK_UP_GPIO_Port GPIOB
#define JOYSTICK_UP_EXTI_IRQn EXTI15_10_IRQn
#define BUCKBOOST_I_IN_SENSE_Pin GPIO_PIN_11
#define BUCKBOOST_I_IN_SENSE_GPIO_Port GPIOB
#define BUCKBOOST_P1_DRIVE_Pin GPIO_PIN_12
#define BUCKBOOST_P1_DRIVE_GPIO_Port GPIOB
#define BUCKBOOST_N1_DRIVE_Pin GPIO_PIN_13
#define BUCKBOOST_N1_DRIVE_GPIO_Port GPIOB
#define BUCKBOOST_N2_DRIVE_Pin GPIO_PIN_14
#define BUCKBOOST_N2_DRIVE_GPIO_Port GPIOB
#define BUCKBOOST_P2_DRIVE_Pin GPIO_PIN_15
#define BUCKBOOST_P2_DRIVE_GPIO_Port GPIOB
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define LED_DOWN_BLUE_Pin GPIO_PIN_15
#define LED_DOWN_BLUE_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define LED_UP_RED_Pin GPIO_PIN_5
#define LED_UP_RED_GPIO_Port GPIOB
#define LED_RIGHT_GREEN_Pin GPIO_PIN_7
#define LED_RIGHT_GREEN_GPIO_Port GPIOB
#define GPO1_Pin GPIO_PIN_9
#define GPO1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* Filter parameter P: number of feed-forward taps or coefficients in the range [2:64] */
#define COEFF_VECTOR_B_SIZE     4

/* Filter parameter Q: number of feedback coefficients in the range [1:COEFF_VECTOR_B_SIZE-1] */
#define COEFF_VECTOR_A_SIZE     3

/* Throughput parameter: extra space in the input buffer (minimum: 0) */
#define MEMORY_PARAMETER_D1     1

/* Throughput parameter: extra space in the output buffer (minimum: 1) */
#define MEMORY_PARAMETER_D2     2

/* Throughput parameter: watermark threshold for the input buffer */
#define INPUT_THRESHOLD         FMAC_THRESHOLD_1

/* Throughput parameter: watermark threshold for the output buffer (inferior or equal to MEMORY_PARAMETER_D1) */
#define OUTPUT_THRESHOLD        FMAC_THRESHOLD_1

/* FMAC internal memory configuration: base address of the coefficient buffer */
#define COEFFICIENT_BUFFER_BASE 0

/* FMAC internal memory configuration: size of the two coefficient buffers */
#define COEFFICIENT_BUFFER_SIZE COEFF_VECTOR_B_SIZE + COEFF_VECTOR_A_SIZE

/* FMAC internal memory configuration: base address of the input buffer */
#define INPUT_BUFFER_BASE       COEFFICIENT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the input buffer */
#define INPUT_BUFFER_SIZE       COEFF_VECTOR_B_SIZE + MEMORY_PARAMETER_D1

/* FMAC internal memory configuration: base address of the output buffer */
#define OUTPUT_BUFFER_BASE      COEFFICIENT_BUFFER_SIZE + INPUT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the output buffer */
#define OUTPUT_BUFFER_SIZE      COEFF_VECTOR_A_SIZE + MEMORY_PARAMETER_D2

/* FMAC WDATA register address (for DMA transfer) */
#define FMAC_WDATA (0x40021418)

/* HR timer COMP register address (for DMA transfer */
#define HRTIM_TIMA_CMP1 (0x4001689C)

/* Coefficients for a 200kHz power supply at 170MHz */
/* Fixed point coefficients */
#define B0 (0x5521)
#define B1 (0xB564)
#define B2 (0xAB31)
#define B3 (0x4AEE)
#define A1 (0x616)
#define A2 (0xFE93)
#define A3 (0xFF57)
#define pre_shift (+3)
#define post_shift (+5)
#define DUTY_TICKS_MIN (0)
#define DUTY_TICKS_MAX (24480)


#define BUCK_CONFIG			1
//#define BOOST_CONFIG		1


#define CONST_REF			235						// R2/(R1+R2) * 4096 / 3.3V = equivalent to 1V


#define fHRTIM_HZ       	170e6   				// Internal clock frequency usage


/****************************************************************************************************/
#define HRTIM_FREQ 			200e3f 					// Buck freq. or Boost part when used in Buck&Boost mode

#define HRTIM_TC_RISING		200
#define HRTIM_TC_FALLING	200

#define HRTIM_TD_RISING		200
#define HRTIM_TD_FALLING	200
/****************************************************************************************************/

/****************************************************************************************************/
//#define HRTIM_FREQ 			800e3f 				// Buck freq. or Boost part when used in Buck&Boost mode
//
//#define HRTIM_TC_RISING		13
//#define HRTIM_TC_FALLING	13
//
//#define HRTIM_TD_RISING		13
//#define HRTIM_TD_FALLING	13
/****************************************************************************************************/


#define HRTIM_PERIOD		fHRTIM_HZ * 32 / HRTIM_FREQ 			// Buck period. or Boost part when used in Buck&Boost mode

#define HRTIM_PERIOD_MAX  	fHRTIM_HZ * 32 / HRTIM_FREQ * 0.9   	// Max allowed duty when using hi-res PWM

#define HRTIM_PERIOD_MIN  	50    									// Min allowed duty when using hi-res PWM


#ifdef BUCK_CONFIG
#define REF 	CONST_REF * 3.3										// Buck system, VIN = 5V, VOUT = 3.3V
#endif

#ifdef BOOST_CONFIG
#define REF 	CONST_REF * 9										// Boost system, VIN = 5V, VOUT = 9V
#endif

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

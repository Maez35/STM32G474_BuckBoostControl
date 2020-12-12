/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g4xx_it.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

FMAC_HandleTypeDef hfmac;

HRTIM_HandleTypeDef hhrtim1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
/* FMAC configuration structure */
FMAC_FilterConfigTypeDef sFmacConfig;
DMA_HandleTypeDef hdma_fmac_read;

/* Array of filter coefficients A (feedback coefficients) in Q1.15 format */
static int16_t aFilterCoeffA[COEFF_VECTOR_A_SIZE] = {A1,A2,A3};

/* Array of filter coefficients B (feed-forward taps) in Q1.15 format */
static int16_t aFilterCoeffB[COEFF_VECTOR_B_SIZE] = {(int16_t)-B0,(int16_t)-B1,(int16_t)-B2,(int16_t)-B3};

/* Array of output data to preload in Q1.15 format */
static int16_t aOutputDataToPreload[COEFF_VECTOR_A_SIZE] = {0x0000, 0x0000, 0x0000};

/* Expected number of calculated samples */
uint16_t ExpectedCalculatedOutputSize = (uint16_t) 1;
uint32_t *Fmac_Wdata;
int16_t Fmac_output;
int16_t bTransient = 0;
//extern G4_Demo_t Demo;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_HRTIM1_Init(void);
static void MX_FMAC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_RTC_Init();
	MX_ADC1_Init();
	MX_HRTIM1_Init();
	MX_FMAC_Init();
	/* USER CODE BEGIN 2 */

	/* Perform an ADC automatic self-calibration and enable ADC */
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);


#ifdef RUN_OPEN_LOOP
	/* Do not start ADC and DMA, instead set fixed duty cycle */
	__HAL_HRTIM_SETCOMPARE( &hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_1, 10000);
#else
	/* Start ADC and DMA */
	Fmac_Wdata = (uint32_t *) FMAC_WDATA;
	HAL_ADC_Start_DMA(&hadc1,Fmac_Wdata,1);
#endif


#ifdef BUCK_CONFIG
	/* Start the PWMs */
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2);
	HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_TIMER_C);

	/* Enable T6 for Buck only mode usage */
	HAL_GPIO_WritePin(BUCKBOOST_P2_DRIVE_GPIO_Port, BUCKBOOST_P2_DRIVE_Pin, GPIO_PIN_SET);
#endif

#ifdef BOOST_CONFIG
	/* Start the PWMs */
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2);
	HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_TIMER_D);

	/* Enable T6 for Buck only mode usage */
	HAL_GPIO_WritePin(BUCKBOOST_P1_DRIVE_GPIO_Port, BUCKBOOST_P1_DRIVE_Pin, GPIO_PIN_SET);
#endif

	HAL_GPIO_WritePin(LED_RIGHT_GREEN_GPIO_Port, LED_RIGHT_GREEN_Pin, GPIO_PIN_SET);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV10;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the peripherals clocks
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC12;
	PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_PLL;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_1;
	sConfig.Offset = REF;
	sConfig.OffsetSign = ADC_OFFSET_SIGN_NEGATIVE;
	sConfig.OffsetSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief FMAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_FMAC_Init(void)
{

	/* USER CODE BEGIN FMAC_Init 0 */

	/* USER CODE END FMAC_Init 0 */

	/* USER CODE BEGIN FMAC_Init 1 */

	/* USER CODE END FMAC_Init 1 */
	hfmac.Instance = FMAC;
	if (HAL_FMAC_Init(&hfmac) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN FMAC_Init 2 */

	/*## Configure the FMAC peripheral ###########################################*/
	sFmacConfig.InputBaseAddress  = INPUT_BUFFER_BASE;
	sFmacConfig.InputBufferSize   = INPUT_BUFFER_SIZE;
	sFmacConfig.InputThreshold    = INPUT_THRESHOLD;
	sFmacConfig.CoeffBaseAddress  = COEFFICIENT_BUFFER_BASE;
	sFmacConfig.CoeffBufferSize   = COEFFICIENT_BUFFER_SIZE;
	sFmacConfig.OutputBaseAddress = OUTPUT_BUFFER_BASE;
	sFmacConfig.OutputBufferSize  = OUTPUT_BUFFER_SIZE;
	sFmacConfig.OutputThreshold   = OUTPUT_THRESHOLD;
	sFmacConfig.pCoeffA           = aFilterCoeffA;
	sFmacConfig.CoeffASize        = COEFF_VECTOR_A_SIZE;
	sFmacConfig.pCoeffB           = aFilterCoeffB;
	sFmacConfig.CoeffBSize        = COEFF_VECTOR_B_SIZE;
	sFmacConfig.Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1;
	sFmacConfig.InputAccess       = FMAC_BUFFER_ACCESS_NONE;
	sFmacConfig.OutputAccess      = FMAC_BUFFER_ACCESS_IT;
	sFmacConfig.Clip              = FMAC_CLIP_ENABLED;
	sFmacConfig.P                 = COEFF_VECTOR_B_SIZE;
	sFmacConfig.Q                 = COEFF_VECTOR_A_SIZE;
	sFmacConfig.R                 = post_shift;

	if (HAL_FMAC_FilterConfig(&hfmac, &sFmacConfig) != HAL_OK)
	{
		/* Configuration Error */
		Error_Handler();
	}

	/*## Preload the input and output buffers ####################################*/
	if (HAL_FMAC_FilterPreload(&hfmac, NULL, INPUT_BUFFER_SIZE,
			aOutputDataToPreload, COEFF_VECTOR_A_SIZE) != HAL_OK)
	{
		/* Configuration Error */
		Error_Handler();
	}
	/* Start calculation of IIR filter */
	if (HAL_FMAC_FilterStart(&hfmac,&Fmac_output,&ExpectedCalculatedOutputSize) != HAL_OK)
	{
		/* Processing Error */
		Error_Handler();
	}

	/* USER CODE END FMAC_Init 2 */

}

/**
 * @brief HRTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_HRTIM1_Init(void)
{

	/* USER CODE BEGIN HRTIM1_Init 0 */

	/* USER CODE END HRTIM1_Init 0 */

	HRTIM_ADCTriggerCfgTypeDef pADCTriggerCfg = {0};
	HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg = {0};
	HRTIM_TimerCtlTypeDef pTimerCtl = {0};
	HRTIM_TimerCfgTypeDef pTimerCfg = {0};
	HRTIM_CompareCfgTypeDef pCompareCfg = {0};
	HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg = {0};
	HRTIM_OutputCfgTypeDef pOutputCfg = {0};

	/* USER CODE BEGIN HRTIM1_Init 1 */

	/* USER CODE END HRTIM1_Init 1 */
	hhrtim1.Instance = HRTIM1;
	hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
	if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_DLLCalibrationStart(&hhrtim1, HRTIM_CALIBRATIONRATE_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_PollForDLLCalibration(&hhrtim1, 10) != HAL_OK)
	{
		Error_Handler();
	}
	pADCTriggerCfg.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_C;
	pADCTriggerCfg.Trigger = HRTIM_ADCTRIGGEREVENT13_TIMERC_CMP3;
	if (HAL_HRTIM_ADCTriggerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, &pADCTriggerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, 0x0) != HAL_OK)
	{
		Error_Handler();
	}
	pTimeBaseCfg.Period = HRTIM_PERIOD;
	pTimeBaseCfg.RepetitionCounter = 0x00;
	pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
	pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCtl.UpDownMode = HRTIM_TIMERUPDOWNMODE_UP;
	pTimerCtl.TrigHalf = HRTIM_TIMERTRIGHALF_DISABLED;
	pTimerCtl.GreaterCMP3 = HRTIM_TIMERGTCMP3_EQUAL;
	pTimerCtl.GreaterCMP1 = HRTIM_TIMERGTCMP1_EQUAL;
	pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCfg.InterruptRequests = HRTIM_TIM_IT_NONE;
	pTimerCfg.DMARequests = HRTIM_TIM_DMA_NONE;
	pTimerCfg.DMASrcAddress = 0x0000;
	pTimerCfg.DMADstAddress = 0x0000;
	pTimerCfg.DMASize = 0x1;
	pTimerCfg.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	pTimerCfg.InterleavedMode = HRTIM_INTERLEAVED_MODE_DISABLED;
	pTimerCfg.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	pTimerCfg.DACSynchro = HRTIM_DACSYNC_NONE;
	pTimerCfg.PreloadEnable = HRTIM_PRELOAD_DISABLED;
	pTimerCfg.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	pTimerCfg.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	pTimerCfg.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
	pTimerCfg.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	pTimerCfg.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	pTimerCfg.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
	pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	pTimerCfg.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
	pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	pTimerCfg.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	pTimerCfg.ReSyncUpdate = HRTIM_TIMERESYNC_UPDATE_UNCONDITIONAL;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCfg.DMASrcAddress = 0x0000;
	pTimerCfg.DMADstAddress = 0x0000;
	pTimerCfg.DMASize = 0x1;
	pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = 0;
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = HRTIM_PERIOD_MAX;
	pCompareCfg.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	pCompareCfg.AutoDelayedTimeout = 0x0000;

	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = 50;
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_3, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pDeadTimeCfg.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;
	pDeadTimeCfg.RisingValue = HRTIM_TC_RISING;
	pDeadTimeCfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
	pDeadTimeCfg.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
	pDeadTimeCfg.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
	pDeadTimeCfg.FallingValue = HRTIM_TC_FALLING;
	pDeadTimeCfg.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
	pDeadTimeCfg.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
	pDeadTimeCfg.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
	if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pDeadTimeCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pDeadTimeCfg.RisingValue = HRTIM_TD_RISING;
	if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pDeadTimeCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
	pOutputCfg.SetSource = HRTIM_OUTPUTSET_TIMPER;
	pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1|HRTIM_OUTPUTRESET_TIMCMP2;
	pOutputCfg.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
	pOutputCfg.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
	pOutputCfg.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
	pOutputCfg.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	pOutputCfg.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_OUTPUT_TC1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pOutputCfg.SetSource = HRTIM_OUTPUTSET_NONE;
	pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_NONE;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_OUTPUT_TC2, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = 0;
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = HRTIM_PERIOD_MAX;
	pCompareCfg.AutoDelayedTimeout = 0x0000;

	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pCompareCfg.CompareValue = 50;
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_3, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN HRTIM1_Init 2 */

	/* USER CODE END HRTIM1_Init 2 */
	HAL_HRTIM_MspPostInit(&hhrtim1);

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */
	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, BUCKBOOST_LOAD_1_Pin|BUCKBOOST_LOAD_2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BUCKBOOST_USBPD_EN_GPIO_Port, BUCKBOOST_USBPD_EN_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LED_LEFT_ORANGE_Pin|LED_UP_RED_Pin|LED_RIGHT_GREEN_Pin|GPO1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_DOWN_BLUE_GPIO_Port, LED_DOWN_BLUE_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : JOYSTICK_SELECT_Pin JOYSTICK_LEFT_Pin JOYSTICK_DOWN_Pin */
	GPIO_InitStruct.Pin = JOYSTICK_SELECT_Pin|JOYSTICK_LEFT_Pin|JOYSTICK_DOWN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : BUCKBOOST_LOAD_1_Pin BUCKBOOST_LOAD_2_Pin BUCKBOOST_USBPD_EN_Pin */
	GPIO_InitStruct.Pin = BUCKBOOST_LOAD_1_Pin|BUCKBOOST_LOAD_2_Pin|BUCKBOOST_USBPD_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_LEFT_ORANGE_Pin LED_UP_RED_Pin LED_RIGHT_GREEN_Pin */
	GPIO_InitStruct.Pin = LED_LEFT_ORANGE_Pin|LED_UP_RED_Pin|LED_RIGHT_GREEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : JOYSTICK_RIGHT_Pin JOYSTICK_UP_Pin */
	GPIO_InitStruct.Pin = JOYSTICK_RIGHT_Pin|JOYSTICK_UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_DOWN_BLUE_Pin */
	GPIO_InitStruct.Pin = LED_DOWN_BLUE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_DOWN_BLUE_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PC10 PC11 */
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : GPO1_Pin */
	GPIO_InitStruct.Pin = GPO1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPO1_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	/* JOYSTICK UP enables load transients load */
	if (GPIO_Pin == JOYSTICK_UP_Pin)
	{
		HAL_GPIO_TogglePin(LED_UP_RED_GPIO_Port, LED_UP_RED_Pin);
	}

	/* JOYSTICK DOWN disables load transients load */
	if (GPIO_Pin == JOYSTICK_DOWN_Pin)
	{
		HAL_GPIO_TogglePin(LED_DOWN_BLUE_GPIO_Port, LED_DOWN_BLUE_Pin);
	}

	/* JOYSTICK RIGHT increases the activated Resistors load */
	if (GPIO_Pin == JOYSTICK_RIGHT_Pin)
	{
		HAL_GPIO_TogglePin(LED_RIGHT_GREEN_GPIO_Port, LED_RIGHT_GREEN_Pin);
	}

	/* JOYSTICK LEFT decreases the activated Resistors load */
	if (GPIO_Pin == JOYSTICK_LEFT_Pin)
	{
		HAL_GPIO_TogglePin(LED_LEFT_ORANGE_GPIO_Port, LED_LEFT_ORANGE_Pin);
	}


}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

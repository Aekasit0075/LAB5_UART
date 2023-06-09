/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
uint8_t current = 1;
uint8_t before = 0;
GPIO_PinState Button1;

uint8_t RxBuffer[1];
uint8_t TxBuffer[200];
uint8_t modecontrol = 'x';
uint8_t checkstate = 1;
uint8_t LEDfrequency = 1;
int a = 0;
int round = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void CheckRising();
void DummyTask();
void UARTDMAConfig();
void mainmenu();
void detailscase0();
void detailscase1();
void functionstate();
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Button1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
  UARTDMAConfig();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  DummyTask();
	  functionstate();
	  if(round%2 == 1)
	  {
		  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin, RESET);
		  RxBuffer[0] = '\0';
//		  sprintf((char*)TxBuffer,"LED OFF %s\r\n", RxBuffer);
//		  HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
	  }
	  else if(round%2 == 0)
	  {
		  DummyTask();
		  RxBuffer[0] = '\0';
//		  sprintf((char*)TxBuffer,"LED ON %s\r\n", RxBuffer);
//		  HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
	  }
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);



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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void CheckRising()
{
	Button1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	current = Button1;

	if(before == 0 && current == 1)
	{
		uint8_t text1[] = "Status : UnPressed\r\n";
		HAL_UART_Transmit_DMA(&huart2, text1 , strlen((char*)text1));
		checkstate = 0;
		while(!checkstate)
		{
			HAL_Delay(1);
		}
	}
	else if(current == 0 && before == 1)
	{
		uint8_t text[] = "Status : Pressed\r\n";
		HAL_UART_Transmit_DMA(&huart2, text , strlen((char*)text));
		checkstate = 0;
		while(!checkstate)
		{
			HAL_Delay(1);
		}
	}
	before = current;
	HAL_Delay(1);
}

void mainmenu()
{
	uint8_t textmainmenu[] = 	"Please select the function:\r\n\r\n"
								"Press 0 : LED Control\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu , strlen((char*)textmainmenu));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
	uint8_t textmainmenu5[] = 	"---------------------------------------------------------\r\n"
								"Press 1 : Button Status\r\n\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu5, strlen((char*)textmainmenu5));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
}
void detailscase0()
{
	uint8_t textmainmenu1[] = 	"---------------------------------------------------------\r\n"
								"Press the Character:\r\n\r\n"
								"a : Speed Up +1Hz\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu1 , strlen((char*)textmainmenu1));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
	uint8_t textmainmenu2[] = "s : Speed Down -1Hz\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu2 , strlen((char*)textmainmenu2));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
	uint8_t textmainmenu3[] = "d : On/off\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu3 , strlen((char*)textmainmenu3));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
	uint8_t textmainmenu4[] = "Press 'x' to go to back menu\r\n"
							  "---------------------------------------------------------\r\n\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu4 , strlen((char*)textmainmenu4));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
}
void detailscase1()
{
	uint8_t textmainmenu6[] =	"Press 'x' to go to back menu\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu6 , strlen((char*)textmainmenu6));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
	uint8_t textmainmenu7[] = "If Button Press/UnPress Show Button Status:\r\n";
	HAL_UART_Transmit_DMA(&huart2, textmainmenu7 , strlen((char*)textmainmenu7));
	checkstate = 0;
	while(!checkstate)
	{
		HAL_Delay(1);
	}
}

void functionstate()
{
	switch(modecontrol)
	{
	case '0':
		if(RxBuffer[0] == 'a')
		{
			a += 1;
			LEDfrequency += 1;
			RxBuffer[0] = '\0';
			sprintf((char*)TxBuffer,"hz = %d\r\n", a);
			HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
		}
		else if(RxBuffer[0] == 's')
		{
			LEDfrequency -= 1;
			if(LEDfrequency < 0)
			{
				a -= 1;
				if(a < 0)
				{
					a = 0;
				}
				LEDfrequency -= 1;
				RxBuffer[0] = '\0';
				sprintf((char*)TxBuffer,"hz = %d\r\n", a);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}

		}
		else if(RxBuffer[0] == 'd')
		{
			round += 1;
		}
		else if(RxBuffer[0] == 'x')
		{
			modecontrol = 'x';
		}

		break;
	case '1':
		CheckRising();
		if(RxBuffer[0] == 'x')
		{
			modecontrol = 'x';
		}
		break;
	case 'x':
		mainmenu();
		RxBuffer[0] = '\0';
	default:
		modecontrol = RxBuffer[0];
		if(modecontrol == '1')
		{
			detailscase1();
		}
		else if(modecontrol == '0')
		{
			detailscase0();
		}

	}
	RxBuffer[0]= 0;
}

void DummyTask()
{
	static uint32_t timestamp=0;
	if (HAL_GetTick()>=timestamp)
	{
		timestamp = HAL_GetTick()+(1000/(2.0*LEDfrequency));
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	}
}

void UARTDMAConfig()
{
	//start UART in DMA Mode
	HAL_UART_Receive_DMA(&huart2, RxBuffer, 1);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
	{
		//(for string only) Add string stop symbol \0 to end string
		RxBuffer[1] = '\0';
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
		{
		checkstate = 1;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

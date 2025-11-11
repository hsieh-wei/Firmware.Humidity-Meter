/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "button.h"
#include "pc_link.h"
#include "sht30.h"
#include "lcd.h"
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
/* USER CODE BEGIN PV */
// led handle
static LED_HANDLE s_led_handle ={
  .gpiox = GPIOA,
  .gpio_pin = GPIO_PIN_7,
};
// button handle
static BUTTON_HANDLE s_button_handle ={
  .gpiox = GPIOE,
  .gpio_pin = GPIO_PIN_3,
};
// pc_link handle
extern PC_LINK_HANDLE g_pc_link_handle;
// sht30 handle
static SHT30_HANDLE s_sht30_handle = {
    .hi2c = &hi2c1,
};
// sht30 handle
static LCD_HANDLE s_lcd_handle = {
    .hspi = &hspi1,
    .rst  = {GPIOC, GPIO_PIN_0},
    .dc   = {GPIOC, GPIO_PIN_1},
    .cs   = {GPIOC, GPIO_PIN_2},
    .blk  = {GPIOA, GPIO_PIN_8},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // Inject gpioa into led handle
  // Inject gpioa into button handle
  // Inject huart1 and buffer into pc_link handle,then start DMA ReceiveToIdle
  g_pc_link_handle.huart = &huart1;
  (void)pc_link_init(&g_pc_link_handle);
  (void)pc_link_rx_dma(&g_pc_link_handle);
  // Inject hi2c1 into sht30 handle
  (void)sht30_init(&s_sht30_handle);
  // Inject hi2c1 into sht30 handle
  (void)lcd_init(&s_lcd_handle);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  (void)lcd_fill_screen(&s_lcd_handle, LCD_COLOR_WHITE);
  (void)lcd_print_icon(&s_lcd_handle, &LCD_Thermometer_30X30, 10, 45, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, 'T', &LCD_Font_11x18, 45, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, 'e', &LCD_Font_11x18, 61, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, 'm', &LCD_Font_11x18, 77, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, 'p', &LCD_Font_11x18, 93, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, ':', &LCD_Font_11x18, 109, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, '2', &LCD_Font_11x18, 125, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  (void)lcd_print_font(&s_lcd_handle, '7', &LCD_Font_11x18, 141, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
  while (1){
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// HAL uart tx complete send
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  pc_link_irq_tx_cplt(&g_pc_link_handle, huart);
}

// HAL uart rx idle or full buffer
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  pc_link_irq_rx_event(&g_pc_link_handle, huart, Size);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  sht30_irq_tx_cplt(&s_sht30_handle, hi2c);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  sht30_irq_rx_cplt(&s_sht30_handle, hi2c);
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
#ifdef USE_FULL_ASSERT
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

# Project Setting

## Step 1：建立 STM32CubeMX 專案

| 項目     | 操作路徑與說明                                                                      |
| ------ | ---------------------------------------------------------------------------- |
| MCU 選擇 | Start New Project → `MCU Selector` → 搜尋 `STM32F407IGH6` → 點擊 `Start Project` |
---

## Step 2：Pinout & Configuration

### 2.1 System Core

#### System 
| 腳位   | 功能設定 | 外接模組腳位
| ---- | ---- | ---- 
| SWDIO  | ST-Link 燒錄 |ST-Link SWDIO
| SWCLK  | ST-Link 燒錄 |ST-Link SWCLK
| NRST  | ST-Link 燒錄 |ST-Link RST
| GND  | ST-Link 燒錄 |ST-Link GND
| 3V3  | ST-Link 燒錄 |ST-Link 3.3 V

| 模組  | 選項設定
| --- | ---- |
| SYS | Serial Wire (SWD)                           
| RCC | HSE = Crystal/Ceramic Resonator` <br> LSE 不勾選 

#### GPIO 

| 腳位   | 功能設定| GPIO Mode| Pull 設定| 外接模組腳位
| ---- | ---- | ---- |---- |----
| PA6  | LED | Output Push Pull| No pull-up and pull-down| X
| PA7  | LED | Output Push Pull| No pull-up and pull-down| X
| PA8  | LCD 背光恆亮 | Output Push Pull| No pull-up and pull-down| LCD BLK
| PC0  | LCD 初始化  | Output Push Pull| No pull-up and pull-down| LCD RST
| PC1  | LCD 數據/指令控制 | Output Push Pull| No pull-up and pull-down| LCD DC
| PC2  | LCD Slave Select| Output Push Pull| No pull-up and pull-down| LCD CS
| PE3  | BTN | External Interrupt| pull-up|X
| PE4  | BTN | External Interrupt| pull-up|X

### 2.2 Analog
NULL

### 2.3 Timers

#### TIM6

| 腳位   | 功能設定| 外接模組腳位
| ---- |---- |----
| X | 系統啟動 timestamp | X

> 在 APB1 clock上，，目前頻率設為84MHZ，參考reference manual RCC register

| 項1  | 項2 | 項3| 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Mode  | X | X | X | Activated
| Configuration  | Parameter Setting | Counter Setting | Prescaler | 8400-1
| Configuration  | Parameter Setting | Counter Setting | Counter Peroid | 10000-1 


#### TIM1
先不設置
| 腳位   | 功能設定| 外接模組腳位
| ---- |---- |----
| PA8 | LCD 背光調亮度 |LCD BLK

| 項1  | 項2 | 項3| 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Mode  | Parameter Setting |Clock Source | X |Internal Clock
| Mode  | Parameter Setting | Channel 1 | X |PWM Generation CH1
| Configuration  | Parameter Setting | Counter Setting | Prescaler | 84 
| Configuration  | Parameter Setting | Counter Setting | Counter Peroid | 100-1 
| Configuration | Parameter Setting |  PWM Generation Channel 1 | Mode | PWM Mode 1
| Configuration | Parameter Setting |  PWM Generation Channel 1 | Pulse | 50
| Configuration | Parameter Setting |  PWM Generation Channel 1 | Output Compare preload | Enable
| Configuration | Parameter Setting |  PWM Generation Channel 1 | Fast Mode | Disable


### 2.4 Connectivity
#### I2C1
| 腳位   | 功能設定| 外接模組腳位
| ---- |---- |----
| PB6 | I2C1_SCL | 溫溼度計 SCL
| PB7 | I2C1_SDA | 溫溼度計 SDA

| 項1  | 項2 | 項3 | 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Mode  | I2C | X | X | I2C
| Configuration | Parameter Setting | Master Features | I2C Clock Speed | 100000 
| Configuration | Parameter Setting | Slave Features | Primary Address Length selsction | 7-bit 
| Configuration |  DMA Setting | I2C1_Rx |DMA1 Stream 0 | Periphrel To Memory 
| Configuration |  DMA Setting | I2C1_Tx |DMA1 Stream 6 | Memory To Periphrel 
| Configuration | NVIC | X | X | 全部啟用 

#### SPI1
| 腳位   | 功能設定| 外接模組腳位
| ---- |---- |----
| PA5 | SPI1_SCK | LCD SCL
| PB4 | SPI1_MISO | X
| PB5 | SPI1_MOSI | LCD SDA

| 項1  | 項2 | 項3 | 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Mode  | Mode | X | X | Full-Duplex Master
| Configuration | Parameter Setting | Basic Paremeters | Data Size | 100000 
| Configuration | Parameter Setting | Clock Paremeters | Prescaler | 4 
| Configuration | Parameter Setting | Clock Paremeters | CPOL | Low
| Configuration | Parameter Setting | Clock Paremeters | CPOL | 1 Edge  
| Configuration | NVIC | X | X | 全部啟用 

#### USART1
| 腳位   | 功能設定| 外接模組腳位
| ---- |---- |----
| PA9 | USART1_Tx | USB Uart RXD
| PA10 | USART1_Rx | USB Uart TXD

| 項1  | 項2 | 項3 | 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Mode  | Mode | X | X | Asynchronous
| Configuration | Parameter Setting | Basic Paremeters | Baud Rate | 115200 Bits/s 
| Configuration | Parameter Setting | Basic Paremeters | Word Length | 115200 Bits/s 
| Configuration | Parameter Setting | Basic Paremeters | Parity | None
| Configuration | Parameter Setting | Basic Paremeters | Stop Bits| 1
| Configuration |  DMA Setting | USART1_Rx |DMA1 Stream 2 | Periphrel To Memory 
| Configuration |  DMA Setting | USART1_Tx |DMA1 Stream 7 | Memory To Periphrel 
| Configuration | NVIC | X | X | 全部啟用 

---

## Step 3：Clock Configuration 設定

| 項目            | 設定值              | 說明                             |
| ------------- | ---------------- | ------------------------------ |
| PLL Source    | HSE              | 使用外部 8MHz 晶振                   |
| HSE Frequency | 8 MHz            | 外部晶振頻率                         |
| PLLM          | 8                | VCO Input = 25 / 25 = 1 MHz      |
| PLLN          | 336              | VCO Output = 1 × 336 = 336 MHz |
| PLLP          | 2                | SYSCLK = 336 / 2 = 168 MHz     |
| PLLQ          | 7                | USB Clock = 336 / 7 = 48 MHz   |
| AHB Prescaler | 1                | HCLK = 168 MHz                 |
| APB1          | 4                | PCLK1 = 42 MHz                 |
| APB2          | 2                | PCLK2 = 84 MHz                 |
| 系統主頻率         | SYSCLK = 168 MHz | STM32F407 可支援之最大頻率             |

---

## Step 4：Project Manager 設定

### 4.1 Project Settings
| 項1  | 項2 | 項3 | 項4 | 設置
| ---- | ---- | ---- | ---- | ---- 
| Project  | Project Settings | Project Name | X | 自訂
| Project  | Project Settings | Project Location | X | 自訂
| Project  | Project Settings | Application Structure | X | Advanced
| Project  | Project Settings | Toolchain Folder Location | X | 自訂
| Project  | Project Settings | Toolchain / IDE | X | CMake
| Project  | Thread-safe Settings | X | X | Enable multi-threaded support
| Project  | Thread-safe Settings | Thread-safe Locking Strategy | X | Default - Mapping suitable strategy for RTOS selection
| Project  | MCU and Firmware Package | MCU Reference | X | STM32F407VETx
| Code Generator  | STM32Cube MCU packages and embedded software packs | X | X | Copy only the necessary library files
| Code Generator  | Generated files | X | X | Generating peripheral initialization as a pair of '.c / .h ' files per peripheral
| Code Generator  | Generated files | X | X | Keep User Code when re-generating
| Code Generator  | Generated files | X | X | delete previously  generated files when re-generated

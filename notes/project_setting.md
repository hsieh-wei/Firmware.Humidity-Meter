# Project Setting

## Step 1：建立 STM32CubeMX 專案

| 項目     | 操作路徑與說明                                                                      |
| ------ | ---------------------------------------------------------------------------- |
| MCU 選擇 | Start New Project → `MCU Selector` → 搜尋 `STM32F407VET6` → 點擊 `Start Project` |
---

## Step 2：Pinout & Configuration

### 2.1 System Core 主要設定# 5_uart_dma

## Step 1：建立 STM32CubeMX 專案

| 項目     | 操作路徑與說明                                                                      |
| ------ | ---------------------------------------------------------------------------- |
| MCU 選擇 | Start New Project → `MCU Selector` → 搜尋 `STM32F407IGH6` → 點擊 `Start Project` |
---

## Step 2：Pinout & Configuration

### 2.1 System Core 主要設定

| 模組  | 選項設定                                             | 功能說明                    | 為何選此設定               |
| --- | ------------------------------------------------ | ----------------------- | -------------------- |
| SYS | `Serial Wire (SWD)`                              | 提供 ULINKpro Debug 與燒錄通道 | SWD 僅需 2 線，節省腳位，支援廣泛 |
| RCC | `HSE = Crystal/Ceramic Resonator` <br> `LSE` 不勾選 | 啟用外部高精度振盪器              | HSE 精準穩定，適合實際硬體時鐘來源  |
| NVIC | `USART3 Global Interrupt`勾選 (Connectivityg 設定後才能啟用) | 致能 USART3 例外事件              | 使用 USART3 中斷 |

### 2.2 System Core GPIO 腳位設定
| 腳位   | 功能設定    | GPIO Mode                               | Pull 設定 | 備註                                |
| ---- | ------- | --------------------------------------- | ------- | --------------------------------- |
| PA6  | LED   | `GPIO_Output`                           | 無       | 控制用 LED
| PA7  | LED         | GPIO\_Output |    無     |                 控制用 LED          |               控制用 LED          |
### 2.3 Connectivity

* 開啟 USART3 ， 設置成 Asynchronous
    * 會自動開啟 `PB10` 、 `PB11`
    * Baud Rate = 115200 bps
    * Word length  = 8 but bits
    * Parity = None
    * Stop Bits = 1 bit
* 選取 USART3 中 DMA Settings ，按下 ADD 並且新增 USART3 RX 、 USART3 TX

---

## Step 3：Clock Configuration 設定

| 項目            | 設定值              | 說明                             |
| ------------- | ---------------- | ------------------------------ |
| PLL Source    | HSE              | 使用外部 8MHz 晶振                   |
| HSE Frequency | 25 MHz            | 外部晶振頻率                         |
| PLLM          | 25                | VCO Input = 25 / 25 = 1 MHz      |
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

| 項目                        | 設定                         |
| ------------------------- | -------------------------- |
| Project Name              | 自訂                         |
| Project Location          | 自訂                         |
| Application Structure     | Advanced                         |
| Toolchain Folder Location | 自訂                         |
| Toolchain / IDE           | STM32CubeIDE |

### 4.2 Thread-safe Settings

| 項目                                           | 操作 |
| -------------------------------------------- | ---- |
| Enable multi-threaded support                | 勾選    |
| Mapping suitable strategy for RTOS selection | 下拉選單    |

### 4.3 MCU and Firmware Package

| 項目            | 說明              | 備註|
| ------------- | --------------- |  --------------- |
| MCU Reference | `STM32F407IGHx` | 其他先不動 |

### 4.4 Code Generator
- Generated files 
    - 勾選 Generating peripheral initialization as a pair of '.c / .h ' files per peripheral
    - 勾選 Keep User Code when re-generating
    - 勾選 delete previously  generated files when re-generated

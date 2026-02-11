# 硬體接線表 (Hardware Wiring Guide)

## 1. 燒錄與除錯 (ST-Link V2)

用於程式燒錄與 Debug。

| STM32 腳位 | 功能說明 | 連接至 ST-Link | 備註 |
| --- | --- | --- | --- |
| **PA13** | SWDIO | **SWDIO** | 資料線 |
| **PA14** | SWCLK | **SWCLK** | 時鐘線 |
| **3V3** | Power | **3.3V** | 供電 |
| **GND** | Ground | **GND** | 共地 |

## 2. 溫濕度感測器 (SHT30)

使用 I2C1 通訊介面。

| STM32 腳位 | 功能說明 | 連接至 SHT30 模組 | 備註 |
| --- | --- | --- | --- |
| **PB6** | I2C1_SCL | **SCL** | 時鐘線 |
| **PB7** | I2C1_SDA | **SDA** | 數據線 |
| **3V3** | Power | **VCC / VIN** |  |
| **GND** | Ground | **GND** |  |

## 3. 彩色顯示螢幕 (ST7735 1.8" LCD)

使用 SPI1 通訊介面與 GPIO 控制。

| STM32 腳位 | 功能說明 | 連接至 ST7735 模組 | 備註 |
| --- | --- | --- | --- |
| **PA5** | SPI1_SCK | **SCK / SCL** | SPI 時鐘 |
| **PB5** | SPI1_MOSI | **SDA / MOSI** | SPI 數據 (主出從入) |
| **PC0** | GPIO Output | **RES / RST** | 螢幕復位腳 |
| **PC1** | GPIO Output | **DC / A0** | 數據/指令選擇腳 |
| **PC2** | GPIO Output | **CS** | 片選腳 (Chip Select) |
| **PA0** | TIM2_CH1 | **BLK / LED** | 背光控制 (PWM) |
| **3V3** | Power | **VCC** |  |
| **GND** | Ground | **GND** |  |

## 4. 電腦通訊 (USB to UART / FT232)

使用 USART1 與 PC 進行指令傳輸與 Log 監控。
**注意：RX 與 TX 必須交叉連接。**

| STM32 腳位 | 功能說明 | 連接至 USB 轉 TTL 模組 | 備註 |
| --- | --- | --- | --- |
| **PA9** | USART1_TX | **RXD** | MCU 發送 -> PC 接收 |
| **PA10** | USART1_RX | **TXD** | MCU 接收 <- PC 發送 |
| **GND** | Ground | **GND** | 務必共地 |

## 5. 人機介面 (On-board IO)

開發板上的按鈕與 LED

| STM32 腳位 | 類型 | 功能定義
| --- | --- | --- 
| **PA6** | LED | **LED D2** (系統狀態/警示) 
| **PA7** | LED | **LED D3** (系統狀態/警示) 
| **PE3** | Button | **Key 1** (K1) - 調整亮度
| **PE4** | Button | **Key 0** (K0) - 切換模式 
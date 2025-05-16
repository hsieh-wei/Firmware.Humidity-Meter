# STM32 嵌入式韌體學習規劃

## 課程目標

本學習專案旨在學習嵌入式韌體工程師能力。透過 STM32 MCU 與相關軟體工具，熟練掌握以下技能：
* CMSIS-Core 架構及應用
* STM32 HAL / LL 驅動開發
* 獨立閱讀 datasheet/spec，實作常見通訊協定（如 UART、I2C、SPI 等）
* 理解 ARM Cortex-M 系列微控制器架構與運作原理
* 熟悉開發工具與環境 (CMake, GCC, GDB, STM32CubeMX 等)

## 購買清單

| 品項名稱                      | 類型         | 數量     | 用途/功能說明                    |
| ------------------------- | ---------- | -------- | -------------------------- |
| **STM32F103VET6 開發板** | MCU     | 1        | CAN主控、濕度顯示、蜂鳴警報  
| **STM32F103C8T6 開發板**     | MCU   | 1        | CAN 感測從板，讀取資料並上傳主板           |
| **SHT30 模組**      | Input   | 1        | 透過 I2C 溫濕度感測主模組                   |
| **ST7735 模組**       | Output         | 1        | 透過SPI顯示溫濕度、ADC 數值等資料            |
| **無源蜂鳴器**                 | Output  | 1        | 透過Timer、PWM變頻警報、狀態提示音                 |
| **TJA1050 CAN 模組**        |  通訊模組  | 2        | STM32 CAN 通訊，主從各 1        |
| **FT232 USB to UART 模組**  |  通訊模組  | 1        | 電腦下指令、接收 UART 訊息           |
| **Mini USB 線**            | 線材    | 1        | 給 STM32F407VET6 供電與 USB 通訊 |
| **Micro USB 線**            | 線材    | 1        | 給 STM32F103C8T6 供電與 USB 通訊 |
| **杜邦線 母母、公母、公公**           | 線材         | 10    | 各模組與開發板之間接線                |
| **ST-Link V2 燒錄器**        | 開發工具       | 1        | 燒錄 F103 系列開發板與 Debug 用            |
| **電阻包**            | 零件包        | 1       | 常見阻值，調試與擴充用，建議120Ω(CAN Bus)、220Ω/330Ω(LED、蜂鳴器)、10kΩ (按鈕、ADC pull-up/down )             |




## 選用工具

| 項目          | 工具名稱                                                 |
| ----------- | ---------------------------------------------------- |
| 微控制器開發板     | STM32F407VET6(mini USB) 、 STM32f103C8T6 (micro USB)                                       |
| IDE 與編輯器    | VSCode                                               |
| VSCode 擴充套件 | C/C++, CMake Tools, Cortex-Debug, Code Spell Checker |
| 專案生成工具      | STM32CubeMX                                          |
| 編譯工具        | GCC (arm-none-eabi-gcc)                              |
| 建構工具        | CMake                                                |
| 燒錄       | ST-Link, OpenOCD                                 |
| 除錯       | GDB                                  |
| 開發環境        | Ubuntu 22.04                                     |

---
## 專案練習
| 周邊功能              | 專案應用                   | 說明                      |
| ----------------- | ---------------------- | ----------------------- |
| **I2C**  | 讀取室內環境的溫度、濕度、氣壓        | 感測器模組，定時量測              |
| **SPI**  | 實時顯示溫度與濕度              | 顯示感測資料，提升互動性            |
| **UART**  | 透過 PC 設定參數，例如資料上報頻率、閾值 | 與電腦雙向通訊，接收/送出指令         |
| **CAN** | 將資料傳送到另一個節點（或接收）       | CAN 用於多板間通信，類似 RS485 概念 |
| **Timer / PWM**   | 蜂鳴提示| 用 Timer 定時產生中斷 + PWM 調音 |
| **Watchdog**      | 確保系統不會死機               | 若系統 Hang 住就自動重啟         |


## 課程規劃

| 課程順序 | 課程主題                          | 搭配參考影片                                                                         | 應掌握的概念                       |
| ---- | ----------------------------- | ------------------------------------------------------------------------------ | ---------------------------- |
| 1    | ARM Cortex-M 核心架構與 CMSIS-Core | arm cortex-m, arm-cmsis core                                                   | ARM Cortex-M 架構、CMSIS 核心功能   |
| 2    | STM32 專案建立與程式結構               | bare metal linking, console vs semihost                                        | 啟動流程、鏈結腳本、半主機輸出除錯            |
| 4    | STM32 開發環境建置與工具熟悉             | ubuntu, gpio, 示波器                                                              | Ubuntu 開發環境、GPIO 操作、基本示波器使用  |
| 5    | 中斷系統及異常處理機制                   | arm cortex-m interrupt, interrupt handler, thread mode, system crash breakdown | 中斷管理、異常處理、系統模式切換、除錯技巧        |
| 6    | UART 通訊協定實作                   | uart                                                                           | UART 原理、資料收發、中斷與 DMA 應用      |
| 7    | Timer 計時器應用                   | timer, pwm                                                                     | 計時器設定、PWM 產生                 |
| 9    | SPI 通訊協定實作                    | spi                                                                            | SPI 主從模式、暫存器控制、中斷/DMA傳輸      |
| 10   | I2C 通訊協定實作                    | i2c                                                                            | I2C 匯流排原理、地址管理、中斷傳輸          |
| 11   | CAN Bus 應用與實作                 | can bus                                                                        | CAN 原理與應用實作、報文格式與錯誤處理        |
| 12   | USB 協定基礎                      | usb                                                                            | USB 原理、協定分類、STM32 USB 應用範例   |
| 13   | 低功耗設計與 RTC/WDT 應用             | 電源管理, rtc + wdt                                                                | 電源管理模式、RTC實時時鐘、看門狗計時器        |
| 14   | Bootloader 實作                 | bootloader                                                                     | 啟動程式實作、韌體更新機制                |
| 15   | RTOS 入門                       | rtos                                                                           | 即時作業系統基本概念、FreeRTOS 入門應用     |
| 16   | CMSIS                       | arm-cmsis                                                                           | 跨平台SDK與實作LL、HAL    |


---

## 參考課程

| 課程主題                     | 相關影片                               | 備註                |
| ------------------------ | ---------------------------------- | ----------------- |
| ARM Cortex-M | arm cortex-m instruction, fpu | 優化與高效能運算，視需求學習    |
| 軟體開發基礎                   | iso-c, unicode, fixed point number | ISO-C 語言標準與特殊數值運算 |
| 硬體輔助工具                   | swo, crc, rng, ecc                 | 特殊應用工具，視專案需求補充    |
|ADC/DAC 模擬訊號處理                | adc/dac                                                                        | ADC、DAC 基本原理與數據轉換應用          |
---

## 專案建議與學習方式

* 每個課程建議先觀看影片搭配 STM32CubeMX 建立初始專案，再嘗試從 LL/HAL 層次手動實作，以深入理解底層機制。
* 遇到問題時善用 datasheet、參考手冊 (Reference Manual) 及網路資源。
* 每個課程完成後，建議整理筆記或 README 記錄所學。
* 熟悉後可加入 CI/CD 流程與 Git 版控訓練，提升實務能力。
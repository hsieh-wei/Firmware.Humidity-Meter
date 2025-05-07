# STM32 嵌入式韌體學習規劃

## 課程目標

本學習專案旨在學習嵌入式韌體工程師能力。透過 STM32 MCU 與相關軟體工具，熟練掌握以下技能：
* CMSIS-Core 架構及應用
* STM32 HAL / LL 驅動開發
* 獨立閱讀 datasheet/spec，實作常見通訊協定（如 UART、I2C、SPI 等）
* 理解 ARM Cortex-M 系列微控制器架構與運作原理
* 熟悉開發工具與環境 (CMake, GCC, GDB, STM32CubeMX 等)

## 選用工具

| 項目          | 工具名稱                                                 |
| ----------- | ---------------------------------------------------- |
| 微控制器開發板     | STM32F407VET6                                        |
| IDE 與編輯器    | VSCode                                               |
| VSCode 擴充套件 | C/C++, CMake Tools, Cortex-Debug, Code Spell Checker |
| 專案生成工具      | STM32CubeMX                                          |
| 編譯工具        | GCC (arm-none-eabi-gcc)                              |
| 建構工具        | CMake                                                |
| 燒錄與除錯       | ST-Link, GDB                                         |
| 開發環境        | Ubuntu                                               |

---

## 課程規劃

| 課程順序 | 課程主題                          | 搭配參考影片                                                                         | 應掌握的概念                       |
| ---- | ----------------------------- | ------------------------------------------------------------------------------ | ---------------------------- |
| 1    | ARM Cortex-M 核心架構與 CMSIS-Core | arm cortex-m, arm-cmsis core                                                   | ARM Cortex-M 架構、CMSIS 核心功能   |
| 2    | STM32 專案建立與程式結構               | bare metal linking, console vs semihost                                        | 啟動流程、鏈結腳本、半主機輸出除錯            |
| 4    | STM32 開發環境建置與工具熟悉             | ubuntu, gpio, 示波器                                                              | Ubuntu 開發環境、GPIO 操作、基本示波器使用  |
| 5    | 中斷系統及異常處理機制                   | arm cortex-m interrupt, interrupt handler, thread mode, system crash breakdown | 中斷管理、異常處理、系統模式切換、除錯技巧        |
| 6    | UART 通訊協定實作                   | uart                                                                           | UART 原理、資料收發、中斷與 DMA 應用      |
| 7    | Timer 計時器應用                   | timer, pwm                                                                     | 計時器設定、PWM 產生                 |
| 8    | ADC/DAC 模擬訊號處理                | adc/dac                                                                        | ADC、DAC 基本原理與數據轉換應用          |
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
---

## 專案建議與學習方式

* 每個課程建議先觀看影片搭配 STM32CubeMX 建立初始專案，再嘗試從 LL/HAL 層次手動實作，以深入理解底層機制。
* 遇到問題時善用 datasheet、參考手冊 (Reference Manual) 及網路資源。
* 每個課程完成後，建議整理筆記或 README 記錄所學。
* 熟悉後可加入 CI/CD 流程與 Git 版控訓練，提升實務能力。
# STM32 嵌入式韌體學習規劃

## 一、課程目標

本學習專案旨在養成 STM32 嵌入式韌體工程師的核心能力，熟練使用 ARM Cortex-M 系列微控制器與其開發工具，並能獨立設計並實作感測、顯示與通訊等常見應用。重點技能包含：

* 理解 ARM Cortex-M 架構與 CMSIS-Core 應用
* 熟悉 STM32 HAL / LL 驅動開發流程
* 具備閱讀 datasheet 與 reference manual 能力，實作 UART、I2C、SPI、CAN 等通訊協定
* 熟悉開發工具鏈 (CMake, GCC, GDB, STM32CubeMX 等)
* 能獨立設計嵌入式應用專案並進行除錯與優化

## 二、硬體與開發工具清單

### 1. 購買硬體元件

| 品項名稱                 | 類型   | 數量   | 功能說明                             |
| -------------------- | ---- | ---- | -------------------------------- |
| STM32F103VET6 開發板    | MCU  | 1    | CAN 主控、濕度顯示、蜂鳴警報                 |
| STM32F103C8T6 開發板    | MCU  | 1    | CAN 感測從板，讀取資料並上傳主板               |
| SHT30 模組             | 輸入裝置 | 1    | I2C 溫濕度感測器                       |
| ST7735 模組            | 輸出裝置 | 1    | SPI 顯示模組，顯示溫濕度與 ADC 數值           |
| 無源蜂鳴器                | 輸出裝置 | 1    | Timer 與 PWM 控制警報音                |
| TJA1050 CAN 模組       | 通訊模組 | 2    | CAN 傳輸，主從各 1                     |
| FT232 USB to UART 模組 | 通訊模組 | 1    | 與電腦進行 UART 通訊                    |
| Mini USB 線           | 線材   | 1    | STM32F407 供電與 USB 傳輸             |
| Micro USB 線          | 線材   | 1    | STM32F103 供電與 USB 傳輸             |
| 杜邦線（母母、公母、公公）        | 線材   | 約 10 | 模組與開發板之間連接用                      |
| ST-Link V2 燒錄器       | 開發工具 | 1    | F103 系列燒錄與 Debug                 |
| 電阻包                  | 零件包  | 1    | 包含 120Ω、220Ω、10kΩ 等，供 CAN、蜂鳴器等用途 |

### 2. 選用軟體工具

| 類別   | 工具名稱                                                 |
| ---- | ---------------------------------------------------- |
| IDE  | VSCode                                               |
| 擴充套件 | C/C++, CMake Tools, Cortex-Debug, Code Spell Checker |
| 專案生成 | STM32CubeMX                                          |
| 編譯工具 | GCC (arm-none-eabi-gcc)                              |
| 建構工具 | CMake                                                |
| 燒錄   | ST-Link, OpenOCD                                     |
| 除錯   | GDB                                                  |
| 環境   | Ubuntu 22.04                                         |

## 三、專案實作與應用

| 周邊功能        | 應用內容      | 技術說明                        |
| ----------- | --------- | --------------------------- |
| I2C         | 感測溫濕度資料   | 定時讀取 SHT30 感測器              |
| SPI         | 顯示感測資料    | ST7735 螢幕即時顯示溫濕度、ADC 數值等資訊  |
| UART        | 與 PC 通訊   | 參數設定、回報資料頻率與閾值，支援中斷與 DMA 傳輸 |
| CAN         | 板與板之間資料傳輸 | 建構主從架構進行資料同步，並處理錯誤與擁塞管理     |
| Timer / PWM | 警報控制      | 使用 Timer 產生中斷觸發蜂鳴器 PWM 發聲   |
| Watchdog    | 系統穩定與自動復原 | 防止系統死機，當系統 hang 住時自動重啟      |

## 四、課程規劃與順序

| 課程順序 | 主題                        | 搭配影片關鍵字                                   | 核心概念                       |
| ---- | ------------------------- | ----------------------------------------- | -------------------------- |
| 1    | ARM Cortex-M 與 CMSIS 核心架構 | arm cortex-m, arm-cmsis core              | 微控制器結構、CMSIS 核心庫使用         |
| 2    | STM32 專案架構與啟動流程           | bare metal linking, console vs semihost   | 啟動程序、鏈結腳本、半主機輸出除錯          |
| 3    | 開發環境建置與 GPIO 初探           | ubuntu, gpio, 示波器                         | Ubuntu 環境設定、GPIO 輸出、基本測試工具 |
| 4    | 中斷系統與異常處理機制               | cortex-m interrupt, handler, system crash | 中斷管理、異常排除、模式切換、除錯技巧        |
| 5    | UART 通訊協定實作               | uart                                      | UART 原理、傳輸流程、中斷/DMA 應用     |
| 6    | Timer 計時器與 PWM            | timer, pwm                                | 時間控制、PWM 輸出應用              |
| 7    | SPI 通訊協定實作                | spi                                       | 主從同步、資料傳輸、中斷與 DMA 機制       |
| 8    | I2C 通訊協定實作                | i2c                                       | 匯流排架構、主從尋址、傳輸方式            |
| 9    | CAN 通訊協定應用                | can bus                                   | 報文格式、節點通訊、錯誤處理             |
| 10   | USB 協定入門                  | usb                                       | USB 類型、協定分類、STM32 USB 範例應用 |
| 11   | RTC 與 Watchdog 應用         | rtc, wdt                                  | RTC 實時時鐘、系統穩定性             |
| 12   | Bootloader 與韌體更新          | bootloader                                | 韌體更新流程、自訂啟動程式              |
| 13   | RTOS 入門                   | rtos                                      | 任務排程、資源管理、FreeRTOS 初步實作    |
| 14   | CMSIS 延伸應用與 HAL/LL 實作層理解  | cmsis, stm32 hal/ll                       | HAL 封裝結構與 LL 操作            |

## 五、補充與進階參考課程

| 主題              | 關鍵詞                         | 備註               |
| --------------- | --------------------------- | ---------------- |
| ARM Cortex-M 優化 | cortex-m instruction, fpu   | 高效能運算與硬體乘法器使用    |
| 軟體基礎概念          | iso-c, unicode, fixed point | 精確資料表示與語言標準      |
| 硬體輔助模組          | swo, crc, rng, ecc          | CRC 檢查、隨機數與錯誤校驗  |
| 模擬訊號處理          | adc/dac                     | ADC 轉換與 DAC 輸出應用 |

## 六、學習建議

* 觀看教學影片後，搭配 STM32CubeMX 建立初始專案並手動撰寫程式加深理解。
* 撰寫筆記或 README 紀錄每一課所學，加強記憶與整理能力。
* 善用 datasheet 與 reference manual 查詢功能細節，並於實作中驗證。
* 專案進階後可嘗試引入 Git 與 CI/CD 概念，培養專業團隊開發能力。
* 請持續實作並反覆驗證，唯有實際操作，方能內化知識與提升解決問題的能力。

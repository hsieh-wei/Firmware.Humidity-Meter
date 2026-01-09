# STM32 溫濕度計

## 一、專案目標

本學習專案旨在養成 STM32 嵌入式韌體工程師的核心能力，熟練使用 ARM Cortex-M 系列微控制器與其開發工具，並能獨立設計並實作感測、顯示與通訊等常見應用。重點技能包含：

* 理解 ARM Cortex-M 架構
* 熟悉 STM32 驅動開發流程
* 掌握即時作業系統 (RTOS) 核心機制：熟悉 FreeRTOS 移植與配置，理解 Task 排程原理，並能運用 Semaphore、Mutex、Queue 解決多工同步與資源競爭問題
* 具備閱讀 datasheet 、reference manual 、 HAL Driver 能力，應用 UART、I2C、SPI 等通訊協定
* 熟悉開發工具鏈 (CMake, GCC, STM32CubeMX 等)
* 能獨立設計嵌入式應用專案並進行除錯與優化

## 二、硬體與開發工具清單

### 1. 購買硬體元件

| 品項名稱 | 類型 | 數量 | 功能說明 |
| --- | --- | --- | --- |
| STM32F407VET6 開發板 | MCU  | 1 | 主板 |
| SHT30 模組 | 輸入裝置 | 1 | I2C 溫濕度感測器 |
| ST7735 模組 | 輸出裝置 | 1 | SPI 顯示模組，顯示溫濕度與 ADC 數值 |
| FT232 USB to UART 模組 | 通訊模組 | 1    | 與電腦進行 UART 通訊 |
| Mini USB 線 | 線材 | 1 | STM32F407 供電與 USB 傳輸 |
| 杜邦線(母母)| 線材 | 約 20 | 模組與開發板之間連接用  |
| ST-Link V2 燒錄器 | 開發工具 | 1 | F103 系列燒錄與 Debug |

### 2. 選用軟體工具

| 類別 | 工具名稱 |
| ---- | --- |
| IDE  | VSCode |
| VSCode 擴充套件 | C/C++, CMake Tools, Cortex-Debug, STM32CubeIDE for Visual Studio Code |
| 專案生成 | STM32CubeMX 6.14.1 |
| 編譯工具 | GCC (arm-none-eabi-gcc) |
| 建構工具 | CMake |
| 燒錄   | ST-Link |
| 除錯   | VSCode Debug Tool |
|嵌入式作業系統| FreeRTOS |
| 程式碼開發作業系統   | Windows 11 |

## 三、專案實作與應用

| 周邊功能 | 應用內容 | 技術說明 |
| --- | --- | --- |
| GPIO        | 按鈕、LED    | 透過按鈕操控螢幕顯示畫面與LED溫濕度警示 |
| I2C         | 感測溫濕度資料    | 定時讀取 SHT30 感測器，測量溫濕度 |
| SPI         | 顯示感測資料      | ST7735 螢幕即時顯示溫濕度 |
| UART        | 與 PC 通訊        | 參數設定、回報資料頻率與閾值，支援中斷與 DMA 傳輸 |
| Timer / PWM | Timestamp 、 PWM | 使用 Timer 計算開機後時間、透過PWM 控制 ST7735 螢幕亮度 |
| RTOS Kernel  | 多工管理與通訊 |使用 FreeRTOS 進行任務拆分，實作 Queue (資料傳遞)、Mutex (I2C/SPI 資源鎖定) 與 Semaphore (同步)|

---

## 四、專案庫架構

本專案採用「程式源碼」與「開發文件」分離的雙目錄結構，確保開發環境單純，並將建置過程文件化以利維護。

### 1. 根目錄結構

```text
Project_Root/
├── code/   # 韌體專案本體 (Firmware Source)
└── notes/  # 專案文件與筆記 (Documentation)

```

### 2. 目錄內容說明

#### **Code (`/code`)**

* **定位**：實際執行的 STM32 韌體專案。
* **內容**：包含 `.ioc` (STM32CubeMX 設定檔)、CMake 建置腳本、HAL Drivers，以及實際運作程式碼。
* **操作**：所有的編譯 (Build)、燒錄 (Flash)、除錯 (Debug) 皆在此目錄下進行。

#### **Notes (`/notes`)**

* **定位**：開發過程的知識庫，紀錄環境建置、硬體參數與外部參考資料。
* **檔案清單**：

| 檔案名稱 | 說明 | 重點內容 |
| --- | --- | --- |
| **enviroment_setting.md** | **開發環境建置指南** | 紀錄在 Windows 11 環境下，從零建置 VSCode + CMake + GCC + ST-Link 工具鏈的步驟、環境變數設定與 VSCode 套件清單。 |
| **project_setting.md** | **STM32CubeMX 設定紀錄** | 詳細紀錄 Clock Tree 配置、GPIO Pinout 定義、NVIC 優先級與 DMA Channel 分配。用於在 `.ioc` 檔損毀或版本衝突時快速重建專案設定。 |
| **reference.md** | **參考資料彙整** | 專案相關的 Datashee。 |

---
## 五、專案軟體架構

本專案採用 **分層式架構** 設計，將硬體驅動（Drivers）、系統狀態（System State）與應用邏輯（Application Tasks）明確分離。此設計旨在降低模組間的耦合度，並確保程式碼在 RTOS 環境下的可維護性與擴充性。

### 1. 基礎建置與程式碼生成 (STM32CubeMX)

本專案的底層初始化程式碼由 **STM32CubeMX (v6.14.1)** 自動生成，主要包含在 `Core` 與 `Drivers` 資料夾中。

* **職責**：負責 Clock Configuration、NVIC 中斷優先級、GPIO/Peripherals 的底層 HAL 庫初始化。
* **原則**：除 `main.c`、`stm32f4xx_it.c` 中的 User Code區段外，盡量不修改自動生成的 HAL 檔案，以保持隨時可重新生成配置的彈性。

沒問題，這是將表格轉換為條列式說明的版本，強調層級之間的「呼叫關係」與「職責邊界」：

### 2. 自定義軟體分層 (User Structure)

為了解決嵌入式系統常見的「邏輯與驅動混雜」問題，本專案在 Generated Code 之上建立了三層架構，由下而上分別為：

#### **1. System (系統配置與狀態層)**

* **核心定義**：全域資料中心與板級配置
* **主要職責**：
    * `System State`：定義所有 Task 需要共享的資料結構（例如：目前的溫度數值、系統運作模式、LCD 亮度設定）。它是 Task 之間溝通的橋樑。
    * `Board Config`：充當 BSP角色。它負責將底層 HAL 的硬體定義抽象化，讓上層程式碼不需要知道實際使用硬體腳位

#### **2. Module (硬體驅動層)**

* **核心定義**：內/外部硬體驅動
* **主要職責**：
    * **封裝細節**：負責處理具體的硬體通訊協定與操作細節。
    * **提供 API**：只提供單純的功能函式
    * **設計原則**：這一層只管**怎麼做 (How)**，。應盡量保持與 RTOS TTask 無關，以利於移植。

#### **3. Task (應用邏輯層)**

* **核心定義**：User Space 的 Application
* **主要職責**：
    * **流程控制**：這是 FreeRTOS Task 實際運行的地方。負責決定**何時做**以及**做什麼**。
    * **資源調度**：負責時間控制（Delay）、資源控管(Mutex)、等待訊號（Semaphore/Queue）、狀態機切換。
    * **互動方式**：Task 會從 `System` 讀取設定，依據邏輯呼叫 `Module` 的 API 來控制硬體，最後將結果更新回 `System`。

### 3. 開發流程與檔案權責

在實際開發功能時，程式碼的修改路徑如下：

1. **System (`board_config.h`)**
    * 定義`Module`內定義的硬體驅動實際上使用那些腳位。

2. **System (`system_state.h`)**
    * 定義資料結構：在 `SystemState_t` 中新增 `temperature`, `humidity` 變數。
    * 定義旗標：若需跨 Task 通知，定義對應的 Semaphore 或 Queue Handle。

2. **Module (`Module/Src`)**
    * 實作任何內件、版上、外接的周邊硬體驅動

3. **Task (`Task/Src`)**
    * 建立RTOS Task 動作，切勿拆分過度詳細的任務，若動作為連續應該放在同一個Task(建立過多Tasky增加 Stack消耗與Context Switch成本)
    * 拆分Task的原則為
        * 任務頻率不同，例如 A 每 1ms 跑一次，B 每 10秒跑一次
        * 等待事件不同，例如 A 每 1ms 跑一次，B 等待外部中斷觸發才執行
        * 優先集不同，例如 A 為必須持續工作功能，B 可以忍受些微延遲

4. **Main Entry (`Core/Src/main.c`)**
    * 系統整合：在 `main()` 函式中呼叫進行硬體初始化、HAL 庫初始化。
    * 啟動排程：建立 FreeRTOS Tasks，最後呼叫 `osKernelStart()` 移交控制權給作業系統。

5. **Interrupts (`Core/Src/stm32f4xx_it.c`)**
    * 中斷處理：若使用非 RTOS 託管的中斷（或極簡短的 ISR），在此處清除旗標並通知 Task（例如透過 `xSemaphoreGiveFromISR`）。

---
## 六、RTOS 任務編排

本系統規劃 4 個主要任務，依據即時性需求分配優先級：

### **1. Sensor Task**
* **優先級**：High
* **觸發機制**：週期性喚醒
* **主要職責**：
    1. 定時讀取 SHT30 溫濕度數值。
    2. 根據閾值判斷是否觸發警示 (控制 LED D2/D3)。
    3. 取得 Mutex 並更新 `SystemState`。

#### **2. PC RX Task**
* **優先級**：Above Normal
* **觸發機制**：事件驅動 (Quene)
* **主要職責**：
    1. 進入 Block 狀態等待 UART ISR 放入 Queue 的原始資料。
    2. 解析指令封包 (Command Parsing)。
    3. 修改系統設定 (如取樣頻率 `measure_period_ms`)。

#### **3. Display Task**

* **優先級**：Normal
* **觸發機制**：事件驅動 (Semaphore) 或 定時刷新
* **主要職責**：
    1. 處理按鈕中斷事件 (K0 切換顯示溫濕度, K1 調整亮度)。
    2. 讀取 `SystemState` 數值。
    3. 更新 SPI LCD 畫面內容。



#### **4. PC Report Task**
* **優先級**：Low
* **觸發機制**：週期性喚醒
* **主要職責**：
    1. 定期取得 `SystemState` 複本。
    2. 將狀態打包成 Log 字串。
    3. 透過 UART TX 傳送至電腦端進行監控。

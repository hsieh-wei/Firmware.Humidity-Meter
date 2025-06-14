# 3_quick_start

## Step 1：建立 STM32CubeMX 專案

| 項目     | 操作路徑與說明                                                                      |
| ------ | ---------------------------------------------------------------------------- |
| MCU 選擇 | Start New Project → `MCU Selector` → 搜尋 `STM32F407VET6` → 點擊 `Start Project` |
---

## Step 2：Pinout & Configuration

### 2.1 System Core 主要設定

| 模組  | 選項設定                                             | 功能說明                    | 為何選此設定               |
| --- | ------------------------------------------------ | ----------------------- | -------------------- |
| SYS | `Serial Wire (SWD)`                              | 提供 ULINKpro Debug 與燒錄通道 | SWD 僅需 2 線，節省腳位，支援廣泛 |
| RCC | `HSE = Crystal/Ceramic Resonator` <br> `LSE` 不勾選 | 啟用外部高精度振盪器              | HSE 精準穩定，適合實際硬體時鐘來源  |

### 2.2 System Core GPIO 腳位設定

| 腳位   | 功能          | 模式           | 備註      |
| ---- | ----------- | ------------ | ------- |
| PA6  | LED         | GPIO\_Output |    out put level: low,push pull,no pull up pull down,low speed     |
| PH7  | LED         | GPIO\_Output |     out put level: low,push pull,no pull up pull down,low speed    |
---

## Step 3：Clock Configuration

| 設定項目          | 數值／選項            | 說明                             |
| ------------- | ---------------- | ------------------------------ |
| PLL Source    | HSE              | 使用外部時鐘                         |
| HSE Frequency | 8             | HSE固定頻率                       |
| PLLM          | 8               | VCO Input = 8 / 8 = 1 MHz      |
| PLLN          | 336              | VCO Output = 1 × 336 = 336 MHz |
| PLLP          | 2                | SYSCLK = 336 / 2 = 168 MHz     |
| PLLQ          | 7                | USB Clock = 336 / 7 = 48 MHz   |
| AHB Prescaler          | 1       |  HCLK = 168 MHZ |
| APB1 Prescaler          | 4       | PCLK1 = 42 MHZ|
| APB2 Prescaler          | 2       | PCLK2 = 84 MHZ |
| 目標            | SYSCLK = 168 MHz | 為 F407 可達之最大頻率                 |

---

## Step 4：Project Manager 設定

### 4.1 Project Settings

| 項目                        | 設定                         |
| ------------------------- | -------------------------- |
| Project Name              | 自訂                         |
| Project Location          | 自訂                         |
| Application Structure     | Advanced                         |
| Toolchain Folder Location | 自訂                         |
| Toolchain / IDE           | `MDK-ARM`（Min Version: V5） |

### 4.2 Thread-safe Settings

| 項目                                           | 操作 |
| -------------------------------------------- | ---- |
| Enable multi-threaded support                | 勾選    |
| Mapping suitable strategy for RTOS selection | 下拉選單    |

### 4.3 MCU and Firmware Package

| 項目            | 說明              | 備註|
| ------------- | --------------- |  --------------- |
| MCU Reference | `STM32F407VET6` | 其他先不動 |

### 4.4 Code Generator
- Generated files 
    - 勾選 Generating peripheral initialization as a pair of '.c / .h ' files per peripheral
    - 勾選 Keep User Code when re-generating
    - 勾選 delete previously  generated files when re-generated
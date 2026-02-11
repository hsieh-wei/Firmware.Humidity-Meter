# 開發筆記與文件 (Development Notes)

本目錄存放專案開發過程中整理的所有文件，包含環境建置指南、硬體連接定義、STM32CubeMX 設定備份，以及硬體 Datasheet 的重點整理。

## 1. 開發環境與硬體設置 (Setup Guides)

此區塊包含從零開始建置專案所需的步驟說明。

| 文件名稱 | 內容摘要
| --- | --- 
| **[1_development_enviroment_setting.md](./0_development_enviroment_setting.md)** | **開發環境建置指南**。包含 VSCode + CMake + GCC 工具鏈的安裝步驟、Extension 設定、Debugger 設定檔 (`launch.json`) 與 C/C++ IntelliSense 設定。 
| **[1_hardware_connection.md](./0_hardware_connection.md)** | **硬體接線表**。詳細列出 STM32F407 與各模組 (SHT30, LCD, UART) 的 Pinout 對應關係與供電需求。
| **[1_stm32cubemx_project_setting.md](./0_stm32cubemx_project_setting.md)** | **STM32CubeMX 專案設定紀錄**。紀錄 Clock Tree、GPIO 模式、NVIC 優先級、UART/SPI/I2C 參數與 DMA 設定。用於重建 `.ioc` 檔或查閱底層配置。

## 2. 硬體規格與 Datasheet 筆記 (Technical References)

此區塊為開發過程中對硬體規格書 (Datasheet) 的重點摘要與分析。

| 文件名稱 | 內容摘要 |
| --- | --- |
| **[2_lcd_datasheet.md](./1_lcd_datasheet.md)** | **ST7735 LCD 驅動筆記**。分析 SPI 時序 (CPOL/CPHA)、指令集 (Command Set)、初始化流程 (Reset/Sleep Out) 與繪圖指令 (CASET/RASET/RAMWR)。 |
| **[2_reference_document.md](./1_reference_document.md)** | **外部參考資料彙整**。包含 STM32F407 Reference Manual、HAL Driver 手冊、SHT30 Datasheet 的官方連結。 |

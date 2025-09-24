https://www.youtube.com/watch?v=aWMni01XGeI

# 目標與版本前置

* 目標板：STM32F407VE（可替換為你的實際型號）
* 主要工具（你指定的版本）

  1. STM32CubeMX 1.19
  2. STM32CubeIDE 1.19
  3. STM32CubeCLT 1.19
  4. CMake 4.11
  5. VSCode 外掛：Microsoft C/C++、STM32Cube for Visual Studio Code

---

# 一、安裝與環境變數

1. 安裝 STM32CubeMX 1.19、STM32CubeIDE 1.19、STM32CubeCLT 1.19（皆安裝在預設路徑即可）。
2. CMake 4.11` 安裝後加入系統 PATH
---

# 二、VSCode 外掛

* 安裝：

  * C/C++（ms-vscode.cpptools）
  * STM32Cube for Visual Studio Code（st.com）(2.1.1)

---

# 三、用 CubeMX 建立 CMake 專案

1. 開啟 CubeMX，設定 MCU/Clock/Peripherals。
2. Project Manager → Project：設定 `Project Name` 與專案路徑。
3. Project Manager → Code Generator：

   * 勾選「Generate peripheral initialization as a pair of .c/.h files per peripheral」
   * 建議選「Copy only the necessary library files」
4. Project Manager → Toolchain/IDE：選 **CMake**。
5. Generate Code。產物中會有最上層 `CMakeLists.txt` 與 `Core`, `Drivers`, `STM32CubeIDE` 等資料夾。

---

# 四、以 STM32Cube for VSCode 匯入專案

1. 在 VSCode 按 `Ctrl+Shift+P` → 執行「STM32: Import Project」
2. 選擇你剛產生的專案資料夾（含 `CMakeLists.txt` 的那層）。
3. Extension 會自動偵測並建立 `.vscode` 相關檔案或工作流程（若沒有，後面手動補上）。

---

# 五、建立/修正 VSCode 設定檔

## 1.  `C/C++` IntelliSense 設定（`c_cpp_properties.json`）

```json
{
  "version": 4,
  "configurations": [
    {
      "name": "STM32F4",
      "compilerPath": "C:/ST/STM32CubeCLT_1.19.0/GNU-tools/bin/arm-none-eabi-gcc.exe",
      "intelliSenseMode": "gcc-arm",
      "defines": [
        "USE_HAL_DRIVER",
        "STM32F407xx"
      ],
      "includePath": [
        "${workspaceFolder}/Core/Inc",
        "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc",
        "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy",
        "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F4xx/Include",
        "${workspaceFolder}/Drivers/CMSIS/Include"
        "其他自行編寫的檔案"
      ]
    }
  ]
}
```

## 2. 偵錯啟動（`launch.json`）

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Download Only (ST-LINK GDB Server + cortex-debug)",
      "type": "cortex-debug",
      "request": "launch",
      "servertype": "stlink",

      "serverpath": "C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_2.2.200.202505060755/tools/bin/ST-LINK_gdbserver.exe",

      "executable": "${workspaceFolder}/build/Debug/humidity_meter.elf",
      "device": "STM32F407VE",
      "interface": "swd",

      "runToEntryPoint": false,

      "cwd": "${workspaceFolder}",
      "gdbPath": "C:/ST/STM32CubeCLT_1.19.0/GNU-tools/bin/arm-none-eabi-gdb.exe",
      "preLaunchTask": "cmake-build-debug",

      "svdFile": "${workspaceFolder}/STM32F407.svd",
      "showDevDebugOutput": "raw"
    }
  ]
}
```

---

# 六、編譯與下載流程

1. 在 VSCode：直接按偵錯 F5。
2. 接上目標板的 ST-LINK（SWDIO/SWCLK/RST/GND），上電。
3. 若要單步/下中斷點/看暫存器，使用 cortex-debug 介面操作。

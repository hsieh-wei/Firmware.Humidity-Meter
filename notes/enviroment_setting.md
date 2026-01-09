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
            "compilerPath": "C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344/tools/bin/arm-none-eabi-gcc.exe",
            "intelliSenseMode": "gcc-arm",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "compilerArgs": [
                "-mcpu=cortex-m4",
                "-mthumb",
                "-mfpu=fpv4-sp-d16",
                "-mfloat-abi=hard"
            ],
            "defines": [
                "USE_HAL_DRIVER",
                "STM32F407xx"
            ],
            "includePath": [
                "${workspaceFolder}/Core/Inc",
                "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc",
                "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy",
                "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F4xx/Include",
                "${workspaceFolder}/Drivers/CMSIS/Include",
                // 以下是自己新增的檔案
                "${workspaceFolder}/Module/Inc"
            ],
            "browse": {
                "path": [
                    "${workspaceFolder}/Core/Inc",
                    "${workspaceFolder}/Drivers",
                    // 以下是自己新增的檔案
                    "${workspaceFolder}/Module/Inc"
                ]
            }
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

      // 改成你的st link server
      "serverpath": "C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_2.2.200.202505060755/tools/bin/ST-LINK_gdbserver.exe",

      // 改成你的.elf  
      "executable": "${workspaceFolder}/build/Debug/humidity_meter.elf",
      // 改成你的板子
      "device": "STM32F407VE",
      "interface": "swd",

      "runToEntryPoint": false,
      "entryPoint": "main",

      "cwd": "${workspaceFolder}",
      // 改成你的gdb路徑
      "gdbPath": "C:/ST/STM32CubeCLT_1.19.0/GNU-tools-for-STM32/bin/arm-none-eabi-gdb.exe",

      "svdFile": "${workspaceFolder}/STM32F407.svd",
      "showDevDebugOutput": "raw"
    }
  ]
}
```
## 3.  設定（`settings.json`）
```json
{
  // —— 關閉 Microsoft C/C++ 的語言功能（保留 Debug 也不影響）——
  "C_Cpp.intelliSenseEngine": "Disabled",
  "C_Cpp.errorSquiggles": "Disabled",
  "C_Cpp.autocomplete": "Disabled",
  "C_Cpp.formatting": "Disabled",
  "C_Cpp.codeAnalysis.runAutomatically": false,

  // —— 啟用/調整 Clangd（stm32-cube-clangd 內部用的是 clangd）——
  "clangd.arguments": [
    "--background-index",
    "--clang-tidy",
    "--completion-style=detailed",
    "--header-insertion=never",
    "--cross-file-rename",
    "--all-scopes-completion",
    // 讓 clangd 信任交叉編譯器，找得到對應標頭
    "--query-driver=C:/Program Files (x86)/Arm GNU Toolchain/*/bin/arm-none-eabi-*;C:/SysGCC/arm-none-eabi/bin/arm-none-eabi-*;/usr/bin/arm-none-eabi-*;/opt/*/bin/arm-none-eabi-*",
    // 指向 compile_commands.json 的位置（依你的 build 目錄調整）
    "--compile-commands-dir=${workspaceFolder}/build"
  ],

  // —— 只保留一個 formatter，避免再衝突（clangd 會用 clang-format）——
  "editor.defaultFormatter": "STMicroelectronics.stm32cube-ide-clangd",// 需要到 vscode extention 中取得 STM32Cube clangd 取得 id
  "editor.formatOnSave": true,
  "files.associations": {
    "main.h": "c",
    "stm32f407xx.h": "c"
  },
  "cmake.cmakePath": "cube-cmake",
  "cmake.configureArgs": [
    "-DCMAKE_COMMAND=cube-cmake"
  ],
  "stm32cube-ide-clangd.path": "cube",
  "stm32cube-ide-clangd.arguments": [
    "starm-clangd",
    "--query-driver=${env:CUBE_BUNDLE_PATH}/gnu-tools-for-stm32/13.3.1+st.9/bin/arm-none-eabi-gcc.exe",
    "--query-driver=${env:CUBE_BUNDLE_PATH}/gnu-tools-for-stm32/13.3.1+st.9/bin/arm-none-eabi-g++.exe"
  ]
}

```
---

# 六、編譯與下載流程

1. 接上目標板的 ST-LINK（SWDIO/SWCLK/RST/GND），上電。
2. 在 VSCode：直接按偵錯 F5。
3. 若要單步/下中斷點/看暫存器，使用 cortex-debug 介面操作。

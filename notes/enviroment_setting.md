https://www.youtube.com/watch?v=aWMni01XGeI

# 目標與版本前置

* 目標板：STM32F407VE（可替換為你的實際型號）
* 主要工具（你指定的版本）

  1. STM32CubeMX 1.19
  2. STM32CubeIDE 1.19
  3. STM32CubeCLT 1.19
  4. CMake4.11
  5. VSCode 外掛：Microsoft C/C++、STM32Cube for Visual Studio Code

---

# 一、安裝與環境變數

1. 安裝 STM32CubeMX 1.19、STM32CubeIDE 1.19、STM32CubeCLT 1.19（皆安裝在預設路徑即可）。
2. CMake 4.11` 安裝後加入系統 PATH
---

# 二、VSCode 外掛

* 安裝：

  * C/C++（ms-vscode.cpptools）
  * STM32Cube for Visual Studio Code（st.com）

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

## 2) 建置任務（`tasks.json`）

如果你不想仰賴 CMake Tools，也可以用最簡單的兩段式命令：Configure → Build。

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "cmake-configure-debug",
      "type": "process",
      "command": "C:/ST/STM32CubeCLT_1.19.0/CMake/bin/cmake.exe",
      "args": [
        "-S", "${workspaceFolder}",
        "-B", "${workspaceFolder}/build/Debug",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-G", "Ninja"
      ],
      "options": {
        "env": {
          "PATH": "C:/ST/STM32CubeCLT_1.19.0/GNU-tools/bin;${env:PATH}"
        }
      },
      "problemMatcher": []
    },
    {
      "label": "cmake-build-debug",
      "type": "process",
      "command": "C:/ST/STM32CubeCLT_1.19.0/CMake/bin/cmake.exe",
      "args": [
        "--build", "${workspaceFolder}/build/Debug",
        "-j"
      ],
      "dependsOn": "cmake-configure-debug",
      "problemMatcher": "$gcc"
    }
  ]
}
```

## 3) 偵錯啟動（`launch.json`）

你提供的設定需要兩個修正：

* `runToEntryPoint` 應給布林或符號名字串，不能是字串 "false"。
* 建議加上 `preLaunchTask` 以確保下載前已完成編譯。

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

> `svdFile` 可選，放對應 SVD 能在 Variables/Peripherals 視窗看到暫存器。
> 若你偏好「執行到 `main`」，也可改用 `"runToMain": true`（與 `runToEntryPoint` 擇一）。

---

# 六、編譯與下載流程

1. 在 VSCode：`Terminal → Run Task... → cmake-build-debug`（或直接按偵錯 F5，會先跑 `preLaunchTask`）。
2. 接上目標板的 ST-LINK（SWDIO/SWCLK/RST/GND），上電。
3. 選擇上面的 `Download Only` 啟動設定，開始下載與連線。
4. 若要單步/下中斷點/看暫存器，使用 cortex-debug 介面操作。

---

# 七、在 CMakeLists.txt 的注意事項

CubeMX 產生的 `CMakeLists.txt` 通常已足夠，但建議檢查：

* `CMAKE_TOOLCHAIN_FILE` 或 `CMAKE_SYSTEM_NAME` 是否由 CubeCLT/STM32 工具鏈檔設定；若沒有，使用者需自行指定。
* `-mcpu`, `-mthumb`, `-mfpu`, `-mfloat-abi` 與 `-O`/`-g` 等旗標是否正確對應你的 MCU 與需求。
* 若使用 FreeRTOS/USB/CMSIS-RTOS 等中介層，確保 `add_subdirectory` 或 `target_sources`/`target_include_directories` 已納入。
* 若需要 `printf` 重導向，加入 `syscalls.c` 或自己的 `_write()` 實作。

---

# 八、.gitignore（建議）

```gitignore
# Build artifacts
/build/
/CMakeFiles/
/CMakeCache.txt
cmake_install.cmake
compile_commands.json
Makefile

# IDE / Logs
*.log
*.bak
*.launch

# Binaries
*.elf
*.bin
*.hex
*.map
*.lst
*.o
*.obj
*.a
*.out
```

---

# 九、常見問題與排除

1. `ST-LINK_gdbserver.exe` 找不到

   * 確認 `serverpath` 的版本子資料夾是否與你安裝的一致。IDE 更新後路徑常換。
2. `runToEntryPoint` 型別錯誤

   * 不可設 `"false"`（字串），應為 `false`（布林）或 `"Reset_Handler"`/`"main"` 等符號名稱。
3. 無法連上目標

   * 檢查 3.3V 供電、SWD 腳位、RESET 線；試著在 ST-LINK 伺服器選項中加入「Connect Under Reset」。
4. 編譯找不到 HAL/CMSIS 標頭

   * 檢查 `includePath` 與 `target_include_directories`；CubeMX 設為「Copy only necessary library files」較不易遺漏。
5. `.elf` 路徑錯誤

   * 確認 `executable` 指向 `build/Debug/xxx.elf`。若你改成 Release，路徑也要一起改。
6. CMake 版本不符

   * 若你安裝了獨立 CMake（3.x）與 CubeCLT 內建 CMake 同時存在，請在 `tasks.json` 與 PATH 明確指定使用哪一個。
7. FPU/硬浮點不匹配

   * MCU 若無 FPU 卻指定 `-mfpu=fpv4-sp-d16 -mfloat-abi=hard` 會連結失敗或執行異常。確定與 F407 的 FPU 設定一致。

---

# 十、你的操作清單（對應你列的 1–9）

1. 下載並安裝 **STM32CubeMX 1.19**
2. 下載並安裝 **STM32CubeIDE 1.19**（供 ST-LINK GDB Server 使用）
3. 下載並安裝 **STM32CubeCLT 1.19**（供 Arm GCC/CMake/Ninja）
4. 使用 **CubeCLT 內建 CMake**（建議），或安裝 CMake 3.21+；「CMake 4.11」並不存在
5. VSCode 安裝 **C/C++** 與 **STM32Cube for VSCode**
6. 在 **CubeMX** 建專案 → Project Manager → Code Generator → Toolchain/IDE 選 **CMake** → Generate
7. 在 VSCode 執行 **STM32: Import Project** 匯入
8. 建立 `.vscode/launch.json`（採上方修正版），並可加入 `tasks.json`
9. 在專案的 `CMakeLists.txt` 右鍵或用 Tasks 執行 **Build All**，產生 `.elf`；之後用 `Download Only` 下載與偵錯

---

需要我把上述檔案（`.vscode/*.json`、`.gitignore`）直接套進你的現有專案命名與路徑嗎？我可以依你現在的資料夾結構改好再貼出來。


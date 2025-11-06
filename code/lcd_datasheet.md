## 1. 判斷模組是 3 線還是 4 線 SPI
| 面向| 3-line SPI| 4-line SPI|
| --- | --- | --- |
| 腳位| CSX、SCL、SDA（共 3 根 + RES/BL）| CSX、SCL、SDA、**D/CX**（共 4 根 + RES/BL
| D/CX 來源 | **封在資料串**裡的控制位| **獨立的 D/CX 腳**
> 看模組焊出來的腳：若有獨立 DC ，那幾乎就是4 線。

## 2. 8.4 Serial Interface Characteristics (4-line)

### 時序線中的參數
- CSX: 晶片選擇，拉低開始一筆交易，拉高結束。
- SCL: SPI 時鐘（STM32 的 SCK）。
- SDA: 主控到 LCD 的資料（MOSI）。
- D/CX: 資料/命令選擇，4 線才有獨立腳。
- SDA(DOUT): LCD 回主控的資料（MISO，版上沒有接出來，代表不做任何 Read 類型的傳輸）。

### MCU 要用哪個取樣邏輯
- SCK idle＝Low，CPOL=0
- 主控讓 SDA 在 SCL 上升緣被取樣，主控在下降緣改變 SDA 輸出，CPHA=0

### 一筆 SPI 交易該怎麼切界線
- D/CX 、 SDA 要在第一個取樣前設置好 CS該 SDA 前被設定好
    - D/CX 設置時間須超過 TDCS，並維持最少 TDCH
    - SDA 設置時間須超過 TSDS，並維持最少 TSDH (SPI無論HAL、bare metal 開發register，都會處理好解滿足)
- CS 規定交易邊界
 - TCSS，CS 由高到低以後，要等 TCSS 才能開始第一個上升緣
 - TCSH，最後一個上升緣後，CS 必須再維持 TCSH 才能拉高結束
 - TCHW，兩筆交易之間，CS 為高至少要維持TCHW。

### SCK 上限與最小脈寬
- 表裡 TSCYCW 表示 write clock cycle 的最小值，最小值 66 ns 表示 write 的 SCK 上限大約 15 MHz。
- High/Low 脈寬的最小值也要滿足TSHW/TSLW = 15ns

## 2. 9.4 Serial Interface

### 確認 9.4.1 Command Write Mode 
 - D/C=0 command, D/C=1 data/parameter
- 一筆資料含 8 bits，MSB first(D7 => D0)
- 傳輸規則
    - 當 CSX=High，介面閒置、忽略所有時鐘與資料
    - CSX 下降沿，開始一筆新的傳輸
    - CSX 上升沿，結束傳輸、重新初始化串列介面
- 取樣規則
    - SDA 在 SCL 上升緣 取樣
    - 在第 8 個 SCL 上升緣取樣 D/CX
- 傳輸連續性
    - 若 CSX 保持低電位，LCD driver 會持續等待下一個 byte
    - 下一筆的第 1 個 bit 視為新資料的 D7 位

## 3. 整理實際傳輸流程
- 準備階段
    - D/CX = 版上 DC = 0/1
    - CS=1
- 開始交易
    - CS=0 ，等 TCSS
    - 開始 SCK 上升緣
- 結束交易
    - 最後一個上升緣後讓 CS 保持低 TCSH
    - CS=1 結束，或 CS 保持低繼續傳輸
- 若要在結束交易後送下一筆， CS=1 要至少 TCHW 再拉低

## 4. 開始交叉閱讀 9.模組 、 10. cmd
- Init
    - Hardware Reset, 9.5、9.17、10.1.11
    - SWRESET, 10.1.2
    - SLPOUT, 10.1.12
    - MADCTL, 9.10、9.11、10.1.24
        - MY/MX/MV Coordinate Mapping for writing memory. Decide whether to draw out the flip, mirror, or rotate options.
        - ML/MH The refresh path when the LCD driver scans the panel.
        - RGB determines whether the lcd panel uses an RGB or BGR array.
    - COLMOD, 10.1.33
    - DISPON, 10.1.19
- Fill Screen
    - CASET, 10.1.20
    - RASET, 10.1.21
    - RAMWR, 10.1.22
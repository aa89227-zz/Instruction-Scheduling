# Instruction-Scheduling
模擬processor使用Tomasulo algorithm的行為，使用到RF、RAT(儲存ROB的位子)、RS(分為加減以及乘除)、RS Buffer(代表要執行的instruction，儲存RS的位子)
## 編譯環境
使用visual studio 2019進行編譯
## 如何運行
編譯後執行程式，輸入檔名為input.txt，輸入各項需求

## 資料格式
### 輸入
RISC-V格式
> 僅有 ADD, ADDI, SUB, SUBI, MUL, MULI, DIV, DIVI

#### 輸入範例

```
ADDI F1, F2, 1
SUB F1, F3, F4
DIV F1, F2, F3
MUL F2, F3, F4
ADD F2, F4, F2
ADDI F4, F1, 2
MUL F5, F5, F5
ADD F1, F4, F4
```

### 輸出
* cycle
* RF : Register file，儲存數值
* RS : Reserve station，有兩種，分別預留給加減法及乘除法
  * 運算符
  * 數值或需要提取數值的ROB位置
* RAT : Register allocation table，儲存ROB的位置
* ROB : Reorder buffer，在重新排序instruction時的儲存空間
  * 數值寫回的具體位置(write result)
  * 數值
  * 數值是否準備好了(for write result)
#### 輸出範例
```
cycle:17
  --RF---
F1|    1|
F2|    3|
F3|    2|
F4|    8|
F5|    9|
  -------

  -RAT--
F1|ROB7|
F2|ROB4|
F3|    |
F4|ROB5|
F5|ROB6|
  ------

   -RS-------------
RS1|   +|   8|ROB3|
RS2|    |    |    |
RS3|   +|ROB5|ROB5|
   ----------------
BUFFER:empty

   -RS-------------
RS4|   *|   9|   9|
RS5|    |    |    |
   ----------------
BUFFER:(RS4)9*9

    -ROB-----------
ROB0|  |    |     |
ROB1|  |    |     |
ROB2|  |    |     |
ROB3|F2|  16|    1|
ROB4|F2|   0|    0|
ROB5|F4|   3|    1|
ROB6|F5|   0|    0|
ROB7|F1|   0|    0|
    ---------------
```

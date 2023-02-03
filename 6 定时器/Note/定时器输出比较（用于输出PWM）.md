# 定时器输出比较（用于输出PWM）

> ![image-20230203213120755](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203213120755.png)

- 除了OC，还有IC（Input Capture输入捕获）、CC（Capture/Compare表示输入捕获/输出比较）

- CNT（计数器）、CCR（捕获/比较寄存器），这两部分电路所在位置如下：

    > ![image-20230203213405757](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203213405757.png)

    另外捕获/比较寄存器的输入捕获和输出比较是共用一个寄存器的，当使用输入捕获模式，其就是捕获寄存器，当使用输出比较时，其就是比较寄存器

- 高级定时器的死区生成和互补输出功能是用于驱动三相无刷电机的。

# PWM简介

> ![image-20230203213742712](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203213742712.png)

- PWM的秘诀就是：天下武功，唯快不破。只要我闪得足够快，你就发现不了我是闪着亮的，还是一个正常、平稳的亮度。
- PWM的频率越快，其等效的模拟信号就越平稳，不过这样性能开销也就越大。**一般来说PWM的频率在1kHz~100kHz即可**。  
- **使用PWM就可以在数字系统输出模拟量。**

## PWM基本结构***

以下内容采用的是PWM模式1

> ![image-20230203221154282](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203221154282.png)
>
> 上图中右上角的图像中，蓝线是CNT，黄线是ARR，红线是CCR的值。

- 可见比较后输出的PWM信号的占空比受CCR值的调控

## PWM参数计算

> ![image-20230203222356028](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203222356028.png)

- PWM的频率可以看出其等于定时的一个溢出更新周期，所以PWM的频率就等于计数器的更新频率
- 如果ARR我们写99，那么CNT就会从0计到99（共100个数），此时如果再写CCR是30，那么CNT的0~29（共30个数）就是CNT<CCR的情况，输出高电平，所以30/100=30%，占空比就是30%

# 输出比较通道结构图

## 高级定时器

> ![image-20230203214926420](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203214926420.png)

此部分内容讲解见 [25:45~28:30](https://www.bilibili.com/video/BV1th411z7sn/?p=15&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=b8a7d9f6d396acb27e811371a96d6e9a)

内容包括MOS管构成的推挽电路以及H桥电路的构成原理（三个这样的推挽电路可以用于驱动三相无刷电机）、OC1与OC1N互补输出、死区生成电路

## 通用定时器

该部分电路位于通用定时器的下示位置：

> ![image-20230203215024120](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203215024120.png)
>
> 该输出比较部分的电路的左边是CNT和CCR的比较电路，右边就是输出比较电路，最后通过TIMx_CHx输出到GPIO引脚上

> ![image-20230203214946424](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203214946424.png)

- 当CNT>=CCR1时，输出模式控制器就会收到一个信号，然后输出模式控制器就会改变其输出oc1ref（ref表示：参考信号）的高低电平

- 随后oc1ref信号可以通向主模式控制器，我们可以把这个ref信号映射到主模式的TRGO输出上。

- 不过oc1ref信号一般都通向主模式控制器下面的那一路：

    信号到了TIMx_CCER这里（这部分电路用于**极性选择**）

    - 如果CC1P寄存器写0，则信号就通过非门上门的那一路电路，直接从oc1ref通道输出使能电路，此时信号的电平不翻转（输入什么就输出什么）
    - 如果CC1P寄存器写1，信号就会通过非门，此时输入信号的电平就会被取反输出，即信号的电平翻转

- 输出使能电路决定信号是否输出
- OC1引脚是CH1通道的引脚。在引脚定义表中可以知道具体是哪一个GPIO口。

### 输出模式控制器

输出模式控制器决定oc1ref为高/低电平，其输入是CNT和CCR的大小关系，输出是oc1ref的高低电平。

下图是输出比较的8种模式，也就是输出模式控制器里面执行的逻辑：

> ![image-20230203220228745](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203220228745.png)

- 冻结模式下实际上不管CNT和CCR的大小关系，其输出的ref保持上一个状态不变。该模式可以用于PWM信号输出的暂停。
- 有效电平：高电平，无效电平：低电平
- 匹配时置有效电平、匹配时置无效电平、匹配时电平翻转，这三种模式的用途讲解见视频 [15:00~18:00](https://www.bilibili.com/video/BV1th411z7sn/?p=15&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=b8a7d9f6d396acb27e811371a96d6e9a)

- 强制为有/无效电平，该模式与冻结模式类似，可以用于暂停波形输出并可以指定暂停期间的ref电平
- PWM模式，**该模式非常重要**，

# 舵机简介

> ![image-20230203223607124](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203223607124.png)
>
> - 注意右图中舵机的轴转角与PWM的占空比是线性关系的
> - 另外在舵机里**PWM信号被当成通信协议**使用

## 舵机硬件电路

> ![image-20230203223733755](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203223733755.png)

> ![image-20230203223747587](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203223747587.png)
>
> ![image-20230203223802812](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203223802812.png)

# 直流电机及驱动简介

> ![image-20230203223906140](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203223906140.png)

## TB6612硬件电路

该部分内容讲解见 [37:40~42:40](https://www. bilibili.com/video/BV1th411z7sn/?p=15&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=b8a7d9f6d396acb27e811371a96d6e9a)

> ![image-20230203224103465](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230203224103465.png)

- STBY（Stand By）是待机控制引脚，若接GND芯片就不工作，接VCC芯片则正常工作
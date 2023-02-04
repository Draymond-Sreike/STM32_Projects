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

# 程序

## 程序配置步骤

> ![image-20230204094424110](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204094424110.png)

1. RCC开启时钟。将TIM和GPIO外设的时钟都打开
2. 配置时基单元及其时钟源选择
3. 配置输出比较单元。包括CCR值设置、输出比较模式、极性选择、输出使能。通过结构体参数进行配置
4. 配置GPIO，将GPIO口初始化为复用推挽输出模式
5. 运行控制。启动计数器即可输出PWM

## 定时器相关函数

### 配置输出比较单元*

> ![image-20230204094803283](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204094803283.png)

用结构体来配置输出比较单元（OC——Output Compare输出比较）。一个函数配置一个单元。

### 结构体赋默认值

> ![image-20230204095031261](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095031261.png)

### 配置强制输出模式

> ![image-20230204095108600](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095108600.png)

在运行过程中如果想要暂停输出并强制输出高或低电平可以用该函数。

### 配置CCR预装功能

> ![image-20230204095234539](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095234539.png)

预装功能即影子寄存器，实现写入的值不会立刻生效而是在更新发生后才生效。

### 配置快速使能

> ![image-20230204095354662](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095354662.png)

这个在手册中“单脉冲模式”那一小节有介绍。

### 外部事件清除Ref信号

> ![image-20230204095458305](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095458305.png)

在功能手册“外部事件清除Ref信号”那一小节有介绍。

### 配置输出比较极性

> ![image-20230204095605408](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204095605408.png)

这些函数来用来单独设置输出比较极性，函数名中带个N的就是高级定时器里互补通道的配置（OC4没有互补通道，所以函数名带N的函数）。

这里可以通过这个函数设置极性，在配置输出比较单元的结构体参数里也有极性的配置，这两个地方设置极性的作用是相同的。只不过结构体是多个参数一起设置，这个函数是对单独的参数进行修改。一般来说，结构体里的参数都会对应有一个单独的函数可以进性单独修改。	

### 配置输出使能

> ![image-20230204100005613](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100005613.png)

用于单独修改输出使能参数。

### 配置输出比较模式

> ![image-20230204100028102](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100028102.png)

用于单独更改输出比较模式。

### 配置CCR值*

> ![image-20230204100101279](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100101279.png)

用于单独更改CCR值。在运行时我们更改占空比就需要用到这四个函数。

### 使能主输出（高级定时器）

> ![image-20230204100240321](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100240321.png)

> ​	<img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100251148.png" alt="image-20230204100251148" style="zoom:50%;" />

## PWM初始化函数编写

### 配置定时器

> ![image-20230204122426484](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204122426484.png)

开启RCC时钟，选择内部时钟，初始化时基单元。

### 配置输出比较单元

配置输出比较单元用下面这四个函数：

> ![image-20230204100641569](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100641569.png)

每个函数对应一个输出比较单元或者说输出比较通道，我们需要用哪个通道就使用对应的函数，但是应注意不同的通道对应的GPIO口不同。

如果要使用PA0口输出PWM，PA0对应的就是OC1，对应关系可以参考引脚定义表：

> ![image-20230204115728666](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204115728666.png)

CHx即表示这些对应的通道，可见OCx的输出通道在PA0~PA3之间。

#### 引脚使用出现冲突可以采用重定义

如果我们要使用USART2_TX并且还要使用TIM2_CH3，此时冲突在了PA2上，这个时候我们就可以把CH3从PA2**重映射/重定义**到PB10上，由此解决引脚冲突问题。

> ![image-20230204115927808](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204115927808.png)

注意，如果重定义的列表中找不到外设那就不能进行重定义。

配置重定义/重映射是通过AFIO来完成的。

那么我们如果要将PA0的TIM2_CH1映射到PA15上，该如何操作呢？

> ![image-20230204120231991](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204120231991.png)

##### 配置AFIO

> ![image-20230204122812825](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204122812825.png)

使用AFIO引脚重映射配置函数。

- GPIO_Remap：

    该参数的选项非常多

    > ![image-20230204122922376](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204122922376.png)
    >
    > 这些都是重映射的方式。

    每种重映射对应的方式可以查阅手册：

    > ![image-20230204123056361](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123056361.png)
    >
    > 该表即重映射和引脚的对应关系。
    >
    > 可以知道如果我们要将PA0的CH1映射得到PA15就可以产生部分重映射方式1或者完全重映射。

    回到GPIO_Remap的参数选项，我们找到TIM2的重映射参数：

    > ![image-20230204123228277](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123228277.png)

    这里我们就选择重映射方式1：

    > ![image-20230204123329632](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123329632.png)

    这样就把PA0的CH1映射到PA15上了。

    另外还要注意到，PA15上电后默认已经复用为调试端口JTDI了：

    > ![image-20230204123500103](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123500103.png)

    所以如果我们要将PA15作为普通的GPIO或者复用定时器通道，那还需要先将其作为JTDI的复用给关掉，将其关掉的函数仍然是：

    > ![image-20230204123825106](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123825106.png)

    > ![image-20230204123835938](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123835938.png)
    >
    > 这三个参数就是用来解除调试端口的复用的。
    >
    > SWJ就是SWD和JTAG这两种调试方式。
    >
    > - NoJTRST：就是解除JTRST引脚的复用，对应的引脚就是：
    >
    >     > PB4
    >     >
    >     > ![image-20230204123957152](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204123957152.png)
    >
    >     所以如果使用这个参数，PB4就变回正常的GPIO口了，**其他四个调试端口就仍为调试端口而不能当作GPIO来使用**：
    >
    >     > ![image-20230204124103410](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204124103410.png)
    >
    > - JTAGDisable：解除JTAG调试端口的复用。在引脚定义里就是：
    >
    >     > ![image-20230204124236750](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204124236750.png)
    >
    >     让这三个引脚变回GPIO，此时PA13和PA14就仍为SWD的调试端口
    >
    > - Disable：这个参数可以把SWD和JTAG的调试端口复用全部解除，即让这下面5个引脚全部变回GPIO：
    >
    >     > ![image-20230204124519938](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204124519938.png)
    >
    >     此时就没有调试功能了。
    >
    >     这个参数千万不要随便调用，一旦我们调用这个参数并下载程序后这些调试端口就全部失效了，之后再使用STLINK就无法下载程序到STM32了，这时就只能通过串口进行下载了（要把调试端口弄回来就要通过串口写一个新的没有解除调试端口的程序）

    以上相关参数在手册中的对应内容如下：

    > ![image-20230204124754357](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204124754357.png)

参照引脚定义表：

> ![image-20230204125051014](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204125051014.png)

如果我们要使用PA15、PB3、PB4这三个引脚，通常就是接触JTAG的复用，保留SWD的复用。所以这里我们参数就选：

> ![image-20230204125117705](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204125117705.png)

这样就能正常使用PA15这个引脚了。

##### 总结

> ![image-20230204125236458](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204125236458.png)

- 如果想让PA15、PB3、PB4这三个引脚当作GPIO来使用，则使用上述代码中的第8、10行即可（第10行代码用于解除调试端口复用）
- 如果想重映射定时器或者其他外设的复用引脚，则使用第8、9行代码即可（第9行代码用于重映射引脚）
- 如果重映射的引脚又正好是调试端口，那么上面这三行代码都得用上



#### 结构体参数配置

> ![image-20230204100932480](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204100932480.png)

输出比较单元的配置结构体参数较多，但很多是高级定时器才用到的，例如结构体参数名中带N的，或者：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101031167.png" alt="image-20230204101031167" style="zoom:50%;" />

这些参数是高级定时器才用得上的。

我们这里使用通用定时器就只需要把我们需要的参数列出来即可：

> ![image-20230204101254301](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101254301.png)
>
> Mode：设置输出比较的模式
>
> Polarity：设置输出比较的极性
>
> OutPutState：设置输出使能
>
> Pulse：设置CCR

但是这样的话结构体的部分参数没有赋值，所以为了防止因为这个未赋值导致程序存在问题，我们可以先通过调用给结构体赋初始值的函数，然后再将结构体中我们需要的参数进行更改：

> ![image-20230204101555973](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101555973.png)

##### Mode

> ![image-20230204101648804](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101648804.png)
>
> Timing冻结模式
>
> Active相等时置有效电平
>
> Inactive相等时置无效电平
>
> Toggle相等时电平翻转

另外：

> ![image-20230204101802445](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101802445.png)

这两个模式是强制输出的两种模式，不过这两种模式无法在初始化时使用。

我们选择PWM1模式：

> ![image-20230204101900073](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101900073.png)

##### Polarity

> ![image-20230204101924424](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204101924424.png)
>
> High高极性，极性不翻转，REF波形直接输出。或者说有效电平是高电平，REF有效时输出高电平。
>
> Low低极性，REF电平取反。或者说有效电平为低电平。

这里我们选择高极性：

> ![image-20230204102146835](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204102146835.png)

##### OutPutState

> ![image-20230204102208600](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204102208600.png)
>
> 失能、使能

使能：

> ![image-20230204102244498](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204102244498.png)

##### Pluse

> ![image-20230204102328873](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204102328873.png)
>
> 注释：指定Pulse的值会被加载到CCR。

### 配置GPIO

> ![image-20230204120924688](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204120924688.png)

- 使用复用推挽输出模式，原因是推挽模式在高低电平下都具有驱动能力（这一点应该是符合PWM信号要求的）

- 而采用“复用”的原因是，普通的推挽输出是由输出数据寄存器来控制引脚的高低电平，而我们此次项目的引脚应交由定时器来控制，所以是使用其他外设来控制引脚，这需要配置为”复用“

    > ![image-20230204120709339](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\6 定时器\Note\image-20230204120709339.png)

    对于PA0而言，这里“片上外设”连接的就是TIM2的CH1通道，所以当我们把引脚设置为复用推挽输出模式时，引脚的控制权就可以给到TIM2的CH1了，此时CH1的PWM就能输出到IO口了。

# 呼吸灯程序


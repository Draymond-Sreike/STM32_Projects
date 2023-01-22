# GPIO程序

.h文件的最下面一般是所有函数的声明

![image-20230122175602611](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\3-1 GPIO\Note\image-20230122175602611.png)

## 第一步：使用RCC开启GPIO的时钟

## 第二步：使用GPIO_Init函数初始化GPIO（配置端口模式）

> ​	<img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\3-1 GPIO\Note\image-20230122175953073.png" alt="image-20230122175953073" style="zoom:50%;" />
>
> - AIN(Analog IN)是模拟输入
>
> - IN_FLOATING是浮空输入
>
> - IPD(In Pull Down)是下拉输入
>
> - IPU(In Pull Up是上拉输入)
>
> - OUT_OD（Out Open Drain）开漏输出
>
> - OUT_PP(Out Push Pull)是推挽输出
>
> - AF_OD(Atl Open Drain)是复用开漏
>
> - AF_PP(Atl Push Pull)是复用推挽 

## 第三步：使用输出或者输入的函数控制GPIO口

### GPIO的写入函数

> ![image-20230122180604195](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\3-1 GPIO\Note\image-20230122180604195.png)
>
> - GPIO_SetBits，可以把指定的端口设置为高电平
> - GPIO_ResetBits，可以把指定的端口设置为低电平
> - GPIO_WriteBit，可以根据该函数的第3个参数BitValue来设置端口的高低电平
> - GPIO_Write，通过PortVal可以同时对16端口进行写入操作

# 注意不要选PA15、PB3、PB4

> ![image-20230122184325545](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\3-1 GPIO\Note\image-20230122184325545.png)

这几个端口默认是JTAG的调试端口，如果要用做普通端口的话，还需要再进行一些配置。
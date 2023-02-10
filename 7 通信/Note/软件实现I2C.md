# 框架（通过MPU6050举例实现）

- 在MyI2C.c文件里写好GPIO的初始化和6个基本时序单元（起始、终止、发送一个字节、接收一个字节、发送应答、接收应答）
- 在MPU6050.c文件里将基于MyI2C实现指定地址读/写，再实现写寄存器对芯片进行配置，读传感器得到传感器数据
- 最后在main.c中调用MPU6050模块，初始化、拿到数据、显示数据

框图如下：

> ![image-20230210105458242](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210105458242.png)

# 软件实现I2C

## MyI2C初始化函数

1. 在这个函数里要把SCL和SDK对应的IO口配置为开漏输出模式
2. 把SCL和SDK置高电平（相当于释放总线，使其处于空闲状态）

> ![image-20230210105946816](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210105946816.png)
>
> 这里虽然IO口是输出，但其仍然可以输入。输入时先输出1（即把SDA放手），再直接读取输入数据寄存器即可（即看着从机怎么拉动SDA变化，由此获得从机的数据）

此时调用这个初始化函数，主机用于SCL和SDK的IO口就被初始化为开漏输出模式并且释放总线。

### 宏定义更换IO口名

为了使得IO口对应SCL和SDK的语义更加明显，并且后续在更换SCL和SDK的IO口时可以快速地进行，我们可以采用宏定义的方法对这两个IO口换一个名字：

> ![image-20230210110715640](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210110715640.png)

这样在编写后续函数时就可以这样：

![image-20230210110758745](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210110758745.png)

这样语义就更加明显，如果有多行代码重复时也就可以方便地一次性修改了。

进一步地，如果每次都需要进行宏定义也会比较麻烦，我们也可以**对整个函数进行宏定义**替换：

### 宏定义更换函数

> ![image-20230210111020852](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210111020852.png)

这里我们就是把宏定义的GPIO_WriteBit包括参数在内的整个函数用宏定义替换了个名字OLED_W_SCL，后续使用时就可以这样：

![image-20230210111140147](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210111140147.png)

并且我们这里使用了带参数的有参宏，有参宏是这样玩的：在宏定义后面加一个括号，里面写入形参：(x)，后续我们在实际引用的时候，如上图的例子所示，实参给了1，替换时x就是1，然后再进一步替换到函数里，替换前和替换后的对比如下：

![image-20230210111443355](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210111443355.png)

这两句的效果是等价的。

进一步地，由于这种宏定义的方法如果换到一个主频很高的单片机中，在软件时序需要进行延时操作的时候也不太方便进一步修改，所以我们也可以直接将对IO口的操作封装为一个函数，这样可以即达成了方便理解、又方便加软件延时：

### 函数封装函数

> ![image-20230210112011430](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210112011430.png)

- 函数名中W代表写的意思
- 这样后面在调用这个函数时，参数给1或0，就可以对应释放或拉低SCL了
- 后续如果我们将这个函数移植到别的单片机，例如51，就可以将上图第6行代码替换成：P10 = BitValue;
- 为了放慢一些引脚的翻转速度让MPU6050跟得上，这里我们可以延时10微秒

## SCL&SDA口读写函数

### 写

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210113604007.png" alt="image-20230210113604007" style="zoom:50%;" />

这两个函数决定主机拉低和释放SCL和SDK，在调用这两个函数时：

- 如果传给BitValue的值是1，即MyI2C_W_SCL（1），则表示释放
- 如果传给BitValue的值是0，即MyI2C_W_SCL（0），则表示拉低

为了放慢一些引脚的翻转速度让MPU6050跟得上，这里我们可以延时10微秒。

### 读

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210113858479.png" alt="image-20230210113858479" style="zoom:50%;" />

这个函数决定主机读取SDA上的电平变化，并且每读取一位延时10微秒。

（注意IO口的读写的数据寄存器是分开的，而不是同一个）

## MyI2C起始条件函数

<img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210110400282.png" alt="image-20230210110400282" style="zoom:33%;" />

1. 先确保MyI2C的SCL和SDA都释放
2. 然后先拉低SDA，再拉低SCL

这样就能产生起始条件了：

> ![image-20230210114531688](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210114531688.png)

- 释放SCL的操作（即MyI2C_W_SCL（1））不能省，因为这个函数在用于重复起始条件Sr处时有可能SCL是处于低电平的状态，这样就没办法产生起始条件了，所以需要确保SCL的释放（到高电平）

- 这里注意释放SDA（即MyI2C_W_SDA（1）），最好要放在释放SCL之前（即MyI2C_W_SCL（1）），否则如果释放SDA在释放SCL之后，则可能出现这种情况：

    由于终止条件是这样的：

    > <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210114944047.png" alt="image-20230210114944047" style="zoom:50%;" />

    如果在这个起始条件函数调用前，SCL和SDA刚好都是低电平，则会导致先释放了SCL，然后释放SDA就**错误地产生了终止条件**。这种情况在“指定地址读”的时序中的重复起始条件Sr处可能存在：

    ![image-20230210115252497](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210115252497.png)

    这个地方的SCL就是低电平，SDA可能是低电平，所以如果我们在重复起始条件Sr的位置调用了这个释放SCL在前，SDA释放在后的函数，是有可能导致上述错误的，因此这里我们**将SDA的释放放在SCL释放之前，以避免产生这种错误的可能**。

    这样重复起始条件Sr就可以也使用这个起始条件函数，而不用单独再另写一个了。

## MyI2C终止条件函数

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210114944047.png" alt="image-20230210114944047" style="zoom:50%;" />

1. 先确保MyI2C的SCL和SDA都处于低电平（同样的为了避免错误产生起始条件，我们要先拉低SCL，再拉低SDA，但是终止条件的时序开始时能保证SCL一定是低电平{因为终止条件之前的单元除终止条件之外，我们都要保证以低电平结束，这能方便各个时序单元的拼接}，所以拉低SCL的代码可以省略】）
2. 然后先释放SCL，再释放SDA

> ![image-20230210120804854](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210120804854.png)

这里我想试试吧MyI2C_W_SCL(0)写在MyI2C_W_SDA(0)前试试。。。

## MyI2C发送一个字节

发送一个字节时序开始时，SCL是低电平。**实际上除了终止条件是以SCL高电平结束，其他所有的单元我们都会保证SCL以低电平结束，这样方便各个单元的拼接**。

> ![image-20230210125545015](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210125545015.png)

- 在SCL低电平期间变换数据，SCL高电平期间保持数据稳定不变
- 高位先行，先放最高位再放低位
- 每放完一位后执行释放SCL，随后再拉低SCL的操作，以此驱动时钟运转

代码如下：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210132252993.png" alt="image-20230210132252993" style="zoom:50%;" />

- 取出Byte的第一位可以：Byte & 0x80，这样就可以把Byte的最高位取出，考虑到我们传入的Byte & 0x80到MyI2C_W_SDA函数后，其值会被转换成BitAction类型（该类型非0即1），所以如果Byte的高位是1，传入的就是Byte & 0x80则转换成BitAction后就是1；如果Byte的高位是0，传入的Byte & 0x80转换成BitAction后就是0，所以这里获得的Byte & 0x80的结果不需要再进行位移的操作。

    Byte & 0x80的示意图如下：

    > ![image-20230210131954837](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210131954837.png)

    其他位依次类推分别**与**0x80、0x40、0x20...即可得到Byte的每一位

- 数据放好之后就释放SCL，随后再拉低SCL，这样在释放SCL的瞬间从机就会把我们放在SDA的数据读走

- 上述操作存在重复，我们可以通过一个循环来实现：

    > <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210132521818.png" alt="image-20230210132521818" style="zoom:50%;" />

    这里为了保证在放上Byte的最高位数据到SDA的时候，SCL是低电平，我想在54行代码处写多一句MyI2C_W_SCL(1)。。。

### 发送第一个字节的最低位为0表示主机要写，为1表示主机要读

## MyI2C接收一个字节

> ![image-20230210135052485](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210135052485.png)

- 为了防止主机干扰从机写入数据，主机需要先释放SDA（释放SDA也就相当于切换为输入模式）
- 在SCL低电平时，从机会把数据写到SDA
- 主机释放SCL，在SCL高电平期间主机读取SDA，随后再拉低SCL，从机就会把下一位数据放到SDA上，依次重复8次从机就能读到一个字节了

程序配置步骤就是：

0. 拉低SCL（保证SCL是低电平）

1. 主机释放SDA（把SDA控制权给从机）
2. 主机释放SCL，SCL高电平期间主机读取SDA
3. 随后主机拉低SCL，从机就会把下一位数据放到SDA上
4. 执行上述1~3的步骤循环8次即可读取一个字节

程序如下下：

> ![image-20230210140715864](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210140715864.png)
>
> 该函数只展示了读取从机发送的Byte数据的最高位的情况

我们用循环来读取从机发送Byte8位数据：

> ![image-20230210141138763](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210141138763.png)

- 在for循环前第66行代码到第一次执行第69行代码的期间，从机就把Byte最高位数据放到SDA上了，可以理解为就在第66行代码执行的瞬间完成的
- 随后在for循环中，第71行代码循环回到第69行代码的期间，从机就以此把Byte放到了SDA上了，可以理解为就是在第71行代码执行的瞬间完成的

# 补充：关于SCL低电平变换SDA数据，高电平读取SDA数据这种读写分离设计的123木头人举例解释

详细讲解见视频 [20:55~22:05](https://www.bilibili.com/video/BV1th411z7sn?p=33&vd_source=b8a7d9f6d396acb27e811371a96d6e9a)

> ![image-20230210135742724](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210135742724.png)

- SCL低电平变换SDA数据，高电平读取SDA数据，这是种读写分离设计：SCL低电平时间为写SDA的时间，SCL高电平时间为读SDA的时间

- 123木头人的类比理解

- 如果在SCL高电平期间违反规则，非要写SDA，这样就相当于破坏游戏规则，这样的信号就变成了起始条件和终止条件：

    > ![image-20230210135949862](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210135949862.png)

    这个设计保证了起始和终止条件的特异性，使得其与发送和接收等其他时序有着本质区别，使得能让我们在连续不断的波形中快速定位起始和终止

    - 字节发送接收期间，SCL高电平不许写SDA
    - 起始终止，SCL高电平期间要写SDA

## MyI2C发送应答&接收应答

发送应答和接收应答实际上就是发送和接收一个字节的缩减版 ：只收发一位Bit

> ![image-20230210141219399](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210141219399.png)

### 0表示应答，1表示非应答(拉下SDA就是回应，不拉就是不回应	)

### 发送应答函数（从机发送给主机数据后）

> ![image-20230210150356062](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210150356062.png)

> ![image-20230210141611775](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210141611775.png)

1. 函数进来时，SCL低电平，主机把AckBit放到SDA上
2. 主机释放SCL，从机读取应答位
3. 主机拉低SCL，进入下一个时序单元

### 接收应答函数（从机接收主机数据后）

> ![image-20230210150402940](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210150402940.png)

> ![image-20230210142001048](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210142001048.png)

1. 函数进来时，SCL低电平
2. 主机释放SDA，防止干扰从机，此时从机把应答位放在SDA上
3. 主机释放SCL，随后主机读取应答位
4. 主机拉低SCL，进入下一个时序单元

补充：第86行代码把SDA置1了，而后面第88行代码又来读取SDA，不是相当于我们主机把1放在SDA上，然后又读SDA得到1吗？这有什么意义呢？

解释：

1. I2C的引脚都是开漏输出+弱上拉的配置，主机对SDA置1，由于已经配置为了开漏输出模式，所以这里并不能将SDA强置为1，而是**释放**SDA（强置是将SDA硬拉到1，而释放是将SDA放开，然后由上拉电阻弱上拉到1，方便从机容易地将SDA拉低）
2. I2C通信期间，主机释放了SDA，从机并不是在看戏的，从机如果在的话是有义务操作SDA的，所以这里即使第86行代码主机把SDA释放了，之后在经过从机的操作之后，读到的SDA就是由从机控制输出出来的。

# 程序初验证

讲解见视频 [27:15~](https://www.bilibili.com/video/BV1th411z7sn?p=33&vd_source=b8a7d9f6d396acb27e811371a96d6e9a)

由于在没有解除MPU6050芯片睡眠模式之前写入寄存器是无效的，从机能准确应答主机的只有主机发送的第一个关于从机寻址+读/写的字节，所以我们程序初验证写成这样：

> ![image-20230210144413047](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210144413047.png)

如果程序正常运行，就是这样的：

> ![image-20230210145247254](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210145247254.png)
>
> 此时应答成功

如果我们将从机地址换成一个不存在的：

> ![image-20230210145307896](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210145307896.png)

程序运行结果如下：

> ![image-20230210145330513](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210145330513.png)
>
> 此时非应答，没有这个0xA0的设备

我们这里这里的程序时间上可以封装成一个功能：从机地址的扫描

> ![image-20230210145437557](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210145437557.png)

我们可以把上图代码套起来并用一个循环遍历所以从机地址，然后把应答位为0的地址统计下来，这样就能实现扫描总线上所有从机的功能了。

不过注意遍历时改变从机地址即可：

> ![image-20230210145558077](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210145558077.png)

不要改动写标志位0，如果将写标志位改成了读标志位1，则可能导致在主机遍历的过程中从机可能因为主机授予的这个权利而干扰程序后续的遍历。

再配合AD0改名的功能还可以继续进行上面寻机的应答验证。

（MPU6050默认AD0为低电平0）

# MPU6050指定地址写（一个字节）

> ![image-20230210161603559](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210161603559.png)

程序配置如下：

> ![image-20230210161733530](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210161733530.png)

- 第10、12、14行代码实际上从机都返回了一个应答位，如果我们想通过应答位进行判断后执行相应的程序，可以在此处展开。但这里为了保证时序结构的清晰，我们这里没有对返回的应答位进行判断。
- 发送第一个字节之后的第二个字节就是指定MPU6050的寄存器地址了，如上图第11行代码所示：此时RegAddress这个字节数据就会作为MPU6050的寄存器指针变量的值，写入到MPU6050的指针变量中，用于具体读写哪一个寄存器
- 第三个字节就是在寄存器指针所指向的单元内写入数据Data了
- 如果我们想指定地址写多个字节就可以用for循环将第13、14行代码套起来反复执行即可（最好是用数组存储这些字节）

# MPU6050指定地址读（一个字节）

> ![image-20230210162542523](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210162542523.png)

程序配置如下：

> ![image-20230210162841858](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210162841858.png)

- 第32行代码处如果我们想继续读取从机数据，则主机就给应答0，如果只要一个字节数据则就是直接给非应答1。给非应答的时候从机就不会再去碰SDA了
- 如果我们想升级为指定地址读多个字节，就是用循环将第31、32行代码反复执行（注意此时主机要给应答0直到循环读取结束才给非应答1，由于是循环读取多个字节所以这里可能需要用数组来存储，具体实现细节有待进一步思考）

# 读取设备ID

> ![image-20230210164053891](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210164053891.png)

可以封装成一个函数：

> ![image-20230210174215850](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210174215850.png)

# 解除芯片睡眠模式

> ![image-20230210164152672](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210164152672.png)

# 写入和读取测试

> ![image-20230210164550923](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210164550923.png)

> ![image-20230210164605216](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210164605216.png)

# MPU6050初始化函数

我们这里先用宏定义将芯片寄存器的地址用语义清晰的字符串表示出来：我们将这些宏定义放在一个.h文件里，再在MPU6050.c文件中包含这个.h文件：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210164957571.png" alt="image-20230210164957571" style="zoom:50%;" />
>
> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165020833.png" alt="image-20230210165020833" style="zoom:50%;" />
>
> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165033253.png" alt="image-20230210165033253" style="zoom:50%;" />

函数配置步骤：

1. 初始化I2C通信

2. 配置电源管理器1，解除芯片睡眠模式（芯片睡眠模式会导致写芯片的寄存器失效，但能读）。这要配置电源管理寄存器：

    > ![image-20230210165146835](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165146835.png)
    >
    > 不复位0、不睡眠0、不循环0、无关位0、温度传感器不失能0、选择陀螺仪X轴时钟001
    >
    > 所以该寄存器就给0x01

3. 配置电源管理器2：

    > ![image-20230210165356993](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165356993.png)
    >
    > 循环模式唤醒频率不需要00、六个轴都不待机000000
    >
    > 该寄存器写入0x00

4. 配置采样率分频：

    > ![image-20230210165531028](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165531028.png)
    >
    > 这8位决定分频值，值越小则越快
    >
    > 这里我们给0x09也就是10分频

5. 配置配置寄存器：

    > ![image-20230210165658241](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210165658241.png)
    >
    > 无关位00、外部同步不需要000、数字低通滤波器110（这是最平滑的滤波）
    >
    > 该寄存器写入0x06

6. 陀螺仪配置寄存器：

    > ![image-20230210171122949](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210171122949.png)
    >
    > 自测不使能000（手册写漏了，并不是无关位）、最大量程11、无关位000
    >
    > 该寄存器写入0x18

7. 加速度计配置寄存器：

    > ![image-20230210171258095](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210171258095.png)
    >
    > 自测不使能000、最大量程11、高通滤波器不用000
    >
    > 该寄存器写入0x18

这样初始化配置就完成了，目前的配置是：解除睡眠、选择陀螺仪时钟，6轴均不待机，采样分频为10，数字低通滤波输出给最大、陀螺仪和加速度计都给最大量程。

函数代码如下：

## 函数代码

> ![image-20230210171548323](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210171548323.png)

此时配置完这些寄存器后MPU内部就在连续不断地进行数据转换了，输出的数据就存放在这些寄存器里：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210171646247.png" alt="image-20230210171646247" style="zoom:50%;" />

接下来我们要获取数据的话就只需要再写一个获取数据寄存器的函数即可：

# MPU6050数据获取函数

> ![image-20230210172339071](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210172339071.png)
>
> ![image-20230210172441778](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210172441778.png)

- 我们这里的程序连续调用了指定地址读一个字节的函数12次，如果要进一步提高读取效率的话可以利用指定地址读多个字节的时序：

    因为我们可以看到实际上这些数据寄存器的地址是连续的：

    > <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210172714343.png" alt="image-20230210172714343" style="zoom:50%;" />

    所以我们每读取一个字节后，芯片就可以再给主机发送你下一个地址的寄存器单元的内容，这样效率可以提高。

最后在主函数中显示这些数据，主函数程序如下：

> ![image-20230210173639634](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210173639634.png)

# 计算重力加速度g

> ![image-20230210173722387](D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210173722387.png)

这里我们OLED第一列第三行显示的是Z轴受力值的编码值，将其换算为重力加速度：

因为我们选择满量程是16g，设重力加速度为x，则有：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210173925832.png" alt="image-20230210173925832" style="zoom:50%;" />

由此算得x为：

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\7 通信\Note\image-20230210173947591.png" alt="image-20230210173947591" style="zoom:50%;" />

其他数据的编码值与实际值换算都可以参考本案例。




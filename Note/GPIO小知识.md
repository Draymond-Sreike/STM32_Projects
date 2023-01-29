# GPIO小知识

## 知识1（电源的VCC和GND可以接到单片机的IO口）

> <img src="D:\大学\单片机学习\MCU Learning Resource\STM32\STM32_Projects\Note\image-20230129224120679.png" alt="image-20230129224120679" style="zoom:50%;" />

**电源的VCC和GND可以接到单片机的IO口**（如上图所示接到了PB6和PB7），在我们程序不初始化这两个引脚的情况下是没有问题的（因为不认为初始化的情况下，**IO口默认为浮空输入模式**，在这个模式下，引脚不会输出电平，所以不会有任何影响）

## 知识2（GPIO可以直接提供OLED的VCC和GND）

**GPIO可以直接提供OLED的VCC和GND**。我们仍然用上图，如果PB6要接OLED的GND，PB7要接OLED的VCC，那么我们程序可以配置PB6输出低电平，PB7输出高电平，这样可以让GPIO口直接给OLED供电。

这是因为OLED的功率很小，所以IO口可以驱动OLED。

**但是！用GPIO口供电并不符合工程规范，所以实际项目中要避免使用这种方法！**
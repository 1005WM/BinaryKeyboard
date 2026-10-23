# RGB 引脚持续高电平测试（CH592F 五键款）

本 demo 直接复用仓库 `hal/include/kbd_config.h` 的引脚定义、WCH SDK、启动文件和应用链接脚本。
它是独立固件，正常键盘固件的源码与默认编译行为不变。

| 引脚 | 板上用途 | demo 上电初始化完成后 |
| --- | --- | --- |
| PA9 / `WS2812_EN_PIN` | RGB 电源使能，高电平上电 | 持续高电平 |
| PA10 / `WS2812_PIN` | WS2812 DIN 数据线 | 持续高电平 |

先让 DIN 保持低电平，再打开 RGB 电源；等待 300µs 后将 DIN 拉高。
关闭占用 PA10 的 LSE、TMR1 PWM/DMA，不启动灯效、USB、蓝牙、按键扫描或睡眠。
demo 不读写键位及灯效配置，但运行 demo 时键盘输入和 Studio 连接不可用。
**持续高电平用于测量 GPIO；WS2812 需要有效的数据时序，因此此 demo 不保证灯珠点亮。**

## 编译

安装与正常 CH592F 固件相同的 MounRiver RISC-V 工具链、CMake 和 Ninja，在仓库根目录运行：

```sh
cmake -S firmware/CH592F/demos/rgb_gpio_high -B firmware/CH592F/build/rgb-gpio-high -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build firmware/CH592F/build/rgb-gpio-high
```

输出 `firmware/CH592F/build/rgb-gpio-high/CH592F_RGB_GPIO_HIGH-app.bin` 和 `-app.hex`，应用起始地址为 `0x1000`。
应用文件不包含引导程序，不要当成地址 0 的完整镜像使用。

也可在 GitHub Actions 运行 **CH592F RGB GPIO High Demo**。
推送本目录或该工作流的修改至 `main` 时也会编译。
它额外复用仓库已有 JumpIAP 和高地址 IAP，生成 `CH592F_RGB_GPIO_HIGH-full.bin` / `-full.hex`。
测试产物仅上传到对应 Actions artifact，不作为正式固件发布。

## 测试及恢复

1. 按住 BOOT 插入 USB，进入芯片 ROM 下载模式，按[无线版刷写指南](../../../../docs/wireless/flash.md)刷入 demo 的 `-full.hex`。
2. 重新上电，用万用表或示波器测量 PA9、PA10 相对 GND 的电压，应为持续高电平，接近 MCU 的 I/O 供电电压。
3. 测试完成，再次按住 BOOT 插入 USB，用 ISP 刷回之前的正常五键款完整固件。

demo 本身没有 USB/Studio 更新功能，恢复使用上面的 BOOT + ISP 方式。
实际电压和电路是否正常须以实板测量为准。

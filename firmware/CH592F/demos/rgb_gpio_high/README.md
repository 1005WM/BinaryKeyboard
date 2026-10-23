# RGB 引脚持续高电平测试（CH592F 五键款）

本 demo 直接复用仓库 `hal/include/kbd_config.h` 的引脚定义、WCH SDK、启动文件和应用链接脚本。
它是独立固件，正常键盘固件的源码与默认编译行为不变。

| 引脚 | 板上用途 | demo 上电初始化完成后 |
| --- | --- | --- |
| PA9 / `WS2812_EN_PIN` | RGB 电源使能，高电平上电 | 持续高电平 |
| PA10 / `WS2812_PIN` | WS2812 DIN 数据线 | 持续高电平 |

先让 DIN 保持低电平，再打开 RGB 电源；等待 300µs 后将 DIN 拉高。
切换到内部 LSI 后关闭外部 LSE，停用 TMR1 PWM/DMA/中断，并把 TMR1 复用移到 PB10。
主循环直接持续设置 PA10 的输出锁存器和推挽输出方向；不启动灯效、USB、蓝牙、按键扫描或睡眠。
demo 不读写键位及灯效配置，但运行 demo 时键盘输入和 Studio 连接不可用。
**持续高电平用于测量 GPIO；WS2812 需要有效的数据时序，因此此 demo 不保证灯珠点亮。**

## 编译

安装与正常 CH592F 固件相同的 MounRiver RISC-V 工具链、CMake 和 Ninja，在仓库根目录运行：

```sh
cmake -S firmware/CH592F/demos/rgb_gpio_high -B firmware/CH592F/build/rgb-gpio-high -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build firmware/CH592F/build/rgb-gpio-high
```

输出 `firmware/CH592F/build/rgb-gpio-high/CH592F_RGB_GPIO_HIGH_V2-app.bin` 和 `-app.hex`，应用起始地址为 `0x1000`。
应用文件不包含引导程序，不要当成地址 0 的完整镜像使用。

也可在 GitHub Actions 运行 **CH592F RGB GPIO High Demo**。
推送本目录或该工作流的修改至 `main` 时也会编译。
它额外复用仓库已有 JumpIAP 和高地址 IAP，生成 `CH592F_RGB_GPIO_HIGH_V2-full.bin` / `-full.hex`。
测试产物仅上传到对应 Actions artifact，不作为正式固件发布。

## 测试及恢复

1. 按住 BOOT 插入 USB，进入芯片 ROM 下载模式，按[无线版刷写指南](../../../../docs/wireless/flash.md)刷入 demo 的 `-full.hex`。
2. 重新上电，用万用表或示波器测量 PA9、PA10 相对 GND 的电压，应为持续高电平，接近 MCU 的 I/O 供电电压。
3. 测试完成，再次按住 BOOT 插入 USB，用 ISP 刷回之前的正常五键款完整固件。

demo 本身没有 USB/Studio 更新功能，恢复使用上面的 BOOT + ISP 方式。
实际电压和电路是否正常须以实板测量为准。

## V2 排查 PA10 没有高电平

V1 已关闭 LSE 电源和 PWM，但收到 PA9 高、PA10 不高的实板反馈。
V2 增加显式 LSI 时钟源选择、TMR1 引脚重映射和 PA10 寄存器持续写高，以排查软件复用或输出状态因素。
这些改动尚未证明是故障根因，也无法消除外部电路的短路或负载。
若 V2 仍不高，请分别记录 MCU PA10 焊盘和灯珠 DIN 相对 GND 的电压；
不要只根据灯是否亮来判断 PA10 电平。此测试仍不发送 WS2812 点灯数据。

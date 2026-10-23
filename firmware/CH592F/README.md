# CH592F 固件

- [无线版开发指南](../../docs/wireless/dev.md)
- [无线版固件刷写](../../docs/wireless/flash.md)
- [便捷开发工具](../../docs/wireless/dev-tools.md)

## 本仓库的 DeepSeek Harness 启动键

CH592F 的第 1 个普通按键（K1，索引 0）在所有层发送 `Ctrl+Alt+F9`。
Windows 通过桌面快捷方式启动 `D:\deepseek_harness\DeepSeek Harness.exe`。
启动键适用于五键款和旋钮款的 USB / BLE 模式；FN 与 BOOT 层切换功能保持原有行为。

在 Windows 上运行一次：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\scripts\install-deepseek-shortcut.ps1
```

脚本会在当前用户桌面创建 `BinaryKeyboard - DeepSeek Harness.lnk`，不需要管理员权限或后台脚本。
可先用电脑键盘按 `Ctrl+Alt+F9` 验证；请保留此快捷方式，否则 Windows 无法响应启动热键。
如果程序迁移位置，使用 `-ProgramPath '新的完整 exe 路径'` 重新运行脚本。

推送固件修改至 `main` 会触发 `.github/workflows/release.yml`。
编译完成后，在 Actions 下载对应 CH592F 型号的固件并按上面的刷写指南升级。
刷写不会自动在电脑上安装快捷方式，因此每台使用的 Windows 电脑都需要运行一次安装脚本。

启动键在有效映射读取时固定覆盖 K1，因此已有 Flash 配置也能生效，无需恢复出厂设置。
Studio 读取的存储配置仍会显示 K1 原来的映射，修改该存储映射不会改变启动键；
其余映射及宏数据保留。按下会发送组合键，松开会释放，长按由 Windows 的正常按键行为处理。

## 五键款常用快捷键

CH592F **五键款（5KEY）** 在所有层使用以下映射，适用于 USB 和 BLE：

| 按键 | 功能 | 发送按键 |
| --- | --- | --- |
| K1 | 保持 DeepSeek Harness 启动功能 | Ctrl+Alt+F9 |
| K2 | 打开 Windows 截图选区 | Win+Shift+S |
| K3 | 回车 | Enter |
| K4 | 复制当前选中的内容 | Ctrl+C |
| K5 | 粘贴剪贴板内容 | Ctrl+V |

K5 不再处理单击换色、双击切换灯效或长按调亮度；按下立即发送粘贴组合键，松开释放。
其余按键也通过正常 HID 按下/松开流程发送，不使用宏或定时延迟。

快捷键在有效映射读取处覆盖存储映射，已有 Flash 配置无需恢复出厂设置。
Studio 仍显示存储的原映射，修改该存储映射不会改变上述固定快捷键。
FN 功能和 BOOT + 普通按键切换层保留，旋钮款 K2 及其余映射不变。

此固件是正常键盘固件，启动 USB/BLE 和原有 RGB 驱动；此前的 GPIO 高电平测试 demo 为独立项目。
截图功能会打开选区界面，需要用户选择截图范围。

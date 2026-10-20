# CH592F 固件

- [无线版开发指南](../../docs/wireless/dev.md)
- [无线版固件刷写](../../docs/wireless/flash.md)
- [便捷开发工具](../../docs/wireless/dev-tools.md)

## 本仓库的 DeepSeek Harness 启动键

CH592F 的第 1 个普通按键（K1，索引 0）在所有层发送 `Ctrl+Alt+F9`。
Windows 通过桌面快捷方式启动 `D:\deepseek_harness\DeepSeek Harness.exe`。
五键款和旋钮款的 USB / BLE 模式均适用；其他按键、旋钮、FN 与 BOOT 层切换功能保持原有行为。

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

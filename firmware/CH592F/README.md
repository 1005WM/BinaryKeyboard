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

## 五键款第 5 键控制背光

CH592F **五键款（5KEY）** 的第 5 个普通按键（K5，索引 4）在所有层控制按键背光：

| 操作 | 功能 |
| --- | --- |
| 单击 | 切换颜色，并切到静态单色灯效 |
| 双击 | 常亮 → 呼吸 → 闪烁 → 彩虹 → 常亮，保留所选颜色和亮度 |
| 长按约 800ms | 亮度 20% → 40% → 60% → 80% → 100% → 20%，每次长按只切换一档 |

双击要求第二次按下距第一次松开小于 300ms。为了区分单击与双击，单击在松开约 300ms 后生效。
双击只切换灯效，长按只调亮度，不额外触发单击换色。

单击颜色顺序：

`红 → 黄 → 绿 → 青 → 蓝 → 紫 → 白 → 红 …`

- 单击换色时启用静态单色灯效，打开背光；保留当前亮度，若亮度为零则恢复到默认 20%。
- 调节亮度时保留当前动画；若当前关闭背光或仅显示指示灯，则启用静态单色背光。
- 颜色、灯效与亮度保存在配置中，重新上电后保留；自定义颜色不在预设表中时，下次单击从红色开始。
- 指示灯仍用于连接、电量等状态显示，不跟随按键背光颜色改变。
- K1 的 DeepSeek Harness 启动功能以及 K2、K3、K4 保持原有行为。
- `BOOT + K5` 仍切换到第 5 层，不执行灯光手势。切换层、断开连接或切换 USB/BLE 时取消未完成手势。旋钮款的旋钮动作不受影响。

此功能在物理按键事件处覆盖 K5 的存储映射，已有配置无需恢复出厂设置；
Studio 仍会显示 K5 原有的存储映射，但实际执行上述灯光操作，不向电脑输入原来的 `5`。

手势逻辑的主机测试（从 `firmware/CH592F` 目录运行）：

```sh
cc -I keyboard/include tests/light_key_test.c keyboard/src/kbd_light_key.c -o light_key_test
./light_key_test
```

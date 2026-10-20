param(
    [string]$ProgramPath = 'D:\deepseek_harness\DeepSeek Harness.exe'
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $ProgramPath -PathType Leaf)) {
    throw "DeepSeek Harness executable not found: $ProgramPath"
}

$programFile = Get-Item -LiteralPath $ProgramPath
$desktopPath = [Environment]::GetFolderPath('DesktopDirectory')
if (-not $desktopPath -or -not (Test-Path -LiteralPath $desktopPath -PathType Container)) {
    throw 'Windows desktop directory not found.'
}

$shortcutPath = Join-Path $desktopPath 'BinaryKeyboard - DeepSeek Harness.lnk'
$shell = New-Object -ComObject WScript.Shell
$shortcut = $shell.CreateShortcut($shortcutPath)
$shortcut.TargetPath = $programFile.FullName
$shortcut.WorkingDirectory = $programFile.DirectoryName
$shortcut.IconLocation = $programFile.FullName + ',0'
$shortcut.Description = 'Launch DeepSeek Harness with BinaryKeyboard K1 (Ctrl+Alt+F9)'
$shortcut.Hotkey = 'CTRL+ALT+F9'
$shortcut.WindowStyle = 1
$shortcut.Save()

# Read back the saved shortcut, rather than assuming Save registered the fields.
$savedShortcut = $shell.CreateShortcut($shortcutPath)
if ($savedShortcut.TargetPath -ne $programFile.FullName -or
    $savedShortcut.Hotkey -notin @('Alt+Ctrl+F9', 'Ctrl+Alt+F9')) {
    throw 'The shortcut target or hotkey was not saved correctly.'
}

Write-Output "Installed: $shortcutPath"
Write-Output "Target: $($savedShortcut.TargetPath)"
Write-Output "Hotkey: $($savedShortcut.Hotkey)"
Write-Output 'Keep this shortcut on the desktop so Windows can handle the hotkey.'

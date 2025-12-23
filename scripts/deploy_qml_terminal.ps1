param(
    [string]$QtRoot = "$PSScriptRoot\..\3rdparty\Qt\6.8.3\msvc2022_64",
    [string]$Configuration = "Debug",
    [string]$Target = "qml_terminal",
    [string]$QmlDir = "$PSScriptRoot\..\src\qml"
)

$ErrorActionPreference = "Stop"

$exePath = Join-Path -Path $PSScriptRoot -ChildPath "..\build\bin\$Configuration\$Target.exe"
$qtDeploy = Join-Path -Path $QtRoot -ChildPath "bin\windeployqt.exe"

if (-not (Test-Path $qtDeploy)) {
    throw "windeployqt.exe not found at $qtDeploy"
}

if (-not (Test-Path $exePath)) {
    throw "Executable not found at $exePath"
}

& $qtDeploy --qmldir $QmlDir $exePath

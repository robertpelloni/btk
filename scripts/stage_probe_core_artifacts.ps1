param(
    [string]$MainBuildRoot = 'C:\Users\hyper\workspace\btk\build-vs2019',
    [string]$ProbeBuildRoot = 'C:\Users\hyper\workspace\btk\build-vs2019-script-probe5',
    [string]$Configuration = 'Release'
)

$sourceLib = Join-Path $MainBuildRoot ("lib\{0}\CsCore2.1.lib" -f $Configuration)
$sourceDll = Join-Path $MainBuildRoot ("bin\{0}\CsCore2.1.dll" -f $Configuration)
$destLibDir = Join-Path $ProbeBuildRoot ("lib\{0}" -f $Configuration)
$destDllDir = Join-Path $ProbeBuildRoot ("bin\{0}" -f $Configuration)
$destLib = Join-Path $destLibDir 'CsCore2.1.lib'
$destDll = Join-Path $destDllDir 'CsCore2.1.dll'

if (!(Test-Path $sourceLib)) {
    throw "Missing source import library: $sourceLib"
}

if (!(Test-Path $sourceDll)) {
    throw "Missing source runtime DLL: $sourceDll"
}

New-Item -ItemType Directory -Force -Path $destLibDir, $destDllDir | Out-Null

if (!(Test-Path $destLib)) {
    Copy-Item $sourceLib -Destination $destLib
}

if (!(Test-Path $destDll)) {
    Copy-Item $sourceDll -Destination $destDll
}

Get-Item $destLib, $destDll |
    Select-Object FullName, Length, LastWriteTime |
    Format-Table -AutoSize

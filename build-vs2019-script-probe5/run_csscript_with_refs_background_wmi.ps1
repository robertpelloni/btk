$out = 'C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\csscript-with-refs-background.out.log'
$err = 'C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\csscript-with-refs-background.err.log'

if (Test-Path $out) { Remove-Item $out -Force }
if (Test-Path $err) { Remove-Item $err -Force }

$cmd = 'cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe" "C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\src\script\CsScript.vcxproj" /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=true /m:1 /nologo /v:minimal "/clp:ErrorsOnly;Summary" 1>"C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\csscript-with-refs-background.out.log" 2>"C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\csscript-with-refs-background.err.log""'

$result = Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList $cmd
Write-Output "ReturnValue=$($result.ReturnValue)"
Write-Output "PID=$($result.ProcessId)"
Write-Output "OUT=$out"
Write-Output "ERR=$err"

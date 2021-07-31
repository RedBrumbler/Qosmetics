param($p1)

$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"

$OS = $PSVersionTable.OS
$windows = $OS.Contains("Windows")

# when core, or when on windows we want to add .cmd to the end
if ((-not ($PSVersionTable.PSEdition -eq "Core")) -or $windows) {
    $buildScript += ".cmd"
}
$coreCount = 0
#get amount of processors
# if on a windows system
if ($windows)
{
    $prop = WMIC CPU Get NumberOfCores 
    
    $coreCount = $prop.split('\n')[2]
    $coreCount = $coreCount.Trim()
}
# if on a linux system
else
{
    $coreCount = & nproc
}
# if all fails, just assume 4
if (-not $coreCount)
{
    $coreCount = 4
}

$msg = "Building with " + $coreCount + " Cores!"
echo $msg
if ($p1 -eq "test")
{
    & $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/AndroidTest.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j $coreCount -Onone
}
else
{
    & $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j $coreCount -Onone
}
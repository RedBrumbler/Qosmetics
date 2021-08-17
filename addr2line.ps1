param($p1, $p2)
$NDKPath = Get-Content ndkpath.txt

if ($p1 -and $p2)
{
    & $NDKPath\toolchains\llvm\prebuilt\windows-x86_64\bin\aarch64-linux-android-addr2line.exe -e .\obj\local\arm64-v8a\$p1 $p2
}
else
{
    if ($p1)
    {
        & $NDKPath\toolchains\llvm\prebuilt\windows-x86_64\bin\aarch64-linux-android-addr2line.exe -e .\obj\local\arm64-v8a\libquestcosmetics.so $p1
    }
    else
    {
        echo give at least 1 argument
    }
}
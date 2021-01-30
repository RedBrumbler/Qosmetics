# Builds a .zip file for loading with BMBF
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk
Compress-Archive -Path  "cover.gif",`
                        "./libs/arm64-v8a/libquestcosmetics.so",`
                        "./bmbfmod.json",`
                        "./extern/libbeatsaber-hook_1_0_12.so",`
                        "./ExtraFiles/IncludedItems/Plasma Katana.qsaber",`
                        "./ExtraFiles/IncludedItems/PC recreation.qwall",`
                        "./ExtraFiles/IncludedItems/PC recreation + Rainbow Frames.qwall",`
                        "./ExtraFiles/IncludedItems/UWU Notes.qbloq",`
                        "./ExtraFiles/IncludedItems/DDR Arrows.qbloq",`
                        "./ExtraFiles/UI/Icons/MenuIcon.png",`
                        "./ExtraFiles/UI/Icons/MenuIconSelected.png",`
                        "./ExtraFiles/UI/Icons/SaberIcon.png",`
                        "./ExtraFiles/UI/Icons/SaberIconSelected.png",`
                        "./ExtraFiles/UI/Icons/NoteIcon.png",`
                        "./ExtraFiles/UI/Icons/NoteIconSelected.png",`
                        "./ExtraFiles/UI/Icons/WallIcon.png",`
                        "./ExtraFiles/UI/Icons/WallIconSelected.png",`
                        "./ExtraFiles/UI/Icons/GameSetupIcon.png",`
                        "./ExtraFiles/UI/Icons/GameSetupIconSelected.png" -DestinationPath "./Qosmetics.zip" -Update

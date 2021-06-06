# Builds a .zip file for loading with BMBF
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 4
Compress-Archive -Path  "./libs/arm64-v8a/libbeatsaber-hook_1_3_5.so",`
                        "./extern/libbeatsaber-hook_1_3_3.so",`
                        "./libs/arm64-v8a/libquestcosmetics.so",`
                        "./libs/arm64-v8a/libbs-utils.so",`
                        "./libs/arm64-v8a/libcodegen_0_9_0.so",`
                        "./libs/arm64-v8a/libquestui.so",`
                        "./libs/arm64-v8a/libcustom-types.so",`
                        "./cover.gif",`
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
                        "./ExtraFiles/UI/Icons/GameSetupIconSelected.png",`
                        "./mod.json" -DestinationPath "./Qosmetics.zip" -Update

& copy-item -Force "./Qosmetics.zip" "./Qosmetics.qmod"
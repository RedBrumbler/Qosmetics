if (-not $env:qmodName) 
{
    $env:qmodName = "Qosmetics"
}

$zip = $env:qmodName + ".zip"
$qmod = $env:qmodName + ".qmod"

# if the qmod exists, rename it to zip to update it, we'll rename it back later
if (Test-Path $qmod) 
{
    & move-item -Force $qmod $zip
}

& Compress-Archive -Path  "./libs/arm64-v8a/libbeatsaber-hook_2_0_3.so",`
                        "./extern/libbeatsaber-hook_2_0_2.so",`
                        "./libs/arm64-v8a/libquestcosmetics.so",`
                        "./libs/arm64-v8a/libbs-utils.so",`
                        "./libs/arm64-v8a/libcodegen_0_10_2.so",`
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
                        "./mod.json" -DestinationPath $zip -Update
& move-item -Force $zip $qmod
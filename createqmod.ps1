Param(
    [String]$qmodname="",
    [Parameter(Mandatory=$false)]
    [Switch]$clean
)

if ($qmodName -eq "")
{
    echo "Give a proper qmod name and try again"
    exit
}
$mod = "./mod.json"
$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$filelist = @($mod)

$cover = "./" + $modJson.coverImage
if ((-not ($cover -eq "./")) -and (Test-Path $cover))
{ 
    $filelist += ,$cover
}

foreach ($mod in $modJson.modFiles)
{
        $path = "./build/" + $mod
    if (-not (Test-Path $path))
    {
        $path = "./extern/libs/" + $mod
    }
    $filelist += $path
}

foreach ($lib in $modJson.libraryFiles)
{
    $path = "./extern/libs/" + $lib
    if (-not (Test-Path $path))
    {
        $path = "./build/" + $lib
    }
    $filelist += $path
}

$bannedFiles = @("CreatorCache", "Patrons")

if (Test-Path "./ExtraFiles")
{
    $extraEntries = Get-ChildItem ./ExtraFiles/* -Recurse

    foreach ($entry in $extraEntries)
    {
        $mode = $entry | Select -Expand Mode
        if ($mode.Contains("d"))
        {
            continue
        }

        $doContinue = 0
        foreach ($ban in $bannedFiles)
        {
            if ($entry.Name.Contains($ban))
            {
                $doContinue = 1
                break
            }
        }

        if ($doContinue)
        {
            continue
        }

        $path = $entry | Select -expand fullname
        $filelist += ,$path
    } 
}
else
{
    echo "No ExtraFiles Directory Found"
}

$zip = $qmodName + ".zip"
$qmod = $qmodName + ".qmod"

if (($clean.IsPresent) -and (Test-Path $qmod))
{
    echo "Making Clean Qmod"
    Move-Item $qmod $zip -Force
}

Compress-Archive -Path $filelist -DestinationPath $zip -Update
Move-Item $zip $qmod -Force

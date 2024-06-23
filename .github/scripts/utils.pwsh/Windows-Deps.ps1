function Windows-Deps {
    $ProjectRoot = Resolve-Path -Path "$PSScriptRoot/../../.."
    $DepsDirectory = "$ProjectRoot/.deps"
    New-Item -ItemType Directory -Force -Path $DepsDirectory
    
    $BoostDirectory = "$DepsDirectory/boost"
    if(-Not (Test-Path -Path $BoostDirectory)) {
        Write-Output "Building Boost"
        $BoostUrl = "https://archives.boost.io/release/1.85.0/source/boost_1_85_0.7z"
        $BoostZip = "$DepsDirectory/boost.7z"
        Invoke-WebRequest -Uri $BoostUrl -OutFile $BoostZip
        Expand-ArchiveExt -Path $BoostZip -DestinationPath $DepsDirectory
        Move-Item -Path "$DepsDirectory/boost_1_85_0" -Destination $BoostDirectory
        Remove-Item -Path $BoostZip
        Set-Location -Path $BoostDirectory
        & ./bootstrap | Out-Default
        & ./b2 link=static --with-system --with-url --with-json | Out-Default
    } else {
        Write-Output "Boost directory exists, skipping build"
    }
}
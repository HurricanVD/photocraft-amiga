param(
    [string]$RepoRoot = (Get-Location)
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-MetadataValue {
    param([string]$Content, [string]$Name)

    $match = [System.Text.RegularExpressions.Regex]::Match($Content, "(?m)^\s*$Name\s*=\s*`"?([^`"\r\n]+)`"?\s*$")
    if ($match.Success) {
        return $match.Groups[1].Value.Trim()
    }
    return ""
}

function ConvertFrom-PosixDrivePath {
    param([string]$Path)

    if ([System.IO.Path]::DirectorySeparatorChar -ne "\") {
        return $Path
    }

    if ($Path -match '^/([A-Za-z])(?:/(.*))?$') {
        $drive = $Matches[1].ToUpperInvariant()
        $rest = $Matches[2]
        if ([string]::IsNullOrEmpty($rest)) {
            return "${drive}:\"
        }
        return ("{0}:\{1}" -f $drive, ($rest -replace '/', '\'))
    }

    if ($Path -match '^/mnt/([A-Za-z])(?:/(.*))?$') {
        $drive = $Matches[1].ToUpperInvariant()
        $rest = $Matches[2]
        if ([string]::IsNullOrEmpty($rest)) {
            return "${drive}:\\"
        }
        return ("{0}:\\{1}" -f $drive, ($rest -replace '/', '\\'))
    }

    return $Path
}

$root = [System.IO.Path]::GetFullPath($RepoRoot)
$metadataPath = Join-Path $root "docs/project-metadata.env"
if (-not (Test-Path -LiteralPath $metadataPath -PathType Leaf)) {
    throw "Missing docs/project-metadata.env"
}

$metadata = Get-Content -LiteralPath $metadataPath -Raw
$processRepo = Get-MetadataValue -Content $metadata -Name "VD_PROCESS_REPO"
$expectedVersion = Get-MetadataValue -Content $metadata -Name "VD_PROCESS_VERSION"

if ([string]::IsNullOrWhiteSpace($processRepo)) {
    throw "VD_PROCESS_REPO is not set"
}
if ([string]::IsNullOrWhiteSpace($expectedVersion)) {
    throw "VD_PROCESS_VERSION is not set"
}

$processRepo = ConvertFrom-PosixDrivePath -Path $processRepo

if ([System.IO.Path]::IsPathRooted($processRepo)) {
    $processRoot = $processRepo
} else {
    $processRoot = Join-Path $root $processRepo
    if (-not (Test-Path -LiteralPath $processRoot -PathType Container)) {
        $processRoot = Join-Path (Split-Path -Parent $root) $processRepo
    }
}

$versionPath = Join-Path $processRoot "VERSION"
if (-not (Test-Path -LiteralPath $versionPath -PathType Leaf)) {
    throw "Process VERSION not found: $versionPath"
}

$actualVersion = (Get-Content -LiteralPath $versionPath -Raw).Trim()
if ($actualVersion -ne $expectedVersion) {
    throw "Process version drift: expected $expectedVersion, got $actualVersion from $versionPath"
}

Write-Host "process version check: PASS"
Write-Host "version: $actualVersion"

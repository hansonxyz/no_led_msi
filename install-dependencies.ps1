# MSI LED Off Tool - Dependency Installation Script
# This script installs all required dependencies using Chocolatey

param(
    [switch]$Force
)

Write-Host "=== MSI LED Off Tool - Dependency Installation ===" -ForegroundColor Green
Write-Host ""

# Check if running as Administrator
if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "ERROR: This script must be run as Administrator!" -ForegroundColor Red
    Write-Host "Please restart PowerShell as Administrator and run this script again." -ForegroundColor Yellow
    exit 1
}

# Check if Chocolatey is installed
try {
    $chocoVersion = choco --version
    Write-Host "✓ Chocolatey is installed (version: $chocoVersion)" -ForegroundColor Green
} catch {
    Write-Host "ERROR: Chocolatey is not installed or not in PATH!" -ForegroundColor Red
    Write-Host "Please install Chocolatey first from: https://chocolatey.org/install" -ForegroundColor Yellow
    exit 1
}

Write-Host ""
Write-Host "Installing dependencies..." -ForegroundColor Yellow
Write-Host ""

# Install Visual Studio Build Tools 2019
Write-Host "Installing Visual Studio Build Tools 2019..." -ForegroundColor Cyan
try {
    if ($Force) {
        choco install visualstudio2019buildtools --params "--add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows10SDK.19041" -y --force
    } else {
        choco install visualstudio2019buildtools --params "--add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows10SDK.19041" -y
    }
    Write-Host "✓ Visual Studio Build Tools 2019 installed" -ForegroundColor Green
} catch {
    Write-Host "⚠ Visual Studio Build Tools installation may have failed" -ForegroundColor Yellow
}

# Install CMake
Write-Host "Installing CMake..." -ForegroundColor Cyan
try {
    if ($Force) {
        choco install cmake -y --force
    } else {
        choco install cmake -y
    }
    Write-Host "✓ CMake installed" -ForegroundColor Green
} catch {
    Write-Host "⚠ CMake installation may have failed" -ForegroundColor Yellow
}

# Install Git (useful for development)
Write-Host "Installing Git..." -ForegroundColor Cyan
try {
    if ($Force) {
        choco install git -y --force
    } else {
        choco install git -y
    }
    Write-Host "✓ Git installed" -ForegroundColor Green
} catch {
    Write-Host "⚠ Git installation may have failed" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "=== Installation Complete ===" -ForegroundColor Green
Write-Host ""

# Check if we need to refresh environment
Write-Host "Refreshing environment variables..." -ForegroundColor Cyan
refreshenv

Write-Host ""
Write-Host "=== Next Steps ===" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. RESTART YOUR COMMAND PROMPT/POWERSHELL SESSION" -ForegroundColor Red
Write-Host "   This is required for the new PATH variables to take effect." -ForegroundColor White
Write-Host ""
Write-Host "2. Verify installations by running:" -ForegroundColor White
Write-Host "   cmake --version" -ForegroundColor Cyan
Write-Host "   where cl.exe" -ForegroundColor Cyan
Write-Host ""
Write-Host "3. If Visual Studio Build Tools didn't install properly:" -ForegroundColor White
Write-Host "   - Download Visual Studio Installer from: https://visualstudio.microsoft.com/downloads/" -ForegroundColor Cyan
Write-Host "   - Install 'Build Tools for Visual Studio 2019' or 'Visual Studio Community 2019'" -ForegroundColor Cyan
Write-Host "   - Make sure to select:" -ForegroundColor Cyan
Write-Host "     * C++ build tools workload" -ForegroundColor Cyan
Write-Host "     * Windows 10 SDK (latest version)" -ForegroundColor Cyan
Write-Host "     * MSVC v142 compiler toolset" -ForegroundColor Cyan
Write-Host ""
Write-Host "4. Once everything is installed, run:" -ForegroundColor White
Write-Host "   build.bat" -ForegroundColor Cyan
Write-Host ""
Write-Host "5. Alternative: If you have Visual Studio 2022 installed," -ForegroundColor White
Write-Host "   you can modify CMakeLists.txt to use 'Visual Studio 17 2022' instead" -ForegroundColor Cyan
Write-Host ""
Write-Host "=== Troubleshooting ===" -ForegroundColor Yellow
Write-Host ""
Write-Host "If you encounter issues:" -ForegroundColor White
Write-Host "• Make sure you're running as Administrator" -ForegroundColor Cyan
Write-Host "• Restart your terminal after installation" -ForegroundColor Cyan
Write-Host "• Check that CMake and Visual Studio tools are in your PATH" -ForegroundColor Cyan
Write-Host "• For manual Visual Studio installation, ensure C++ workload is selected" -ForegroundColor Cyan
Write-Host ""
Write-Host "Run this script with -Force parameter to reinstall all dependencies." -ForegroundColor Gray
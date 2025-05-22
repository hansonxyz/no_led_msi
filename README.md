# MSI LED Off Tool

A standalone Windows application that turns off RGB LED lights on MSI motherboards.

(This tool was created by Claude AI)

## Requirements

- Windows 10/11
- MSI motherboard with supported chipset
- Administrator privileges (required for hardware access)
- Visual Studio 2019 or later with C++ development tools
- CMake 3.16 or later

## Building

1. Open Command Prompt as Administrator
2. Navigate to this directory
3. Run `build.bat`

The executable will be created at `build\Release\no_led_msi.exe`

## Usage

1. Run `no_led_msi.exe` as Administrator
2. The program will automatically detect if you have an MSI motherboard
3. If supported, it will turn off the RGB LEDs and exit
4. No additional configuration is required

## Technical Details

This tool is based on the MSIRGB project and uses the same low-level hardware access methods:

- Uses the inpoutx64.sys driver for direct port I/O access
- Accesses the Super I/O chip registers to control LED settings
- Specifically sets the LED disable bits in the RGB control registers

The tool only performs the LED disable operation and does not include GUI or advanced features.

## Supported Motherboards

This tool supports the same motherboards as the original MSIRGB project. If your motherboard is not supported, the program will display an error message.

## Notes

- The driver (inpoutx64.sys) is automatically installed and removed by the program
- Administrator privileges are required for driver installation and hardware access
- The program will exit immediately after turning off the LEDs

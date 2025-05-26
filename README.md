# MSI LED Off Tool

A standalone Windows application that turns off RGB LED lights on MSI motherboards.

## Usage

Running no_led_msi.exe will detect and turn off the RGB LEDs if your motherboard is supported.

To make this effect permanent, open up "Task Scheduler" and create a task which runs as highest permissions, as the current user, triggered when the workstation unlocks, and another trigger for when the workstation logs in, to run the program periodically.  I create my triggers with a 15 second delay.  This should cover all the bases on keeping the RGB LEDs off permanently on your suystem,

## Building

Requires:

- Visual Studio 2019 or later with C++ development tools
- CMake 3.16 or later

Alternatively, you may install the dependencies to build automatically, by running "install-dependencies.ps1" within powershell.  This tool assumes you have chocolatey installed. 

Once the dependencies are in place, run build.bat, and you should have a compiled binary.  The executable will be created at `build\Release\no_led_msi.exe`

## Technical Details

This tool is based on the ixjf/MSIRGB project and uses the same low-level hardware access methods:

- Uses the inpoutx64.sys driver for direct port I/O access
- Accesses the Super I/O chip registers to control LED settings
- Specifically sets the LED disable bits in the RGB control registers

The tool only performs the LED disable operation and does not include GUI or advanced features.

This tool was created algoritmically by Claude AI.

## Supported Motherboards

As this is based on ixjf/MSIRGB, the same list of supported motheboards should apply:



    This repository is archived, therefore this list may be out-of-date. The following list was correct at the time of archival. More motherboards may work.
    
    B450I GAMING PLUS AC (works)
    X470 GAMING PLUS (works)
    X470 GAMING PRO (works)
    X470 GAMING PRO MAX (works)
    Z270 GAMING M7 (works)
    B450 TOMAHAWK (works)
    B450 TOMAHAWK MAX (works)
    H370 GAMING PLUS (works)
    B450M MORTAR (works)
    B450M MORTAR TITANIUM (works)
    B450M BAZOOKA PLUS (works)
    B250M MORTAR (works)
    B350M PRO-VDH (works)
    B450M BAZOOKA V2 (works)
    B450 GAMING PLUS (works)
    B250M BAZOOKA (works)
    Z270 GAMING M6 AC (works)
    Z270 GAMING M5 (works)
    B360M GAMING PLUS (works)
    X370 GAMING PLUS (works)
    B450-A PRO MAX (works)
    B450M PRO-VDH MAX (works)
    MPG Z390 GAMING PLUS (works)
    B450 GAMING PLUS MAX (works)
    B450M MORTAR MAX (works)
    B450M PRO-VDH PLUS (works)
    A320M GAMING PRO (works)
    B365M PRO-VDH (works)
    Z370 PC PRO (works)
    B350 TOMAHAWK (works despite giving warning message on start)
    B350 GAMING PLUS (works despite giving warning message on start)
    B350 PC MATE (works despite giving warning message on start)
    A320M BAZOOKA (works despite giving warning message on start)
    B350M MORTAR ARTIC (works despite giving warning message on start)
    B350 KRAIT GAMING (works despite giving warning message on start)
    X370 KRAIT GAMING (works despite giving warning message on start)
    B350M MORTAR (works despite giving warning message on start)
    Z270 GAMING M3 (works despite giving warning message on start)
    H270 GAMING PRO CARBON (works despite giving warning message on start)
    B350M BAZOOKA (works despite giving warning message on start)
    B250M GAMING PRO (works despite giving warning message on start)
    B350 GAMING PRO CARBON (works despite giving warning message on start)
    B350I PRO AC (untested, should work)
    B350M GAMING PRO (untested, should work)
    A320M GRENADE (untested, should work)
    B450M PRO-VDH (untested, should work)
    B450M BAZOOKA (untested, should work)
    Z370 OC GAMING (untested, should work)
    Z370 GAMING PLUS (untested, should work)
    Z370M MORTAR (untested, should work)
    Z370-A PRO (untested, should work)
    Z370 GAMING PRO AC (untested, should work)
    Z270 SLI PLUS (untested, should work)
    Z270 KRAIT GAMING (untested, should work)
    Z270 GAMING PRO (untested, should work)
    Z270 TOMAHAWK (untested, should work)
    H270 TOMAHAWK ARTIC (untested, should work)
    Z299M-A PRO (untested, should work)
    X299 RAIDER (untested, should work)
    X399 GAMING PRO CARBON AC (untested, should work)
    X399 SLI PLUS (untested, should work)
    X299M GAMING PRO CARBON AC (untested, should work)
    Z270 XPOWER GAMING TITANIUM (untested, should work)
    H270 GAMING M3 (untested, should work)
    B250M MORTAR ARTIC (untested, should work)
    B250M PRO-VDH (untested, should work)
    B250 PC MATE (untested, should work)
    H270 PC MATE (untested, should work)
    7A78 (revision >1.x, untested, should work)
    B250M BAZOOKA OPT BOOST (untested, should work)
    H310M PRO-VL (untested, should work)
    B360M MORTAR TITANIUM (untested, should work)
    B360M PRO-VDH (untested, should work)
    B360M BAZOOKA (untested, should work)
    H310M PRO-VHL (untested, should work)
    7B30 (untested, should work)
    B360I GAMING PRO AC (untested, should work)
    MPG Z390I GAMING EDGE AC (untested, should work)
    MAG Z390M MORTAR (untested, should work)
    Z390-A PRO (untested, should work)
    Z390 PLUS? / 7C22 (untested, should work)
    7C24 (untested, should work)
    7C01 (untested, should work)
    B450A-PRO (untested, should work)
    B450M GAMING PLUS (untested, should work)
    B350M PRO-VDH (untested, should work)
    X470 GAMING PLUS MAX (untested, should work)


## Notes

- The driver (inpoutx64.sys) is automatically installed and removed by the program
- Administrator privileges are required for driver installation and hardware access
- The program will exit immediately after turning off the LEDs

cmake_minimum_required(VERSION 3.16)
project(no_led_msi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(no_led_msi no_led_msi.cpp)

# Link required Windows libraries directly
target_link_libraries(no_led_msi 
    wbemuuid
    comsuppw
    ole32
    oleaut32
    shell32
    advapi32
)

# Set subsystem to console and require administrator privileges
set_target_properties(no_led_msi PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE /MANIFESTUAC:level='requireAdministrator'"
)
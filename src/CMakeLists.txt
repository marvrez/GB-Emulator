# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_sources(
    main.cpp
    register.cpp
    address.cpp
    mmu.cpp
    mbc.cpp
    cartridge.cpp
    gameboy.cpp
    keypad.cpp
    timer.cpp
    apu.cpp

    util.h
    cartridge_util.h
    boot_roms.h
)
add_subdirectory(cpu)
add_subdirectory(gpu)
add_subdirectory(incl)

#ifndef PLATFORM_WINDOWS

    #include "DetectDrives.hpp"
    #include <iostream>

static const fs::path fake_drive = "/mnt/USB";

// Just pretend that it's a real drive, the only purpose of this is testing on
// non-windows systems
std::optional<fs::path> get_new_drive_path() {
    std::cout << "Press a key to start copying from " << fake_drive
              << std::endl;
    std::cin.get();
    return fake_drive;
}

#endif
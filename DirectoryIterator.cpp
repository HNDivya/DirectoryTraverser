#include "DirectoryIterator.h"
#include <iostream>

DirectoryIterator::DirectoryIterator(const std::string& path, EventDispatcher& dispatcher)
    : path_(path), dispatcher_(dispatcher) {}

void DirectoryIterator::operator()() const {
    auto start = std::chrono::high_resolution_clock::now();
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path_, fs::directory_options::skip_permission_denied)) {
            dispatcher_.notify(entry.path().string());
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error iterating directory " << path_ << ": " << e.what() << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Log to file
    std::ofstream log_file("log.txt", std::ios_base::app);
    if (log_file.is_open()) {
        log_file << "Thread ID: " << std::this_thread::get_id()
            << " processed directory: " << path_
            << " in " << duration.count() << " ms." << std::endl;
    }
}

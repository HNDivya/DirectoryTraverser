#ifndef DIRECTORY_ITERATOR_H
#define DIRECTORY_ITERATOR_H

#include <string>
#include <functional>
#include <filesystem>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include "EventDispatcher.h"

namespace fs = std::filesystem;

class DirectoryIterator {
public:
    DirectoryIterator(const std::string& path, EventDispatcher& dispatcher);
    void operator()() const;

private:
    std::string path_;
    EventDispatcher& dispatcher_;
};

#endif

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include "ThreadPool.h"
#include "EventDispatcher.h"
#include "DirectoryIterator.h"

// Shared data structure
std::deque<std::string> sharedQueue;
std::mutex mtx;
std::condition_variable cv;
bool done = false;

// Function to print the contents of the shared queue
void print_contents() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !sharedQueue.empty() || done; });

        while (!sharedQueue.empty()) {
            std::string path = sharedQueue.front();
            sharedQueue.pop_front();
            std::cout << path << std::endl;
        }

        if (done && sharedQueue.empty()) {
            break;
        }
    }
}

std::vector<std::string> read_directories_from_file(const std::string& filename) {
    std::vector<std::string> directories;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        directories.push_back(line);
    }
    return directories;
}

int main() 
{

    std::vector<std::string> directories = { "C:\\Users\\navee\\source\\repos\\", "C:\\"};

    EventDispatcher dispatcher;

    // Register the callback to add items to the shared queue
    dispatcher.registerCallback([](const std::string& path) {
        std::unique_lock<std::mutex> lock(mtx);
        sharedQueue.push_back(path);
        cv.notify_one();
        });

    // Create a thread pool with a suitable number of threads
    const size_t numThreads = std::thread::hardware_concurrency();
    ThreadPool pool(numThreads);

    // Enqueue directory iteration tasks
    std::vector<std::future<void>> futures;
    for (const auto& dir : directories) {
        futures.emplace_back(pool.enqueue(DirectoryIterator(dir, dispatcher)));
    }

    // Create and start the consumer thread
    std::thread consumer_thread(print_contents);

    // Wait for all directory iteration tasks to complete
    for (auto& future : futures) {
        future.get();
    }

    // Notify the consumer thread to finish
    {
        std::unique_lock<std::mutex> lock(mtx);
        done = true;
        cv.notify_one();
    }

    // Join the consumer thread
    consumer_thread.join();

    return 0;
}

#include "EventDispatcher.h"

void EventDispatcher::registerCallback(EventCallback callback) {
    std::unique_lock<std::mutex> lock(callbackMutex);
    callbacks.push_back(callback);
}

void EventDispatcher::notify(const std::string& path) {
    std::unique_lock<std::mutex> lock(callbackMutex);
    for (auto& callback : callbacks) {
        callback(path);
    }
}

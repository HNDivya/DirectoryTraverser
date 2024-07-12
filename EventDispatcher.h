#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <vector>
#include <functional>
#include <mutex>

class EventDispatcher {
public:
    using EventCallback = std::function<void(const std::string&)>;

    void registerCallback(EventCallback callback);
    void notify(const std::string& path);

private:
    std::vector<EventCallback> callbacks;
    std::mutex callbackMutex;
};

#endif // EVENTDISPATCHER_H

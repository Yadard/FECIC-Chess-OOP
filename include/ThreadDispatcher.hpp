#ifndef THREADDISPATCHER_HPP
#define THREADDISPATCHER_HPP

#include <SFML/System.hpp>
#include <memory>
#include <vector>

class ThreadDispatcher {

  public:
    ThreadDispatcher(ThreadDispatcher &other) = delete;
    void operator=(const ThreadDispatcher &) = delete;
    ~ThreadDispatcher();

    static auto GetInstance() -> ThreadDispatcher &;

    auto launchThread(sf::Thread *thread) -> void;

  private:
    ThreadDispatcher() = default;

    static std::unique_ptr<ThreadDispatcher> m_instance;

    std::vector<std::unique_ptr<sf::Thread>> m_threads;
};

#endif // THREADDISPATCHER_HPP
#include "ThreadDispatcher.hpp"

std::unique_ptr<ThreadDispatcher> ThreadDispatcher::m_instance = nullptr;

ThreadDispatcher::~ThreadDispatcher() {
    for (auto it = m_threads.begin(); it != m_threads.end(); it++)
        it->get()->wait();
}

auto ThreadDispatcher::GetInstance() -> ThreadDispatcher & {
    if (!m_instance)
        m_instance.reset(new ThreadDispatcher());
    return *m_instance;
}

auto ThreadDispatcher::launchThread(sf::Thread *thread) -> void {
    m_threads.emplace_back(thread);
    thread->launch();
}
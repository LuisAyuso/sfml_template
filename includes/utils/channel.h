#pragma once

#include "utils/tools.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>


namespace myapp {

template <typename message_t>
class channel : public no_copy {

  public:
    channel() = default;

    void send(message_t &&msg) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.emplace_back(std::move(msg));
        }
        m_ready.notify_one();
    }

    message_t recv() {

        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_queue.empty()) {
            auto tmp = std::move(m_queue.back());
            m_queue.pop_back();
            return tmp;
        } else {
            m_ready.wait(lock);
            auto tmp = std::move(m_queue.back());
            m_queue.pop_back();
            return tmp;
        }
    }

  private:
    std::vector<message_t>  m_queue;
    std::mutex              m_mutex;
    std::condition_variable m_ready;
};

template <typename message_t>
using channel_ptr = std::shared_ptr<channel<message_t>>;

template <typename message_t>
class sender : public no_copy {

  public:
    sender(const channel_ptr<message_t> &ch_ptr)
      : m_queue(ch_ptr) {
    }
    sender(sender &&) = default;

    void send(message_t &&msg) {
        return m_queue->send(std::move(msg));
    }

  private:
    channel_ptr<message_t> m_queue;
};

template <typename message_t>
class receiver : public no_copy {

  public:
    receiver(const channel_ptr<message_t> &ch_ptr)
      : m_queue(ch_ptr) {
    }
    receiver(receiver &&) = default;

    message_t recv() {
        return m_queue->recv();
    }

  private:
    channel_ptr<message_t> m_queue;
};


template <typename message_t>
std::pair<sender<message_t>, receiver<message_t>>
make_producer_consumer_pair() {
    auto ptr = std::make_shared<channel<message_t>>();

    sender<message_t>   s(ptr);
    receiver<message_t> r(ptr);

    return std::make_pair<sender<message_t>, receiver<message_t>>(std::move(s),
                                                                  std::move(r));
}


} // namespace

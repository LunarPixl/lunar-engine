
#include <core/eventbus.hpp>

namespace lunar::core {
    void EventBus::publish(std::type_index index, void *ptr) {
        if(auto itr = m_handlers.find(index); itr != m_handlers.end())
        {
            std::invoke(itr->second,ptr);
        }
    }

    void EventBus::consume(std::type_index index, const delegate<void(void *)> &callback) {
        m_handlers[index] += callback;
    }
}
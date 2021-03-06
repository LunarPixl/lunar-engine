//
// Created by Raphael Baier on 05.03.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once

#include <unordered_map>
#include <impfast_delegate/delegate.h>
#include <impfast_delegate/multi_cast_delegate.h>
#include <typeindex>


namespace lunar::core {

    class EventBus{
    public:

        template <class Event>
        void publish(Event* ptr)
        {
            publish(typeid(Event), static_cast<void*>(ptr));
        }

        void publish(std::type_index index, void* ptr);

        template <class Event,class Func>
        void consume(Func&& f){
            static_assert(std::is_invocable<Func,Event*>::value,
                    "Func must be a function-like object & must accept exactly one parameter of type Event*");

            auto surrogate = delegate<void(void*)>::create([f](void* ptr)
            {
                std::invoke(f,reinterpret_cast<Event*>(ptr));
            });

            consume(typeid(Event),surrogate);
        }
        void consume(std::type_index index, const delegate<void(void *)> &callback);


    private:
        std::unordered_map<std::type_index,multicast_delegate<void(void*)>> m_handlers;
    };
}
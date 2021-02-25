//
// Created by rapha on 25.02.2021.
//

#pragma once
#ifndef STATEMACHINE_DEFINED
#error This cannot be included directly, include state_machine.hpp
#endif


namespace lunar::core {
class StateMachine;

struct state_base {
    virtual void start() {}
    virtual void update() {}
    virtual void stop() {}

    StateMachine * fsm;

};
}



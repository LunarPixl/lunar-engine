//
// Created by rapha on 25.02.2021.
//

#pragma once
#define STATEMACHINE_DEFINED

#include <core/static_errors.hpp>
#include <core/state_base.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <impfast_delegate/delegate.h>
#include <impfast_delegate/multi_cast_delegate.h>
#include <hedley.h>

namespace lunar::core {

class StateMachine {

public:

    /**
     * @brief Transitions to a new State in the FSM
     * @tparam To the type of the state you want to transition to
     */
    template <class To>
    void transition() noexcept {
        SE_MUST_INHERIT_FROM(To,state_base);
        transition(typeid(To));
    }

    /**
     * @brief Transitions to a new State in the FSM
     * @param type the typeid of the state you want to transition to
     */
    void transition(std::type_index type) noexcept;

    /**
     * @brief placement-creates a new state in the registry
     * @tparam State the type of the State you want to insert, must inherit from StateBase
     * @param args
     */
    template <class State, class ... Args>
    State& emplace(Args&& ... args)
    {
        SE_MUST_INHERIT_FROM(State,state_base);
        return static_cast<State&>(insert(typeid(State),std::make_unique<State>(std::forward<Args>(args)...)));
    }

    /**
     * @brief Inserts a new State into the FSM
     * @tparam State the type of the State you want to insert, must inherit from StateBase
     * @param s the State you want to insert
     */
    template <class State>
    State& insert(std::unique_ptr<State> s){
        SE_MUST_INHERIT_FROM(State,state_base);
        return static_cast<State&>(insert(typeid(State),std::unique_ptr<state_base>(static_cast<state_base*>(s.release()))));
    }


    /**
     * @brief Inserts a new State into the FSM by typename & ptr combo
     * @param index the typeid of the State you want to insert
     * @param state a pointer to the state you want to insert
     */
    state_base& insert(std::type_index index,std::unique_ptr<state_base> state) noexcept;

    /**
     * @brief starts the fsm
     * @param start the initial state
     */
    void run(std::type_index start);

    /**
     * @brief is run on every update tick
     */
     multicast_delegate<void(StateMachine*)> onUpdate;

    /**
     * @brief is run when a state transition occurs
     */
     multicast_delegate<void(StateMachine*,state_base* current,state_base* next)> onTransition;

    /**
     * @brief gets a pointer to the current state
     */
    HEDLEY_WARN_UNUSED_RESULT const state_base* current() const noexcept;

    /**
     * @brief gets a pointer to the next state
     */
    HEDLEY_WARN_UNUSED_RESULT const state_base* next() const noexcept;

    void stop();

private:

    void validate_index(std::type_index) const noexcept;

    enum class FsmState {
        RUNNING,
        TRANSITION,
        ENDING
    };

    FsmState m_state = FsmState::ENDING;
    state_base* m_current = nullptr;
    state_base* m_next = nullptr;
    std::unordered_map<std::type_index,std::unique_ptr<state_base>> m_state_registry;
};
}

//
// Created by rapha on 25.02.2021.
//

#include "state_machine.hpp"
#include <cassert>

namespace lunar::core {

    void StateMachine::transition(std::type_index type) noexcept {

        //check if the index is valid
        validate_index(type);

        //get the requested state
        const auto& ptr = m_state_registry.at(type);

        // set next state and request the fsm to transition
        m_next = ptr.get();
        m_state = FsmState::TRANSITION;
    }

    void StateMachine::validate_index(std::type_index index) const noexcept{

        //State must be in registry!
        assert(m_state_registry.find(index) != m_state_registry.end());
    }

    state_base& StateMachine::insert(std::type_index index, std::unique_ptr<state_base> state) noexcept {

        //setup fsm pointer and insert into registry
        state->fsm = this;
        return *m_state_registry.emplace(index,std::move(state)).first->second;
    }

    void StateMachine::run(std::type_index start) {

        //make sure the start index is valid
        validate_index(start);

        //setup fsm
        m_state = FsmState::RUNNING;

        //get initial state
        const auto& ptr = m_state_registry.at(start);
        m_next = m_current = ptr.get();

        m_current->start();

        while(m_state != FsmState::ENDING)
        {
            onUpdate(this);
            m_current->update();
            if(m_state == FsmState::TRANSITION) {

                // reset state flag
                m_state = FsmState::RUNNING;

                // Transition target was not set!
                assert(m_current != m_next);

                onTransition(this,m_current,m_next);
                // stop the old and start the new
                m_current->stop();
                m_next->start();

                // transition
                m_current = m_next;
            }
        }
        m_current->stop();
    }

    const state_base *StateMachine::current() const noexcept {
        return m_current;
    }

    const state_base *StateMachine::next() const noexcept{
        return m_next;
    }

    void StateMachine::stop() {
        m_state = FsmState::ENDING;
    }
}
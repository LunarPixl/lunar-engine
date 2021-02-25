//
// Created by rapha on 25.02.2021.
//

#pragma once


#include <core/state_machine.hpp>
#include <doctest/doctest.h>

#include <array>

using namespace lunar::core;

template <class id>
struct TestState : public state_base
{
    bool was_started;
    bool was_run;
    bool was_stopped;
    SA::delegate<void(StateMachine*)> onStart;
    SA::delegate<void(StateMachine*)> onUpdate;
    SA::delegate<void(StateMachine*)> onStop;


    id dummy_;

    void start() override {
        if(!onStart.isNull())
            onStart(fsm);

        was_started = true;
    }

    void update() override {
        if(!onUpdate.isNull())
            onUpdate(fsm);

        was_run = true;
    }

    void stop() override {
        if(!onStop.isNull())
            onStop(fsm);

        was_stopped = true;
    }
};



TEST_CASE("Placement is possible")
{
    StateMachine fsm;
    struct s1{};

    //make sure that current is nullptr when nothing is setup yet
    CHECK_EQ(fsm.current(),nullptr);

    //emplace new state
    fsm.emplace<TestState<s1>>();

    //shutdown fsm on first update
    fsm.onUpdate += [](StateMachine* fsm)
    {
        fsm->stop();
    };

    // start fsm
    fsm.run(typeid(TestState<s1>));

    //make sure that current is not nullptr
    CHECK_NE(fsm.current(),nullptr);

}

TEST_CASE("Transition is possible")
{
    StateMachine fsm;
    struct s1{};
    struct s2{};



    //emplace our test-states
    auto& tss1 = fsm.emplace<TestState<s1>>();
    auto& tss2 = fsm.emplace<TestState<s2>>();

    // instruct the first state to transition to the second one
    tss1.onUpdate = [](StateMachine* fsm){
        fsm->transition<TestState<s2>>();
    };

    tss2.onUpdate = [](StateMachine* fsm){
        fsm->stop();
    };


    //run the fsm
    fsm.run(typeid(TestState<s1>));

    //check the results
    CHECK_EQ(tss1.was_started, true);
    CHECK_EQ(tss1.was_run, true);
    CHECK_EQ(tss1.was_stopped, true);
    CHECK_EQ(tss2.was_started, true);
    CHECK_EQ(tss2.was_run, true);
    CHECK_EQ(tss2.was_stopped, true);

}

TEST_CASE("Transition is possible")
{
    StateMachine fsm;
    struct s1{};
    struct s2{};

    bool transition_happened = false;

    //emplace our test-states
    auto& tss1 = fsm.emplace<TestState<s1>>();
    auto& tss2 = fsm.emplace<TestState<s2>>();

    // instruct the first state to transition to the second one
    tss1.onStart = [](StateMachine* fsm){
        fsm->transition<TestState<s2>>();
    };

    //on transition make sure that all the variables are correct
    fsm.onTransition += [&](auto* fsm, auto current, auto next)
    {
        transition_happened =
                current == &tss1 &&
                next == &tss2;

        //also stop the fsm otherwise we never finish
        fsm->stop();
    };

    //run the fsm
    fsm.run(typeid(TestState<s1>));

    //check the results
    CHECK_EQ(transition_happened, true);
    CHECK_EQ(fsm.current(),&tss2);

}



//
// Created by rapha on 25.02.2021.
//

#include "screen.hpp"

void lunar::core::Screen::start() {
    early_setup();
    setup();
    late_setup();
}

void lunar::core::Screen::update() {
    using clk = std::chrono::high_resolution_clock;

    static float acc;

    static clk::time_point last = clk::now();
    clk::time_point now = clk::now();
    float delta = std::chrono::duration<float>(last - now).count();
    last = now;

    const float fixed_dt = 0.001f;

    acc += delta;

    while(acc >= fixed_dt)
    {
        acc -= fixed_dt;
        fixed_update(fixed_dt);
    }

    early_update(delta);
    update(delta);
    late_update(delta);

}

void lunar::core::Screen::stop() {
    early_shutdown();
    shutdown();
    late_shutdown();
}

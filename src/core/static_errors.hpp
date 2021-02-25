//
// Created by rapha on 25.02.2021.
//

#pragma once

#include <type_traits>

#define SE_MUST_INHERIT_FROM(Test,Base) static_assert(std::is_base_of_v<Base,std::decay<Test>::type>, "static_assertion failed! " #Test " must inherit from " #Base)
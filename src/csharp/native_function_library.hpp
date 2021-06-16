//
// Created by Raphael Baier on 10.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once

#include <csharp/loader.hpp>
#include <unordered_map>


namespace lunar::csharp {
class NativeFunctionLibrary {
public:
    explicit NativeFunctionLibrary(HostFXR* ptr);

    void register_function(void* fnptr,const std::string& fnname);

private:
};

}



#pragma once

#include "expected.hpp"
#include <cstring>

//
// Created by Raphael Baier on 27.02.2021.
// Copyright (c) 2021 Lunar Pixl Team

//


static constexpr int ERROR_PROGRAM_EXECUTION_FAILED = 127;
#include <expected.hpp>
#include <stdexcept>

#if _WIN32
#define WIN32_LEAN_AND_MEAN



#include <Windows.h>

inline tl::expected<int,std::runtime_error> invoke_external(const std::string& cmd)
{
    auto data = std::make_unique<char[]>(cmd.size()+1);
    memcpy(data.get(),cmd.data(),cmd.size());
    data[cmd.size()] = '\0';

    // generate startupinfo, since we do not want to get
    // stdout or stderr we can just zero it and call it a day
    STARTUPINFO si;
    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);

    //generate process_information, this will store the handle
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi,sizeof(pi));

    auto ret = CreateProcessA(NULL,data.get(),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    if(ret != NULL)
    {
        return tl::unexpected(std::runtime_error("Command Execution Failed"));
    }


    WaitForSingleObject(pi.hProcess,INFINITE);

    DWORD exit_code = NULL;
    GetExitCodeProcess(pi.hProcess,&exit_code);
    return static_cast<int>(exit_code);
}
#else
inline tl::expected<int,std::runtime_error> invoke_external(const std::string& cmd)
{
    auto data = std::make_unique<char[]>(cmd.size()+1);
    memcpy(data.get(),cmd.data(),cmd.size());
    data[cmd.size()] = '\0';

    //open process ... for reading I guess ?
    auto ptr = popen(data.get(),"r");
    if(ptr == NULL)
    {
        return tl::unexpected(std::runtime_error("Command Execution Failed"));
    }

    return pclose(ptr);
}
#endif
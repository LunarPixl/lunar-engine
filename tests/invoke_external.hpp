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
    auto data = std::make_unique<char[]>(cmd.size()+1+3);
    data[0] = '/';
    data[1] = 'c';
    data[2] = ' ';

    memcpy(data.get()+3,cmd.data(),cmd.size());
    data[cmd.size()+3] = '\0';

    // generate startupinfo, since we do not want to get
    // stdout or stderr we can just zero it and call it a day
    STARTUPINFO si;
    ZeroMemory(&si,sizeof(si));
    GetStartupInfo(&si);
    si.cb = sizeof(si);
    si.dwFlags =    STARTF_USESTDHANDLES;
    si.hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError =  ::GetStdHandle(STD_ERROR_HANDLE);


    //generate process_information, this will store the handle
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi,sizeof(pi));

    auto ret = CreateProcessA("C:\\Windows\\System32\\cmd.exe",data.get(),NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
    if(ret == NULL)
    {
        std::cerr << GetLastError() << std::endl;
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
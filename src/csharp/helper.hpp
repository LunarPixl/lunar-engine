//
// Created by Raphael Baier on 02.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once
#include <dotnet/hostfxr.h>
#include <string>
#include <windows.h>

#if defined(_WIN32) || defined(WIN32)
# define LUNAR_CSHARP_ANNOTATE_TEXT(TEXT) L ## TEXT
# define LUNAR_CSHARP_PATH_SEPERATOR L'\\'
#else
# define LUNAR_CSHARP_ANNOTATE_TEXT(TEXT) TEXT
# define LUNAR_CSHARP_PATH_SEPERATOR '/'
#endif


namespace lunar::csharp {

    using csharp_char = char_t;
#if defined(_WIN32) || defined(WIN32)
    // On Platform Windows the
    // Char type is WCHAR
    // And you need to free with
    // LocalFree(void*)

    inline void vm_free_memory(void * ptr)
    {
        LocalFree(ptr);
    }
#else
    // On Platform Linux
    // Normal char and free
    // can be used
    inline void vm_free_memory(void * ptr)
    {
        free(ptr);
    }
#endif

    using csharp_string = std::basic_string<csharp_char>;
}
//
// Created by Raphael Baier on 02.05.2021.
// Copyright (c) 2021 Lunar Pixl Team
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include <string> // std::string, std::string::c_string
#include <utility>

//This ugly macro block
//makes the interface for loading & unloading symbols and libraries more "uniform"
//This is the mapping:
// LUNAR_OPEN_MODULE -> LoadLibraryA or dlopen (Note that the MODE param is only used on linux)
// LUNAR_FREE_MODULE -> FreeLibrary or dlclose
// LUNAR_GET_PROC    -> GetProcAddress or dlsym
#if defined(_WIN32) || defined(WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   define LUNAR_OPEN_MODULE(PATH, MODE) ::LoadLibraryA((PATH))
#   define LUNAR_FREE_MODULE(LIBRARY)   ::FreeLibrary((LIBRARY))
#   define LUNAR_GET_PROC(LIBRARY, NAME) ::GetProcAddress((LIBRARY),(NAME))
#else
#   include <dlfcn.h>
#   define LUNAR_OPEN_MODULE(PATH,MODE) ::dlopen((PATH),(MODE))
#   define LUNAR_FREE_MODULE(LIBRARY)   ::dlclose((LIBRARY))
#   define LUNAR_GET_PROC(LIBRARY,NAME) ::dlsym((LIBRARY),(NAME))
#endif

// Little Side Note on dlopen and LoadLibraryA
// both Windows and *nix systems implement
// reference counting for their dynamic-libs
// which means that if we want our RAAI class
// to work all we need to do is open the library
// everytime an instance is created
// and close the library everytime it is
// destructed

namespace lunar::core {


#if defined(_WIN32) || defined(WIN32)

    //Platform = Windows
    //Libraries are loaded as HMODULE (which is really just a HINSTANCE in disguise)
    using module_t = HMODULE;
    //Dynamic Libraries have the extension .dll
    constexpr static const char * lib_ext = ".dll";

    // Since the Loading-habits of Windows changed recently
    // A call to SetDllDirectoryA might be needed
    namespace detail {
        inline void library_add_search_folder(const std::string &folder) {
            SetDllDirectoryA(folder.c_str());
        }
    }
#else


    //Platform = Linux,BSD,...
    //Libraries are loaded as void*, which is hardly creative but very functional
    using module_t = void*;
    //Dynamic Libraries have the extension .so
    constexpr static const char * lib_ext = ".so";

    //on linux your only option is to specify what you want directly to
    //dlopen, so this function is just a stub
    namespace detail {
        inline void library_add_search_folder(...)
        {
        }
    }
#endif


// All platforms appear to agree that void * is pretty good for whatever
// GetProcAddress/dlsym returns.
using proc_addr_t = void *;

template<class FnType>
class symbol {};

/**
 * @class symbol
 * @brief Represents a single loaded symbol from a dynamic loaded library.
 * @tparam Ret The return type of the symbol
 * @tparam Args The arguments for the symbol
 */
template<class Ret, class... Args>
class symbol<Ret(Args...)> {
public:
    using return_type = Ret;

    explicit symbol(void *symbol_ptr) noexcept: m_symbol_ptr(symbol_ptr) {}

    /**
     * @brief Calling operator for the loaded symbol
     */
    return_type operator()(Args ... arguments) {
        auto ptr = reinterpret_cast<return_type(*)(Args...)>(m_symbol_ptr);
        return ptr(std::forward<Args>(arguments)...);
    }

    /**
     * @brief Checks if the symbol is valid
     */
    [[nodiscard]] bool valid() const noexcept
    {
        return m_symbol_ptr != nullptr;
    }

    /**
     * @brief Checks if the symbol is valid
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return valid();
    }

    friend bool operator==(const symbol& lhs,std::nullptr_t) noexcept{
        return !lhs.valid();
    }

    friend bool operator!=(const symbol& lhs,std::nullptr_t) noexcept{
        return lhs.valid();
    }

private:
    void *m_symbol_ptr;
};

/**
 * @class dylib
 * @brief Opens a dynamic library and exposes the symbols
 */
class dylib {
public:
    explicit dylib(std::string name) noexcept:
            m_name(std::move(name)) {
        m_module = LUNAR_OPEN_MODULE(m_name.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    dylib(dylib &&other) noexcept {
        m_name = std::move(other.m_name);
        m_module = LUNAR_OPEN_MODULE(m_name.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    dylib(const dylib &other) {
        m_name = other.m_name;
        m_module = LUNAR_OPEN_MODULE(m_name.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    dylib &operator=(dylib &&other) noexcept {
        m_name = std::move(other.m_name);
        LUNAR_FREE_MODULE(m_module);
        m_module = LUNAR_OPEN_MODULE(m_name.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    dylib &operator=(const dylib &other) {
        m_name = other.m_name;
        LUNAR_FREE_MODULE(m_module);
        m_module = LUNAR_OPEN_MODULE(m_name.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    ~dylib() {
        LUNAR_FREE_MODULE(m_module);
    }

    /**
     * @brief Checks if the library was opened successfully
     */
    [[nodiscard]] bool valid() const noexcept {
        return m_module != nullptr;
    }

    /**
     * @brief Checks if the dylib is valid
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return valid();
    }

    friend bool operator==(const dylib& lhs,std::nullptr_t) noexcept {
        return !lhs.valid();
    }

    friend bool operator!=(const dylib& lhs,std::nullptr_t) noexcept {
        return lhs.valid();
    }

    friend bool operator==(const dylib &lhs, const dylib &rhs) noexcept {
        return lhs.m_name == rhs.m_name && lhs.valid() == rhs.valid();
    }

    friend bool operator!=(const dylib &lhs, const dylib &rhs) noexcept {
        return !(lhs == rhs);
    }

    /**
     * @brief Loads a symbol from the dynamic library
     * @param name The name of the symbol in the library
     * @return a void* to the symbol, nullptr if the name was invalid or valid() == false
     */
    [[nodiscard]] proc_addr_t get_symbol(const std::string &name) const {
        if(!valid()) return nullptr;
        auto proc_address = LUNAR_GET_PROC(m_module, name.c_str());
        return reinterpret_cast<proc_addr_t>(proc_address);
    }

    /**
     * @brief Loads a symbol from the dynamic library, wrapped in symbol<>
     * @param name The name of the symbol in the library
     * @return a symbol<> to the symbol in the library
     * @note if valid() == false or if the symbol does not exist, symbol<>::valid() = false
     */
    template<class FnType>
    [[nodiscard]] symbol<FnType> get_cast_symbol(const std::string &name) const {
        auto proc_address = get_symbol(name);
        return symbol<FnType>(proc_address);
    }

private:
    std::string m_name;
    module_t m_module;
};

}
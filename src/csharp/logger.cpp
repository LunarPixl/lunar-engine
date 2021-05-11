//
// Created by Raphael Baier on 04.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#include <csharp/logger.hpp>
#include <spdlog/spdlog.h>

namespace lunar::csharp{

    namespace detail {
        inline void logging_fn(int severity,const char * message)
        {
            spdlog::log(static_cast<spdlog::level::level_enum>(severity),message);
        }
    }
    Logger::Logger(HostFXR *loader) {

        constexpr static auto ASSEMBLY_PATH = LUNAR_CSHARP_ANNOTATE_TEXT(".\\build-dotnet\\dotnetlunar.dll");


        assembly_create_info logger_received_info {
            LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
            LUNAR_CSHARP_ANNOTATE_TEXT("Logger"),
            LUNAR_CSHARP_ANNOTATE_TEXT("ReceivedLogDelegateFn"),
            LUNAR_CSHARP_ANNOTATE_TEXT("ReceivedLogDelegate"),
            LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar"),
        };

        auto assembly = loader->load_assembly(
            ASSEMBLY_PATH,
            logger_received_info
        );

        assembly.cast<void(decltype(&detail::logging_fn))>()(&detail::logging_fn);

    }
}

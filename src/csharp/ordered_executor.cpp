#include <csharp/ordered_executor.hpp>

namespace lunar::csharp {
    OrderedExecutor::OrderedExecutor(HostFXR *loader):
            m_has_runners_sym(nullptr),
            m_read_reset_dirty_bit_sym(nullptr),
            m_update_sym(nullptr)
            {
        constexpr static auto ASSEMBLY_PATH = LUNAR_CSHARP_ANNOTATE_TEXT(".\\build-dotnet\\dotnetlunar.dll");

        assembly_create_info prototype {
                assembly_create_info::unmanaged_callers_only{},
                LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
                LUNAR_CSHARP_ANNOTATE_TEXT("OrderedExecutor"),
                LUNAR_CSHARP_ANNOTATE_TEXT(""),
                LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar"),
        };

        assembly_create_info startupfn_info =
                prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("Startup"));

        assembly_create_info hasrunners_info =
                prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("HasRunnersFor"));

        assembly_create_info dirtybit_info =
                prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("DirtyBitSet"));

        assembly_create_info updateindex_info =
                prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("UpdateIndex"));


        //Load and run startup
        auto startupassembly = loader->load_assembly(
            ASSEMBLY_PATH,
            startupfn_info
        );

        startupassembly.cast<void()>()();


        //Load HasRunnersFor and update index
        auto hasrunnerassembly = loader->load_assembly(
                ASSEMBLY_PATH,
                hasrunners_info
        );

        m_has_runners_sym = hasrunnerassembly.cast<uint8_t(uint8_t)>();

        for(int i = 0; i <= 255; i++)
        {
            if(m_has_runners_sym((uint8_t)i))
            {
                m_indicies_with_runners.insert(i);
            }
        }


        //Load DirtyBitSet and run once

        auto dirtybitassembly = loader->load_assembly(
            ASSEMBLY_PATH,
            dirtybit_info
        );

        m_read_reset_dirty_bit_sym = dirtybitassembly.cast<uint8_t()>();
        m_read_reset_dirty_bit_sym();

        auto updateindexassembly = loader->load_assembly(
            ASSEMBLY_PATH,
            updateindex_info
        );

        m_update_sym = updateindexassembly.cast<void(uint8_t)>();
    }

    void OrderedExecutor::update_index(uint8_t idx) {
        if(m_read_reset_dirty_bit_sym())
        {
            for(int i = 0; i < 255; i++)
            {
                if(m_has_runners_sym(i))
                {
                    m_indicies_with_runners.insert(i);
                }
                else if (m_indicies_with_runners.contains(i))
                {
                    m_indicies_with_runners.erase(i);
                }
            }
        }

        if (m_indicies_with_runners.contains(idx))
        {
            m_update_sym(idx);
        }
    }
}


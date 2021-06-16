//
// Created by Raphael Baier on 14.03.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once
#include <string_view>
#include <string>
#include "invoke_external.hpp"


namespace lunar::test {

#if WIN32
    static constexpr const char * python_path = ".\\venv\\Scripts\\python";
#else
    static constexpr const char * python_path = "./venv/bin/python";
#endif

    enum Tools {
          IMG_DIFF,
          IMG_COMPARE,
          IMG_CROP,
          TOOL_TEST
    };

    template <Tools Tool>
    static constexpr const char * tool_path()
    {
        if constexpr (Tool == Tools::IMG_DIFF)
            return "tools\\img_diff.py";
        if constexpr (Tool == Tools::IMG_COMPARE)
            return "tools\\img_compare.py";
        if constexpr (Tool == Tools::IMG_CROP)
            return "tools\\img_crop.py";
        if constexpr (Tool == Tools::TOOL_TEST)
            return "tools\\test_tools.py";
        return "";
    }


    class [[maybe_unused]] Toolkit {


            /**@brief calls the tools\image_compare.py tool
             * @param lhs the reference picture
             * @param rhs the picture to compare
             * @percentage the amount the picture needs to match 100 = completely the same
             *
             * @returns true if the pictures match
             */
            [[maybe_unused]] static bool img_compare(const std::string& lhs, const std::string& rhs, int percentage)
            {
                // get python interpreter
                const auto python = std::string{python_path};

                // get img_compare tool
                const auto tool = std::string{tool_path<Tools::IMG_COMPARE>()};

                // for better looks really
                const auto space = std::string{" "};

                //craft command
                const auto cmd = python + space + tool + space + lhs + space + rhs + std::to_string(percentage);

                //invoke command, default to 1 for errors
                const int rc = invoke_external(cmd).value_or(1);

                // rc <- 0 == no error
                return rc == 0;
            }

            struct ICRect {
                int x;
                int y;
                int width;
                int height;
            };


            [[maybe_unused]] static void image_diff(const std::string & source, const std::string& base,
                                                    const std::string& dest, bool interactive = false) {
                
            }

            [[maybe_unused]] static void image_crop(const std::string& source, ICRect area, const::std::string& dest)
            {
                //get python & tool
                const auto python = std::string{python_path};
                const auto tool = std::string {tool_path<Tools::IMG_CROP>()};

                // _aesthetics_
                const auto space = std::string{" "};

                //craft rect in format <x> <y> <w> <h>
                const auto rect = std::string {
                    std::to_string(area.x)      + space + std::to_string(area.y) + space +
                    std::to_string(area.height) + space + std::to_string(area.width)
                };

                //craft cmd
                const auto cmd = python + space + tool + space + source + space + rect + space + dest;

                //invoke command
                invoke_external(cmd);

            }

    };
}



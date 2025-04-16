// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>

namespace Utils
{
    inline std::string Trim(const std::string &s)
    {
        const auto start = s.find_first_not_of(' ');
        if (start == std::string::npos) return "";
        const auto end = s.find_last_not_of(" \t");
        return s.substr(start, end - start + 1);
    }
}

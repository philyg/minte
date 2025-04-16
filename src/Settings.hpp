// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "Utils.hpp"

class Settings
{
private:
    std::vector<std::string> _ignoredVariables;
    std::vector<std::string> _allowedVariables;
    std::string _startDelimiter = "{{";
    std::string _endDelimiter = "}}";
    std::string _commentMarker = "%";

    static void AddVariableList(const std::string &vars, std::vector<std::string> &list)
    {
        size_t pos = 0;
        for (;;)
        {
            const auto delimpos = vars.find(',', pos);
            auto str = Utils::Trim(vars.substr(pos, delimpos));
            if (!str.empty())
            {
                list.push_back(std::move(str));
            }

            if (delimpos == std::string::npos)
            {
                break;
            }

            pos = delimpos + 1;
        }
    }

public:
    bool AllowNonexistant = false;
    bool OnlyAllowed = false;

    void SetStartDelimiter(const std::string &delim)
    {
        if (delim.empty())
        {
            throw std::runtime_error("Start delimiter cannot be empty!");
        }

        _startDelimiter = delim;
    }

    void SetEndDelimiter(const std::string &delim)
    {
        if (delim.empty())
        {
            throw std::runtime_error("End delimiter cannot be empty!");
        }

        _endDelimiter = delim;
    }

    void SetCommentMarker(const std::string &delim)
    {
        if (delim.empty())
        {
            throw std::runtime_error("Comment marker cannot be empty!");
        }

        _commentMarker = delim;
    }

    [[nodiscard]] const std::string &StartDelimiter() const
    {
        return _startDelimiter;
    }

    [[nodiscard]] const std::string &EndDelimiter() const
    {
        return _endDelimiter;
    }

    [[nodiscard]] const std::string &CommentMarker() const
    {
        return _commentMarker;
    }

    void AddIgnoredVariables(const std::string &vars)
    {
        AddVariableList(vars, _ignoredVariables);
    }

    void AddAllowedVariables(const std::string &vars)
    {
        AddVariableList(vars, _allowedVariables);
    }

    [[nodiscard]] bool IsIgnored(const std::string &name) const
    {
        return std::any_of(_ignoredVariables.begin(), _ignoredVariables.end(), [&](const std::string &s) { return s == name; });
    }

    [[nodiscard]] bool IsAllowed(const std::string &name) const
    {
        bool allowed = true;
        if (OnlyAllowed)
        {
            allowed = std::any_of(_allowedVariables.begin(), _allowedVariables.end(), [&](const std::string &s) { return s == name; });
        }

        allowed = allowed && !IsIgnored(name);

        return allowed;
    }
};

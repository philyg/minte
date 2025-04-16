// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#pragma once
#include <sstream>

#include "LineReader.hpp"
#include "Settings.hpp"

class TemplateReplacer
{
private:
    const Settings &_settings;

public:
    explicit TemplateReplacer(const Settings &settings)
        : _settings(settings)
    {
    }

    [[nodiscard]] std::string FilterLine(const std::string &line, const int linenum) const
    {
        size_t pos = 0;
        std::stringstream ss;

        for (;;)
        {
            auto startpos = line.find(_settings.StartDelimiter(), pos);
            if (startpos == std::string::npos)
            {
                break;
            }

            const auto endpos = line.find(_settings.EndDelimiter(), startpos);
            if (endpos == std::string::npos)
            {
                throw std::runtime_error("Line " + std::to_string(linenum) + ": Missing end delimiter");
            }

            ss << line.substr(pos, startpos - pos);

            startpos += _settings.StartDelimiter().length();

            const auto varname = line.substr(startpos, endpos - startpos);
            const auto trimvarname = Utils::Trim(varname);

            if (trimvarname.find(_settings.CommentMarker()) == 0)
            {
                // Do not append anything, this is a comment.
            }
            else if (!_settings.IsAllowed(trimvarname))
            {
                ss << _settings.StartDelimiter() << varname << _settings.EndDelimiter();
            }
            else
            {
                const char *varvalue = std::getenv(trimvarname.data());

                if (varvalue == nullptr)
                {
                    if (!_settings.AllowNonexistant)
                    {
                        throw std::runtime_error("Line " + std::to_string(linenum) + ": Cannot find referenced environment variable '" + trimvarname + "'");
                    }
                    // Else append nothing here.
                }
                else
                {
                    ss << varvalue;
                }
            }

            pos = endpos + _settings.EndDelimiter().length();
        }

        ss.write(line.data() + pos, static_cast<std::streamsize>(line.length() - pos));
        return ss.str();
    }

    void PipeStream(std::istream &s, std::ostream &o) const
    {
        LineReader lr(s);

        for (;;)
        {
            const auto line = lr.ReadLine();
            const auto filteredLine = FilterLine(line, lr.LineNumber());
            o << filteredLine;
            if (lr.eof())
            {
                break;
            }

            o << "\n";
        }
    }
};

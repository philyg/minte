// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#pragma once

#include <iostream>

#include "Settings.hpp"

class ArgumentParser
{
private:
    const int _argc;
    char *const*const _argv;

    char *GetParameterValue(int &n) const
    {
        if (_argv[n][2] != 0)
        {
            return &_argv[n][2];
        }

        n++;
        if (_argv[n] == nullptr)
        {
            std::cerr << "Missing parameter value for -" << _argv[n - 1][1] << std::endl;
            Usage();
        }

        return _argv[n];
    }

public:
    ArgumentParser(const int argc, char **argv)
        : _argc(argc), _argv(argv)
    {
    }

    [[noreturn]] static void Usage()
    {
        std::cerr << R"usage(
MinTE - The most minimal template engine.
Copyright (c) 2025 Philipp Grassl
https://github.com/philyg/minte

A minimalistic tool to replace placeholders in files with environment variable values.
Reads text lines from stdin and replaces {{ NAME }} by the value of the environment
variable NAME. The start and end delimiters can be set using -s and -e parameters and
are allowed to be equal.

Comments can also be written, which follow the format {{ % This is a comment }} where
the comment marker (% by default) must be the first non-space character after the start
delimiter. The comment marker can be set using the -c parameter.

The replaced variables are filtered as follows:
- If allowed variables are specified (-a), only those variables are replaced
- Additionally, ignored variables (-i) are never replaced

By default, used variables must be defined, otherwise an error is thrown. This
behavior can be changed using the -n switch.

All leading and trailing whitespaces are removed between the start and end delimiter.

Usage: minte [-a VAR1,...] [-i VAR1,...] [-n] [-s value] [-e value] [-c value]

-h            Display this help text.

-a value      Comma-delimited list of allowed variables. Cumulative. Default: none
-i value      Comma-delimited list of ignored variables. Cumulative. Default: none
-n            Replace non-defined variables with an empty string. Default: no

-e value      Set the End-Delimiter to the provided string. Default: }}
-s value      Set the Start-Delimiter to the provided string. Default: {{
-c value      Set the Comment Marker. Default: %

)usage";
        std::exit(1);
    }

    void Parse(Settings &sett) const
    {
        for (int i = 1; i < _argc; i++)
        {
            const char *const currparm = _argv[i];

            if (currparm[0] == '-')
            {
                switch (currparm[1])
                {
                    case 0:
                    case 'h':
                        Usage();

                    case 's':
                        sett.SetStartDelimiter(GetParameterValue(i));
                        break;

                    case 'e':
                        sett.SetEndDelimiter(GetParameterValue(i));
                        break;

                    case 'i':
                        sett.AddIgnoredVariables(GetParameterValue(i));
                        break;

                    case 'c':
                        sett.SetCommentMarker(GetParameterValue(i));
                        break;

                    case 'n':
                        sett.AllowNonexistant = true;
                        break;

                    case 'a':
                        sett.AddAllowedVariables(GetParameterValue(i));
                        sett.OnlyAllowed = true;
                        break;

                    default:
                        std::cerr << "Unknown parameter: '" << currparm[1] << "'!" << std::endl;
                        Usage();
                }
            }
            else
            {
                throw std::runtime_error("Unable to parse command line arguments at " + std::string(currparm));
            }
        }
    }
};

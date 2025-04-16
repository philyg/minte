// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#include <iostream>

#include "ArgumentParser.hpp"
#include "Settings.hpp"
#include "TemplateReplacer.hpp"

int main(const int argc, char **argv)
{
    try
    {
        Settings settings;

        const ArgumentParser parser(argc, argv);
        parser.Parse(settings);

        const TemplateReplacer tr(settings);

        tr.PipeStream(std::cin, std::cout);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

// SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
// SPDX-License-Identifier: MIT

#pragma once

#include <iostream>
#include <string>

class LineReader
{
private:
    std::istream &_stream;
    int _lineCounter = 0;

public:
    explicit LineReader(std::istream &stream)
        : _stream(stream)
    {
    }

    std::string ReadLine()
    {
        std::string linebuf;
        std::getline(_stream, linebuf, '\n');
        if (_stream.bad())
        {
            throw std::runtime_error("Failed to read from stream");
        }

        _lineCounter++;

        return linebuf;
    }

    [[nodiscard]] int LineNumber() const
    {
        return _lineCounter;
    }

    [[nodiscard]] bool eof() const
    {
        return _stream.eof();
    }
};

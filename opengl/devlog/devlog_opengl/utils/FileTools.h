#pragma once

#include <string>
#include <vector>
#include <sstream>

// -------------------- file utils --------------------
std::string readFile(const char* path);

inline std::string stripLineComment(const std::string& line)
{
    size_t pos = line.find("//");
    if (pos != std::string::npos)
        return line.substr(0, pos);
    return line;
}

template <typename T>
std::vector<T> parseFloatVectorFromText(const std::string& text)
{
    std::vector<T> out;

    std::stringstream input(text);
    std::string line;

    while (std::getline(input, line))
    {
        line = stripLineComment(line);

        for (char& c : line)
        {
            if (c == ',' || c == '{' || c == '}' || c == 'f')
                c = ' ';
        }

        std::stringstream ss(line);
        T v{};
        while (ss >> v)
            out.push_back(v);
    }

    return out;
}

template <typename T>
std::vector<T> loadVector(const char* path)
{
    std::string text = readFile(path);
    if (text.empty())
        return {};

    return parseFloatVectorFromText<T>(text);
}
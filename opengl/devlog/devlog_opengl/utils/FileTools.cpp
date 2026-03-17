#include "pch.h"
#include "FileTools.h"

std::string readFile(const char* path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(path);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    }
    catch (const std::ifstream::failure&)
    {
        std::cout << "ERROR::FILE_NOT_SUCCESSFULLY_READ: " << path << "\n";
        return "";
    }
}


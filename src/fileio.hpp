#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <tuple>

class FileIO
{

public:
    // function for file read
    static std::tuple<std::vector<std::string>, std::vector<std::vector<double>>> read(const std::string &filename);
    // function for writing file using comma seperation
    void write(const std::string &filename, const std::vector<std::string> &header, const std::vector<std::vector<double>> &data);
};

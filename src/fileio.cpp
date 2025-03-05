#include "fileio.hpp"
#include <fstream>
#include <sstream>

std::tuple<std::vector<std::string>, std::vector<std::vector<double>>> FileIO::read(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw "Error opening file";
    }

    std::string line;
    std::getline(file, line);
    std::vector<std::string> firstline;
    std::istringstream header(line);
    std::string token;
    while (std::getline(header, token, ','))
    {
        firstline.push_back(token);
    }

    std::vector<std::vector<double>> mat;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::vector<double> row;
        while (std::getline(lineStream, token, ','))
        {
            row.push_back(std::stod(token));
        }
        mat.push_back(row);
    }
    file.close();

    return std::make_tuple(firstline, mat);
}

void FileIO::write(const std::string &filename, const std::vector<std::string> &header, const std::vector<std::vector<double>> &data)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw "Error opening file for writing";

    // header
    for (size_t i = 0; i < header.size(); i++)
    {
        file << header[i];
        if (i != header.size() - 1)
            file << ",";
    }
    file << "\n";

    // data
    for (const auto &row : data)
    {
        for (size_t j = 0; j < row.size(); j++)
        {
            file << row[j];
            if (j != row.size() - 1)
                file << ",";
        }
        file << "\n";
    }

    file.close();
}

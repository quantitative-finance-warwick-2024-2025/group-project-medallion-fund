# pragma once
#include <vector>
#include <iostream>
#include <string>
#include <tuple>


class FileIO {
   
   public:
    static std::tuple<std::vector<std::string>, std::vector<std::vector<double>>> read(const std::string& filename);
};

/////COMMAND 
/*
std::string path = "/Users/akshayzine/Academics/Programming_Group_Project/matrix/Assets.csv"; 
 auto [assetnames, assetprices] = FileRead::read(path);
*/
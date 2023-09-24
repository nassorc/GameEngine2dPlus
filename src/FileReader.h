//
// Created by Matthew Crossan on 9/17/23.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class FileReader {
private:
    std::ifstream m_file;
public:
    FileReader();
    FileReader(const std::string& path);
    bool openFile(const std::string& path);
    std::string readFileContents();
};

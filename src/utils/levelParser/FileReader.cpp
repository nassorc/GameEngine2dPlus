//
// Created by Matthew Crossan on 9/17/23.
//

#include "FileReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

FileReader::FileReader() {

}

FileReader::FileReader(const std::string& path) {
    openFile(path);
}
bool FileReader::openFile(const std::string& path){
    m_file.open(path);
    return m_file.is_open();
}
std::string FileReader::readFileContents(){
    std::stringstream buffer;

    if(!m_file.is_open()) {
        std::cout << "FileReader cannot not open file" << std::endl;
        exit(-1);
    }
    buffer << m_file.rdbuf();
    return buffer.str();
}


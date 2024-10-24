#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

static std::string LoadFileContent(const std::string& filePath) {
    std::ifstream FolderFile(filePath);

    if (!FolderFile.is_open()) 
    {
        std::cerr << "Error: Unable to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream ContentStream;
    ContentStream << FolderFile.rdbuf();
    FolderFile.close(); 

    return ContentStream.str(); 
}
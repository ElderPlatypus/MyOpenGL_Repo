#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>
#include <string>
#include <vector>

struct FileHandler
{
    static std::vector<std::string> LoadVectorOfPath(const std::string& root, const std::string& folderPath)
    {
        //Vector for emplacing path names
        std::vector<std::string> contents;

        // Check if path exists and is a directory
        if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath))
        {
            for (const auto& entry : std::filesystem::directory_iterator(folderPath))
            {
                //Check if entry_path is a regular file: executable, text or image files
                if (entry.is_regular_file())
                {
                    contents.emplace_back(root + entry.path().filename().string()); // Get only the filename  
                }
            }
        }
        else
        {
            std::cerr << "Path does not exist or is not a directory." << std::endl;
            {
                return {};
            }
        }
        return contents;
    }

    static std::vector<std::string> LoadFileFromPath(const std::string& filePath)
    {
        //Vector for emplacing path names
        std::vector<std::string> contents;

        // Check if path exists and is a directory
        if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath))
        {
            std::ifstream file(filePath);

            //Flag
            if (!file.is_open())
            {
                std::cerr << "Failed to open the file: " << filePath << std::endl;
                return {};
            }

            //Read Contents
            std::string line;
            while (std::getline(file, line))
            {
                contents.push_back(line);
            }

            //close file
            file.close();
        }
        else
        {
            std::cerr << "Path does not exist or is not a directory." << std::endl;
            {
                return {};
            }
        }
        return contents;
    }
};

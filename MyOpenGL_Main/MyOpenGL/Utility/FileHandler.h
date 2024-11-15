#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>
#include <string>
#include <vector>

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
               contents.emplace_back(root+entry.path().filename().string()); // Get only the filename  
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


//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#pragma once

#include "SeveralFiles.h"
#include "File.h"
#include <iostream>
#include <list>
#include <vector>
#include <experimental/filesystem>
#include <map>
#include <utility>
#include <sstream>

class SeveralFiles{
private:

    std::vector<File> m_files;

public:
    bool loadFiles (File  & ) ;

    bool load(const std::string & );
    bool save(const std::string & );

    template <class T>
    T byteToType(T verf, std::byte fN[], int ) const;
    bool isValid(const SeveralFiles &severalFiles) const;

    std::vector<File> getFiles() const { return m_files; }


};

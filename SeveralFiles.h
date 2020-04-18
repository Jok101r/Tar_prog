//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#ifndef TAR_PROG_SEVERALFILES_H
#define TAR_PROG_SEVERALFILES_H

#include "SeveralFiles.h"
#include "File.h"
#include <iostream>
#include <list>
#include <vector>
#include <experimental/filesystem>
#include <map>
#include <utility>

class SeveralFiles{
private:

    std::vector<File> m_files;

public:
    bool load(std::list<const std::string> & );
    bool loadFiles (File  & ) ;

    bool load(const std::string & );
    bool save(const std::string & );

    template <class T>
    T byteToType(T verf, std::byte fN[], int ) const;
    //std::list<File> files() const;
    bool isValid() const;

    std::vector<File> getFiles() const { return m_files; }


};

#endif //TAR_PROG_SEVERALFILES_H

//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//
#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <experimental/filesystem>
#include "libs/docopt/docopt.h"


//namespace fs = std::experimental::filesystem;

//ключи управления утилитой
enum Key{

    ARCHIVE_TAR,
    UNARCHIVE_TAR,
    MAX_KEYS

};

class GUI {
private:


    std::string m_archiveFileName;
    //std::string m_path;
    std::string m_path_to_file;

private:
    std::string m_path_to_files;
    std::string m_path_to_fileArch;
    std::string m_path_to_save;
    std::string m_name_fileArh;
    std::vector<Key> m_keys {ARCHIVE_TAR, UNARCHIVE_TAR};
    std::string m_aruments;
    //std::map<std::string, docopt::value> m_args;

public:

    void writeLine(const std::string line) { std::cout << line << std::endl; }
    std::string writeLine();
    std::string nameCreateTar(const std::string &);
    void parseArgs(int argc, const char** argv);
    Key keys();
    //std::string loadLine(bool );
    //std::map<std::string, docopt::value> getArgs() { return m_args; };

    const std::string &getMPathToFiles() const;
    const std::string &getMPathToFileArch() const;
    const std::string &getMPathToSave() const;
    const std::string &getMNameFileArh() const;
    const std::string &getMPathToFile() const;


};

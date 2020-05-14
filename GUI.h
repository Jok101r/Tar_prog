//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//
#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <experimental/filesystem>

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
    std::string m_path;
    std::vector<Key> m_keys {ARCHIVE_TAR, UNARCHIVE_TAR};
    const char *m_aruments[];

public:

    void writeLine(const std::string line) { std::cout << line << std::endl; }
    std::string writeLine();
    std::string nameCreateTar(const std::string &);
    void parseArgs(int argc, char *argv[]);
    Key keys();
    std::string loadLine(bool );

};

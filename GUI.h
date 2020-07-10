//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//
#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>
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

    std::vector<Key> m_keys {ARCHIVE_TAR, UNARCHIVE_TAR};

public:

    void writeLine(const std::string line) { std::cout << line << std::endl; }
    std::string writeLine();
    struct EntredArgs{
        struct Atar{
            const bool flag;
            const std::string pathToFiles;
            const std::string pathToSave;
            const std::string pathToFile;
            Atar();
            Atar(const bool &pFlag, const std::string &pPathToFiles, const std::string &pPathToSave, const std::string &pPathToFile)
                :flag(pFlag)
                ,pathToFiles(pPathToFiles)
                ,pathToSave(pPathToSave)
                ,pathToFile(pPathToFile) {}
        };
        struct Untar{
            const bool flag;
            const std::string pathToFile;
            const std::string pathToFolder;
            Untar();
            Untar(const bool &pFlag, const std::string &pPathToFile, const std::string &pPathToFolder)
                : flag(pFlag)
                , pathToFile(pPathToFile)
                , pathToFolder(pPathToFolder) {}
        };

        const Atar atar;
        const Untar untar;
        EntredArgs(Atar &pAtar, Untar &pUntar)
            : atar(pAtar)
            , untar(pUntar) {}

    };
    EntredArgs parseArgs(int, const char** );

    Key keys(EntredArgs &args);

};

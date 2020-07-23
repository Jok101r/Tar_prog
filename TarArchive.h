//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#pragma once

#include "IArchive.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <cstddef>
#include <fstream>
#include <map>
#include <sstream>
#include <pwd.h>
#include <sys/stat.h>
#include <iterator>

//поля хидера

class TarArchive: public IArchive {

private:
    MetaData m_fieldName;
    std::map<std::byte*, int> m_fieldNameVec {
            {m_fieldName.name, 100},
            {m_fieldName.mode, 8},
            {m_fieldName.uid, 8},
            {m_fieldName.gid, 8},
            {m_fieldName.size, 12},
            {m_fieldName.mtime, 12},
            {m_fieldName.chkSum, 8},
            {m_fieldName.link, 1},
            {m_fieldName.linkName, 100}};

    SeveralFiles  m_archiveFileTar;
    File m_file;

    std::vector<char> m_fileTar;


    void recordFieldToFileTar(const std::byte *, const int );
    void recordFieldToFileTar(const fs::path &);

    void fillingWithZeroTo512();

    //получение данных файлов
    void recordTar(const fs::path &, const std::string &);


    template <class T>
    T byteToType(T verf, std::byte fN[], int ) const;
    int multiplicity512(int );
    void parsingTar();


public:

//    //по штучный разбор файлов для архивирования

    bool isTar(const std::string &);
    virtual void append(const File &);
    virtual void append(const SeveralFiles &);
    virtual File archive();
    virtual SeveralFiles unarchive();


};


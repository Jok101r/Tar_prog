//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#ifndef TAR_PROG_TARARCHIVE_H
#define TAR_PROG_TARARCHIVE_H

#include "IArchive.h"
#include <iostream>
#include <experimental/filesystem>
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


    void recordFieldToFile(const std::byte *, const int );
    void recordFieldToFile(const fs::path &);

    void fillingWithZeroTo512();

    //получение данных файлов
    void recordTar(const fs::path &);


    template <class T>
    T byteToType(T verf, std::byte fN[], int ) const;
    int multiplicity512(int );
    void parsingTar();


public:

//    //по штучный разбор файлов для архивирования
//    void archiveFiles();

    virtual bool append(const File &);
    virtual bool append(const SeveralFiles &);
    virtual File archive();
    virtual SeveralFiles unarchive();


};

#endif //TAR_PROG_TARARCHIVE_H

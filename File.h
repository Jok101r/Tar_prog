//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#ifndef TAR_PROG_FILE_H
#define TAR_PROG_FILE_H

#include <iostream>
#include <vector>
#include <experimental/filesystem>
#include <fstream>
#include <map>

namespace fs = std::experimental::filesystem;
struct Field_name{

    std::byte name[100] {0x0}; //имя файла (вместе с разширением)
    std::byte mode[8] {0x0}; //права доступа на файл (chmod)
    std::byte uid[8] {0x0}; //ID пользователя
    std::byte gid[8] {0x0}; //ID группы
    std::byte size[12] {0x0};  //размер файда
    std::byte mtime[12] {0x0}; // время последнего изменения
    std::byte chkSum[8] {0x0}; // проверка суммы
    std::byte link[1] {0x0}; // файл or директория
    std::byte linkName[100] {0x0};
};
class File {
private:

    std::string m_pathFile;
    std::vector<char> m_dataFile;
    std::string m_archiveFileName;
    Field_name m_fieldName;
    std::map<std::byte*, int> m_metaData {
            {m_fieldName.name, 100},
            {m_fieldName.mode, 8},
            {m_fieldName.uid, 8},
            {m_fieldName.gid, 8},
            {m_fieldName.size, 12},
            {m_fieldName.mtime, 12},
            {m_fieldName.chkSum, 8},
            {m_fieldName.link, 1},
            {m_fieldName.linkName, 100}};
public:


    bool load(const std::string & );
    bool load(const std::vector<char> & );
    bool loadDataField(std::map<std::byte*, int> &, const std::vector<char> &);
    bool save(const std::string & );
    std::vector<char> dataToPath();
    std::vector<char> data() {return m_dataFile; }
    bool isValid() const;

    fs::path getPathFile() { return m_pathFile; }
    std::map<std::byte*, int> getMetaData() { return m_metaData; }
    Field_name getFieldName() {return m_fieldName; }


};


#endif //TAR_PROG_FILE_H

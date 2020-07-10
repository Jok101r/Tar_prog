//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "File.h"



void joinTo(std::byte* metaDataOne,const std::byte* metaDataTwo,const int size){

    for(int run=0 ; run < size; run++)
        metaDataOne[run] = metaDataTwo[run];

}

struct MetaData & MetaData::operator=(const MetaData *metaData) {


    //корректно?
    if (this != metaData) {

        joinTo(this->name,metaData->name, 100);
        joinTo(this->mode, metaData->mode, 8);
        joinTo(this->uid, metaData->uid, 8);
        joinTo(this->gid, metaData->gid, 8);
        joinTo(this->size, metaData->size, 12);
        joinTo(this->mtime, metaData->mtime, 12);
        joinTo(this->chkSum, metaData->chkSum, 8);
        joinTo(this->link, metaData->link, 1);
        joinTo(this->linkName,metaData->linkName, 100);

        return *this;
    }

    return  *this;


}

template <class T>
T byteToType(T verf, std::byte fN[], int sizeArr)  {

    std::string byteTotype;
    for (int run = 0; run < sizeArr; run++) {
        byteTotype += static_cast<char>(fN[run]);
    }
    std::stringstream ss;
    ss << byteTotype;
    T a;
    ss >> a;
    return a;
}
    //загрузка пути файла
    bool File::load(const std::string &pathToFile) {

        if (!pathToFile.empty()) {

            m_pathFile = pathToFile;
            std::fstream file(m_pathFile);
            char *buffer = new char[1];
            while (!file.eof()) {
                file.read(buffer, 1);
                m_dataFile.push_back(*buffer);
            }
            delete[] buffer;
            return true;
        }
        return false;
    }

    //загрузка данных в File
    bool File::load(const std::vector<char> &file) {


        for (unsigned int run = 0; run < file.size(); run++)
            m_dataFile.push_back(file[run]);
        return true;
    }


//запись в класс файл File полей хидера fieldName  и содержимого файла
    bool File::loadDataField(MetaData &metaData, const std::vector<char> &file) {

        for (unsigned int run = 0; run < file.size(); run++)
            m_dataFile.push_back(file[run]);
        m_fieldName = metaData;
        return true;
    }

    //архивация файлов по методу .tar
    bool File::save(const std::string &pathTar){

    try {
        std::ofstream archiveFile(pathTar);
        for (auto &run : m_dataFile)
            archiveFile << run;
        archiveFile.close();
        return true;
    }
    catch (...){

        return false;

    }

    }
    bool File::save(const File &file, const std::string & pathToDir){

        //так можно? пока еще плохо понимаю как исключения использоваться

        try {
            //не знаю как обойти этот момент
            int a;
            std::string b;
            std::string name_file = byteToType(b, m_fieldName.name,
                    sizeof(file.m_fieldName.name) / sizeof(file.m_fieldName.name[0]) );
            {
                std::ofstream newFile(pathToDir + name_file);
                for (auto &run : m_dataFile)
                    newFile << run;
                newFile.close();
            }

            fs::permissions(pathToDir + name_file,
                            static_cast<fs::perms> (byteToType(a, m_fieldName.mode,
                                    sizeof(m_fieldName.mode) / sizeof(m_fieldName.mode[0]))));
            std::chrono::time_point timePoint = fs::file_time_type::clock::from_time_t(
                    byteToType(a, m_fieldName.mtime,
                               sizeof(m_fieldName.mtime) / sizeof(m_fieldName.mtime[0])));
            fs::last_write_time(pathToDir + name_file, timePoint);

            return true;
        }
        catch (...){

            return false;

        }

    }

    //проверка на существование файла
    bool File::isValid() const {

        //корректно?
        if (m_dataFile.empty())
            return false;
        return true;
    }
    bool File::isValid(const File &file) const {


        if (m_dataFile.empty())
            return false;
        for(auto &run_mode : m_fieldName.mode) {
            if ('1' == static_cast<char>(run_mode) || '2' == static_cast<char>(run_mode) ||
                  '3' == static_cast<char>(run_mode) || '4' == static_cast<char>(run_mode) ||
                  '5' == static_cast<char>(run_mode) || '6' == static_cast<char>(run_mode) ||
                  '7' == static_cast<char>(run_mode) || '8' == static_cast<char>(run_mode) ||
                  '9' == static_cast<char>(run_mode))
                break;
            else return false;
        }

        for(auto &run_mode : m_fieldName.uid) {
            if ('1' == static_cast<char>(run_mode) || '2' == static_cast<char>(run_mode) ||
                  '3' == static_cast<char>(run_mode) || '4' == static_cast<char>(run_mode) ||
                  '5' == static_cast<char>(run_mode) || '6' == static_cast<char>(run_mode) ||
                  '7' == static_cast<char>(run_mode) || '8' == static_cast<char>(run_mode) ||
                  '9' == static_cast<char>(run_mode))
                break;
            else return false;
        }

        for(auto &run_mode : m_fieldName.gid) {
            if ('1' == static_cast<char>(run_mode) || '2' == static_cast<char>(run_mode) ||
                  '3' == static_cast<char>(run_mode) || '4' == static_cast<char>(run_mode) ||
                  '5' == static_cast<char>(run_mode) || '6' == static_cast<char>(run_mode) ||
                  '7' == static_cast<char>(run_mode) || '8' == static_cast<char>(run_mode) ||
                  '9' == static_cast<char>(run_mode))
                break;
            else return false;
        }

        if( fs::is_directory(m_pathFile) ) {
            if (!('0' == static_cast<char> (m_fieldName.link[0])))
                return false;
        }
        else {
            if (!('1' == static_cast<char> (m_fieldName.link[0])))
            return false;

        }

    return true;
}

bool File::isDirectory(const std::string is_directory) const {

    if(fs::is_directory(is_directory))
        return true;

    return false;
}


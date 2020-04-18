//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "File.h"

    //загрузка пути файла
    bool File::load(const std::string &pathToFile) {
        m_pathFile = pathToFile;
        return true;
    }

    //загрузка данных в File
    bool File::load(const std::vector<char> &file) {

        for (int run = 0; run < file.size(); run++)
            m_dataFile.push_back(file[run]);
        return true;
    }

    //запись входного хидера в класс File
    void joinTo(std::byte* metaDataOne,  std::byte* metaDataTwo, int size){

    for(int run=0 ; run < size; run++)
        metaDataOne[run] = metaDataTwo[run];

}

//запись в класс файл File полей хидера fieldName  и содержимого файла
    bool File::loadDataField(std::map<std::byte*, int> &metaData, const std::vector<char> &file) {

        for (int run = 0; run < file.size(); run++)
            m_dataFile.push_back(file[run]);

        //не работает
        //m_metaData = metaData;

        std::map<std::byte*, int>::iterator one;
        one = m_metaData.begin();
        std::map<std::byte*, int>::iterator two;
        two = metaData.begin();

        while(two != metaData.end()) {
            joinTo((*one).first, (*two).first, (*two).second );
            two++;
            one++;
        }
        return true;
    }

    //архивация файлов по методу .tar
    bool File::save(const std::string &pathToFile){

    //тут я не уверен
        std::cout << "\nEnter a file name tar:\n";
        std::cin >> m_archiveFileName;
        m_archiveFileName+=".tar";

        m_archiveFileName = pathToFile + m_archiveFileName;
        std::ofstream archiveFile(m_archiveFileName);
        for ( auto &run : m_dataFile)
            archiveFile << run;
        archiveFile.close();

        return true;


    }

    //получение данных по пути
    std::vector<char> File::dataToPath() {

        //как-то коряво и правильно ли
        std::fstream file(m_pathFile);
        char *buffer = new char[1];
        while (!file.eof()) {
            file.read(buffer, 1);
            m_dataFile.push_back(*buffer);
        }
        delete[] buffer;
        return m_dataFile;
    }

    //проверка на существование файла
    bool File::isValid() const {

    //корректно?
    if (m_dataFile.empty())
        return false;

    return true;
}


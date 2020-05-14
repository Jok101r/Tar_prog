//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "SeveralFiles.h"



    std::pair<int,bool> checkNumberFiles(int inputNumber){

        std::pair<int,bool> result {0, false };
        if(!inputNumber <= 0)
            result = std::pair<int,bool> {inputNumber, true};

        return result;
    }

    //добавление File в вектор std::vector<File>
    bool SeveralFiles::loadFiles (File  &file)  {

        m_files.push_back(file);
        return true;

    }

    //чтение файлов в директории
    bool SeveralFiles::load(const std::string & pathToDir) {

        try {

            int numberFiles = 0;
            for (const auto &runDirectory : fs::directory_iterator(pathToDir)) {

                File file;

                if (!fs::is_directory(runDirectory))
                    if (file.load(runDirectory.path())) {
                        numberFiles++;
                        m_files.push_back(file);
                    }
            }

            auto resultnumberFiles = checkNumberFiles(numberFiles);
            if(resultnumberFiles.second)
                std::cout << "Number of files to back up: " << resultnumberFiles.first << std::endl;
            else
            {
                std::cout << "Something wrong" << std::endl;
                return false;
            }
            return true;
        }

        catch(...) { return false; }

    }

    template <class T>
    //не понятно, почему в параметрах нужно указывать Т
    //дублирование метода
    T SeveralFiles::byteToType(T verf, std::byte fN[], int sizeArr) const {

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


    //разархивация файлов по методу .tar
    bool SeveralFiles::save(const std::string & pathToDir) {

        try {
            for (auto &runVecSeveralFiles : m_files) {

                //я тут понимаю, что можно отправлять без 1ого параметра, но иначе будет использоваться другой метод
                runVecSeveralFiles.save(runVecSeveralFiles, pathToDir);
            }
            return true;
        }
        catch (...){

            return false;

        }
    }
    //проверка на существование файла
    bool SeveralFiles::isValid(const SeveralFiles &severalFiles) const {

        //корректно?
        if (m_files.empty())
            return false;

        for(auto &run_m_files : m_files) {
            if (!(run_m_files.isValid(run_m_files)) )
                return false;
        }


        return true;
    }


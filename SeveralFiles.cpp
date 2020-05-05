//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "SeveralFiles.h"

namespace fs = std::experimental::filesystem;

    //bool SeveralFiles::load(std::list<const std::string > &pathToFiles) {}
    //добавление File в вектор std::vector<File>
    bool SeveralFiles::loadFiles (File  &file)  {

        m_files.push_back(file);
        return true;

    }

    //чтение файлов в директории
    bool SeveralFiles::load(const std::string & pathToDir) {

        int numberFiles=0;
        for (const auto &runDirectory : fs::directory_iterator(pathToDir)) {

            File file;

            if (!fs::is_directory(runDirectory))
                if (file.load(runDirectory.path())) {
                    numberFiles++;
                    m_files.push_back(file);
                }
        }
        std::cout << "Number of files to back up: " << numberFiles << std::endl;
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

        for(auto &runVecSeveralFiles : m_files) {

            //не знаю как обойти этот момент
            int a;
            std::string b;
            int size_file = byteToType(a, runVecSeveralFiles.getFieldName().size, sizeof(runVecSeveralFiles.getFieldName().size) / sizeof(runVecSeveralFiles.getFieldName().size[0]));
            std::string name_file = byteToType(b, runVecSeveralFiles.getFieldName().name, sizeof(runVecSeveralFiles.getFieldName().name) / sizeof(runVecSeveralFiles.getFieldName().name[0]));


            int runToBuffer=0;
            {
                std::ofstream newFile(pathToDir + name_file);
                char *bufferFile = new char[size_file];
                for (auto &run : runVecSeveralFiles.data()) {
                    bufferFile[runToBuffer] = run;
                    runToBuffer++;
                }

                newFile.write(bufferFile,size_file);
                delete [] bufferFile;
            }

            fs::permissions(pathToDir + name_file,
                            static_cast<fs::perms> (byteToType(a, runVecSeveralFiles.getFieldName().mode,
                                                               sizeof(runVecSeveralFiles.getFieldName().mode) / sizeof(runVecSeveralFiles.getFieldName().mode[0]))));
            std::chrono::time_point timePoint = fs::file_time_type::clock::from_time_t(
                    byteToType(a, runVecSeveralFiles.getFieldName().mtime, sizeof(runVecSeveralFiles.getFieldName().mtime) / sizeof(runVecSeveralFiles.getFieldName().mtime[0])));
            fs::last_write_time(pathToDir + name_file, timePoint);

        }
        return true;
    }
    //std::list<File> SeveralFiles::files() const {}
    //проверка на существование файла
    bool SeveralFiles::isValid() const {
        //корректно?
        if (m_files.empty())
            return false;
        return true;
    }


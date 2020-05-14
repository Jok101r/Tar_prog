//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "TarArchive.h"

//заполнение полей fieldName
void recordFieldFiles(std::byte *fN, std::string fieldFiles){


    for (int a = 0; a < fieldFiles.length(); a++) {

        //char prob = fieldFiles[a];
        fN[a] = static_cast<std::byte>(fieldFiles[a]);

    }

}
void recordFieldFiles(std::byte *fN, int fieldFiles){

    std::stringstream numberOnString;
    numberOnString << fieldFiles;
    for (int a = 0; a < numberOnString.str().length(); a++)
        fN[a] = static_cast<std::byte>(numberOnString.str()[a]);
}
//обнуление полей fieldName
void zeroingFields( std::byte *fN, const int size_fN){

    for (int runVec=0; runVec < size_fN; runVec++)
        fN[runVec] = static_cast<std::byte>(0);
}

//заполнение вектора std::vector<File>
void TarArchive::recordFieldToFile(const std::byte *fN, const int size_fN){

    for (int run = 0;run < size_fN; run++)
        m_fileTar.push_back( static_cast<char>(fN[run]) );

    
}
void TarArchive::recordFieldToFile(const fs::path &pathFile ){


    File file_;
    file_.load(pathFile);
    for(int runVec =0; runVec < file_.data().size(); runVec++ )
        m_fileTar.push_back(file_.data()[runVec]);

}

//заполнение нулями до кратности блока в 512 байт
void TarArchive::fillingWithZeroTo512(){

    int byte = m_fileTar.size();
    while (byte % 512 != 0) {
        byte++;
    }

    byte -= m_fileTar.size();
    //не знаю как по другому сделать
    for (int run = 0; run<byte; run++)
        m_fileTar.push_back((char)0);


}
//получение метаданных с файла
//запись в вектор std::vector<File> хидер+файл
void TarArchive::recordTar(const fs::path &pathFile){

    recordFieldFiles(m_fieldName.name, pathFile.filename());

    recordFieldFiles(m_fieldName.mode, static_cast<int>(static_cast<fs::perms>( fs::status( pathFile ).permissions() ) ) );

    struct stat info;
    stat(pathFile.c_str(), &info);
    struct passwd *pw = getpwuid(info.st_uid);

    recordFieldFiles(m_fieldName.uid, static_cast<int>(pw->pw_uid));
    recordFieldFiles(m_fieldName.gid, static_cast<int>(pw->pw_gid));

    recordFieldFiles(m_fieldName.size, static_cast<int>( fs::file_size( pathFile) ) );

    auto timePoint = fs::last_write_time(pathFile);
    std::time_t ctime = fs::file_time_type::clock::to_time_t(timePoint);
    recordFieldFiles(m_fieldName.mtime, static_cast<int>(ctime));

    recordFieldFiles(m_fieldName.link, (fs::is_directory(pathFile) ? 0 : 1) );

   //запись блока хидера (fieldName) в вектор std::vector<File>
    for (auto &p : m_fieldNameVec)
        recordFieldToFile(p.first, p.second);
    
    //заполнение нулями блок
    fillingWithZeroTo512();

    //запись файла в вектор std::vector<File>
    recordFieldToFile(pathFile);
    fillingWithZeroTo512();

    //обнуление хидера (fieldName)
    for (auto &run : m_fieldNameVec)
        zeroingFields(run.first, run.second);

}

//не понятно, почему в параметрах нужно указывать Т
//получение данных из полей fieldName
template <class T>
T TarArchive::byteToType(T verf, std::byte fN[], int sizeArr) const {

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

// проверка на кратность блока в 512 байт
int TarArchive::multiplicity512(int sizeFile){

    int byte = sizeFile;
    while (byte % 512 != 0) {
        byte++;
    }

    byte =  byte - sizeFile;
    return byte;

}

//чтение файла .tar
// отделение блоков хидера
// отделение самого файла
void TarArchive::parsingTar(){

    // пытался использоваться итеарторы, но не получилось
    //std::vector<char>::iterator run;
    //m_file.dataToPath();
    //run = m_file.data(true).begin();
    //while (run != m_file.data(true).end() ) {

    //тут пришлось поменять итератор на простой цикл
    // не совпадает std::distance(m_file.data(true).begin(), m_file.data(true).end())  c m_file.data(true).size()
    // причем цифра дистанца в разы больше размера
    // почему не понятно
    for(int run = 0; run < m_file.data().size()-1; run++){

        //кажется костыль
        for (auto &runVec : m_fieldNameVec) {

            char *bufferField = new char[runVec.second];
            for (int a =0; a < runVec.second; a++){
                bufferField[a] = m_file.data()[run];
                run++;
            }
            recordFieldFiles(runVec.first, bufferField);
            delete[] bufferField;
        }

        run +=multiplicity512(run);
        int a;
        int size_file = byteToType(a, m_fieldName.size, sizeof(m_fieldName.size) / sizeof(m_fieldName.size[0]));

        std::vector<char> fileChar;
        for (int i = 0; i < size_file; i++){
            fileChar.push_back(m_file.data()[run]);
            run++;
        }
        File fileWithField;

        //загрука метаданных в хидер файла
        fileWithField.loadDataField(m_fieldName, fileChar);
        if(!m_archiveFileTar.loadFiles(fileWithField))
            std::cout << "Something wrong. Sorry. Error 1-0";

        run += multiplicity512( run);

        for (auto &runs : m_fieldNameVec)
            zeroingFields(runs.first, runs.second);

        run--;
    }

}

//добавление File в класс TarArchive
void TarArchive::append(const File &file) {

    m_file = file;
}
//добавление SeveralFile в класс TarArchive
void TarArchive::append(const SeveralFiles &severalFiles) {

    m_archiveFileTar = severalFiles;
}

//архивация файлов по методу .tar
File TarArchive::archive() {

    for(int runAllFiles=0; runAllFiles < m_archiveFileTar.getFiles().size();runAllFiles++)
        recordTar(m_archiveFileTar.getFiles()[runAllFiles].getPathFile());

    File file;
    file.load(m_fileTar);

    return file;

}
//разархивация файла по методу .tar
SeveralFiles TarArchive::unarchive() {

    parsingTar();
    return m_archiveFileTar;
}







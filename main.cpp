#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <cstddef>
#include <fstream>
#include <map>
#include <sstream>
#include <pwd.h>
#include <sys/stat.h>


namespace fs = std::experimental::filesystem;


//заполнение полей хидера
void recordFieldFiles(std::byte *fN, std::string fieldFiles){


    for (int a = 0; a < fieldFiles.length(); a++) {

        char ssss = fieldFiles[a];
        fN[a] = static_cast<std::byte>(fieldFiles[a]);

    }

}
//запись полей хидера
void recordFieldFiles(std::byte *fN, int fieldFiles){

    std::stringstream numberOnString;
    numberOnString << fieldFiles;
    for (int a = 0; a < numberOnString.str().length(); a++)
        fN[a] = static_cast<std::byte>(numberOnString.str()[a]);
}


//обнуление полей
void zeroingFields( std::byte *fN, const int size_fN){

    for (int runVec=0; runVec < size_fN; runVec++)
        fN[runVec] = static_cast<std::byte>(0);
}

//поля хидера
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

//путь к файлу
class PathToFile{

private:

    std::string filePath = ""; //путь к файлам
    std::string tarPath = "";

public:
    //указание место сохранение файла
    void filePlace(bool inOrOut){

        std::cout << "Indicate the place to unzip files:\n";
        while (1) {
            std::cin >> filePath;
            if (fs::is_directory(filePath))
                break;
            std::cout << "No path found, try again!\n";
        }

        if (!inOrOut)
            //доработать
            filePath += "/";

        std::cout << "The path is found!\n";

    };

    //указание место нахождения .tar
    void tarPlace(bool inOrOut) {

        std::cout << "Specify the location of the files .tar:\n";
        while (1) {
            std::cin >> tarPath;

            //не корректно работает с архивацией и разархивацией "!"
            if (inOrOut)
                if (fs::is_directory(tarPath))
                    break;
            if (!inOrOut)
                if (!fs::is_directory(tarPath))
                    break;
            std::cout << "No path found, try again!\n";
        }

        std::cout << "The path is found!\n";

    };

    std::string getFilePath() { return filePath; };
    std::string getTarPath()  { return tarPath;  };


};

// -------------------- //

class SeveralFile{

private:

    std::vector<fs::path> allFilesInDirectory;
    std::string archiveFileName = "CustomTar.tar";
    int numberFiles = 0;

    // запись пути файлов в вектор
    void readDirectory(){


        for(const auto &runDirectory : fs::directory_iterator(pathToFile.getFilePath())){
            numberFiles++;
            if (!fs::is_directory(runDirectory))
                allFilesInDirectory.push_back(runDirectory);
        }
        std::cout << "Number of files to back up: " << numberFiles << std::endl;

    }

    void outPath(){

        for(auto &runarr : allFilesInDirectory)
            std::cout << runarr <<std::endl;
    }
    //указание наименование .tar
    void createTarFile(){

        std::cout << "\nEnter a file name tar:\n";
        std::cin >> archiveFileName;
        archiveFileName+=".tar";
        std::cout << archiveFileName;

        archiveFileName = pathToFile.getTarPath() + archiveFileName;

        std::ofstream archiveFileName_ (archiveFileName);
        archiveFileName_.close();

    }

public:

    PathToFile pathToFile;

    void runSeveral(){
        pathToFile.filePlace(true);
        pathToFile.tarPlace(true);

        readDirectory();
        outPath();
        createTarFile();

    }

    std::vector<fs::path> getAllFilesInDirectory() { return allFilesInDirectory; }
    std::string getTarName() { return archiveFileName; }

};
class FileTar{

private:

public:

    PathToFile pathToFile;

    void fileTar(){
        pathToFile.tarPlace(false);
        pathToFile.filePlace(false);

    }


};

// -------------------- //

class ArchivingFiles{

private:
    Field_name fieldName;
    std::map<std::byte*, int> fieldNameVec {
            {fieldName.name, 100},
            {fieldName.mode, 8},
            {fieldName.uid, 8},
            {fieldName.gid, 8},
            {fieldName.size, 12},
            {fieldName.mtime, 12},
            {fieldName.chkSum, 8},
            {fieldName.link, 1},
            {fieldName.linkName, 100}};

    void fillingWithZeroTo512(){

        int byte = fs::file_size(severalFile.getTarName());
        while (byte % 512 != 0) {
            byte++;
        }
        fs::resize_file(severalFile.getTarName(), byte);
    }
    //запись файлов в тар
    void recordFilesToTar(int runAllFiles) {

        std::fstream readInputFiles(severalFile.getAllFilesInDirectory()[runAllFiles]);
        std::fstream tarFiles (severalFile.getTarName(), std::ios::app);

        char *buffer = new char[1];
        while (!readInputFiles.eof()) {
            // побайтовая запись файла
            readInputFiles.read(buffer, 1);
            tarFiles.write(buffer,1);
        }
        delete[] buffer;
        readInputFiles.close();
        tarFiles.close();
    }
    //запись хидера в .tar
    void recordFieldToTar(const std::byte *fN, const int size_fN){

        std::fstream tarFiles (severalFile.getTarName(), std::ios::app);
        for (int run = 0;run < size_fN; run++)
            tarFiles << static_cast<char>(fN[run]);
        tarFiles.close();


    }
    //получение данных файлов
    void recordTar(int runAllFiles){

        recordFieldFiles(fieldName.name, severalFile.getAllFilesInDirectory()[runAllFiles].filename());
        recordFieldFiles(fieldName.mode, static_cast<int>(static_cast<fs::perms>( fs::status( severalFile.getAllFilesInDirectory()[runAllFiles] ).permissions() ) ) );

        struct stat info;
        stat(severalFile.getAllFilesInDirectory()[runAllFiles].c_str(), &info);
        struct passwd *pw = getpwuid(info.st_uid);

        recordFieldFiles(fieldName.uid, static_cast<int>(pw->pw_uid));
        recordFieldFiles(fieldName.gid, static_cast<int>(pw->pw_gid));


        recordFieldFiles(fieldName.size, static_cast<int>( fs::file_size( severalFile.getAllFilesInDirectory()[runAllFiles]) ) );

        auto timePoint = fs::last_write_time(severalFile.getAllFilesInDirectory()[runAllFiles]);
        std::time_t ctime = fs::file_time_type::clock::to_time_t(timePoint);
        recordFieldFiles(fieldName.mtime, static_cast<int>(ctime));

        recordFieldFiles(fieldName.link, (fs::is_directory(severalFile.getAllFilesInDirectory()[runAllFiles]) ? 5 : 0) );

        //запись блок хидера в тар
        for (auto &p : fieldNameVec)
            recordFieldToTar(p.first, p.second);

        //забивание нулями блок
        fillingWithZeroTo512();

        //запись файла в тар
        recordFilesToTar(runAllFiles);
        fillingWithZeroTo512();

        //обнуление структуры
        for (auto &run : fieldNameVec)
            zeroingFields(run.first, run.second);

    }

public:

    SeveralFile severalFile;

    //по штучный разбор файлов для архивирования
    void archiveFiles(){

        severalFile.runSeveral();
        for(int runAllFiles=0; runAllFiles<severalFile.getAllFilesInDirectory().size();runAllFiles++)
            recordTar(runAllFiles);
    }

};
class UnArchivingFiles{

private:
    Field_name fieldName;
    std::map<std::byte*, int> fieldNameVec {
            {fieldName.name, 100},
            {fieldName.mode, 8},
            {fieldName.uid, 8},
            {fieldName.gid, 8},
            {fieldName.size, 12},
            {fieldName.mtime, 12},
            {fieldName.chkSum, 8},
            {fieldName.link, 1},
            {fieldName.linkName, 100}};

    template <class T>
    //не понятно, почему в параметрах нужно указывать Т
    T byteToType(T verf, std::byte fN[], int sizeArr) const {

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
    int multiplicity512(int sizeFile){

        int byte = sizeFile;
        while (byte % 512 != 0) {
            byte++;
        }

        byte =  byte - sizeFile;
        return byte;

    }

    //разбор на блоки
    void parsingTar(){

        std::fstream tarFile (fileTar.pathToFile.getTarPath());

        while (!tarFile.eof()) {

            if (tarFile.tellg() == fs::file_size(fileTar.pathToFile.getTarPath()))
                break;

            for (auto &run : fieldNameVec) {
                char *bufferField = new char[run.second];
                tarFile.read(bufferField, run.second);
                recordFieldFiles(run.first, bufferField);
                delete[] bufferField;
            }

            //не знаю как обойти этот момент
            int a;
            std::string b;

            // работа с содержанием файла
            int size_file = byteToType(a, fieldName.size, sizeof(fieldName.size) / sizeof(fieldName.size[0]));
            std::string name_file = byteToType(b, fieldName.name, sizeof(fieldName.name) / sizeof(fieldName.name[0]));

            tarFile.seekg(multiplicity512(tarFile.tellg()), std::ios::cur);

            // бывают случаи, что файл записывается только в конце цикла
            {
                char *bufferFile = new char[size_file];
                tarFile.read(bufferFile, size_file);
                std::ofstream newFile(fileTar.pathToFile.getFilePath() + name_file);
                newFile.write(bufferFile, size_file);

                delete[] bufferFile;
            }


            // запись характеристик

            fs::permissions( fileTar.pathToFile.getFilePath() + name_file, static_cast<fs::perms> (byteToType(a, fieldName.mode, sizeof(fieldName.mode) / sizeof(fieldName.mode[0]) ) ) );

            std::chrono::time_point timePoint  = fs::file_time_type::clock::from_time_t( byteToType(a, fieldName.mtime, sizeof(fieldName.mtime) / sizeof(fieldName.mtime[0])));


            fs::last_write_time(fileTar.pathToFile.getFilePath() + name_file, timePoint);




            tarFile.seekg(multiplicity512(tarFile.tellg()), std::ios::cur);


            for (auto &run : fieldNameVec)
                zeroingFields(run.first, run.second);

        }

    }

public:

    FileTar fileTar;

    void unArchivingFiles(){
        fileTar.fileTar();
        parsingTar();
    }
};

// ------------------- //


//входной текст
class WelcomeText{

private:

    std::string answerText;
    void MatterChoice(){

        std::cout << "\n" << "Select program functionality:\n";

        while(1){
            std::cout << "-a  file archiving\n"
                      << "-u  file unarchiving\n";
            std::getline(std::cin, answerText);

            if(answerText == "-a" || answerText == "-u")
                break;
        }
    };

public:

    WelcomeText() {
        std::cout << "\n"
                  << "===============================================================================\n"
                  << "====             Welcome to the .tar file archiving program.               ====\n"
                  << "===============================================================================\n";
    };
    void choiceTar() {

        MatterChoice();

        if (answerText == "-a") {
            ArchivingFiles arhciveFiles;
            arhciveFiles.archiveFiles();

        }
        if (answerText == "-u") {
            UnArchivingFiles unArchivingFiles;
            unArchivingFiles.unArchivingFiles();
        }
    }


};

int main() {

    WelcomeText text;
    text.choiceTar();

    return 0;
}

#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <cstddef>
#include <fstream>
#include <map>
#include <sstream>
#include <pwd.h>
#include <sys/stat.h>


//
// сделать архивацию папок И разархивацию
//
//
// запись характеристик в файл
//
namespace fs = std::experimental::filesystem;

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

//входные данные
class InputText{

private:

    std::string answerText;

public:

    InputText() {
        std::cout << "\n"
                  << "===============================================================================\n"
                  << "====             Welcome to the .tar file archiving program.               ====\n"
                  << "===============================================================================\n";
    };

    std::string choiceTar(){

        std::cout << "\n" << "Select program functionality:\n";

        while(1){
            std::cout << "-a  file archiving\n"
                      << "-u  file unarchiving\n";
            std::getline(std::cin, answerText);

            if(answerText == "-a" || answerText == "-u")
                break;
        }
        return answerText;
    };

};

//архивирование данные
class ArhciveFiles{

protected:

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
    std::string filePath = ""; //путь к файлам
    std::string tarPath = "";
    std::string tarName = "CustomTar.tar";
    std::vector<fs::path> allFilesInDirectory;
    int numberFiles = 0;

public:

    ArhciveFiles(){}

    //указание пути файлов для архивирования
    virtual void filePlace(){

        std::cout << "Укажите место нахождение файлов:\n";
        while (1) {
            std::cin >> filePath;
            if (fs::is_directory(filePath))
                break;
            std::cout << "Путь не найден, попробуйте еще раз!\n";
        }

        std::cout << "Путь найден!\n";

    };
    //указание пути создание архива
    virtual void tarPlace(){

        std::cout << "Укажите место архивации .tar:\n";
        while (1) {
            std::cin >> tarPath;
            if (fs::is_directory(tarPath))
                break;
            std::cout << "Путь не найден, попробуйте еще раз!\n";
        }

        std::cout << "Путь найден!\n";

    };
    // запись пути файлов в вектор
    void readDirectory(){

        for(const auto &runDirectory : fs::directory_iterator(filePath)){
            numberFiles++;
            allFilesInDirectory.push_back(runDirectory);
        }
        std::cout << "Количество файлов для архивации: " << numberFiles << std::endl;

    }
    //указание наименование .tar
    void createTarFile(){

        std::cout << "\nУкажите имя tar:\n";
        std::cin >> tarName;
        tarName+=".tar";
        std::cout << tarName;

        std::ofstream tarFiles (tarPath+=tarName);
        tarFiles.close();

    }
    //запись хидера в .tar
    void fieldToTar(const std::byte *fN, const int size_fN){

        std::fstream tarFiles (tarPath, std::ios::app);
        for (int run = 0;run < size_fN; run++)
            tarFiles << static_cast<char>(fN[run]);
        tarFiles.close();
    }
    // запонление нулями для кратности 512 байт
    void block512byteToZero(){

        int byte = fs::file_size(tarPath);
        while (byte % 512 != 0) {
            byte++;
        }
        fs::resize_file(tarPath, byte);
    }
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
    //запись файлов в тар
    void recordFilesToTar(int runAllFiles) {

        std::fstream readInputFiles(allFilesInDirectory[runAllFiles]);
        std::fstream tarFiles (tarPath, std::ios::app);

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
    //обнуление полей
    void zeroingFields( std::byte *fN, const int size_fN){

        for (int runVec=0; runVec < size_fN; runVec++)
            fN[runVec] = static_cast<std::byte>(0);
    }

    //получение данных файлов
    void recordTar(int runAllFiles){

        recordFieldFiles(fieldName.name, allFilesInDirectory[runAllFiles].filename());
        recordFieldFiles(fieldName.mode, static_cast<int>(static_cast<fs::perms>( fs::status( allFilesInDirectory[runAllFiles] ).permissions() ) ) );

        struct stat info;
        stat(allFilesInDirectory[runAllFiles].c_str(), &info);
        struct passwd *pw = getpwuid(info.st_uid);

        recordFieldFiles(fieldName.uid, static_cast<int>(pw->pw_uid));
        recordFieldFiles(fieldName.gid, static_cast<int>(pw->pw_gid));
        recordFieldFiles(fieldName.size, static_cast<int>(fs::file_size(allFilesInDirectory[runAllFiles])));

        auto timePoint = fs::last_write_time(allFilesInDirectory[runAllFiles]);
        std::time_t ctime = fs::file_time_type::clock::to_time_t(timePoint);
        recordFieldFiles(fieldName.mtime, static_cast<int>(ctime));

        recordFieldFiles(fieldName.link, (fs::is_directory(allFilesInDirectory[runAllFiles]) ? 5 : 0) );

        //запись блок хидера в тар
        for (auto &p : fieldNameVec)
            fieldToTar(p.first, p.second);

        //забивание нулями блок
        block512byteToZero();

        //запись файла в тар
        recordFilesToTar(runAllFiles);
        block512byteToZero();

        //обнуление структуры
        for (auto &run : fieldNameVec)
            zeroingFields(run.first, run.second);

    }
    //по штучный разбор файлов
    void filesToTar(){
        for(int runAllFiles=0; runAllFiles<allFilesInDirectory.size();runAllFiles++)
            recordTar(runAllFiles);
    }
    void archiveFilesNow(){
        filePlace();
        tarPlace();
        readDirectory();
        createTarFile();
        filesToTar();
    }


};
//разархивирование файлов
class UnArhciveFiles : public ArhciveFiles {

private:

public:

    //указание место сохранение файла
    virtual void filePlace(){

        std::cout << "Укажите место разархивирование файлов: \n";
        while (1) {
            std::cin >> filePath;
            if (fs::is_directory(filePath))
                break;
            std::cout << "Путь не найден, попробуйте еще раз!\n";
        }
        //доработать
        filePath += "/";

        std::cout << "Путь найден!\n";

    };

    //указание место нахождения .tar
    virtual void tarPlace() override {

        std::cout << "Укажите место нахождения .tar:\n";
        while (1) {
            std::cin >> tarPath;
            if (fs::exists(tarPath))
                if ( !fs::is_directory(tarPath))
                    break;
            std::cout << "Путь не найден, попробуйте еще раз!\n";
        }

        std::cout << "Путь найден!\n";

    };

    template <class T>
    //не понятно, почему в параметрах нужно указывать Т
    T byteToType(T verf, std::byte fN[], int sizeArr) const {

        //появляется мусор при кастить к чару
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
    int howMuchTo512(int sizeFile){

        int byte = sizeFile;
        while (byte % 512 != 0) {
            byte++;
        }

        byte =  byte - sizeFile;
        return byte;

    }
    //разбор на блоки
    void parsingTar(){

        std::fstream tarFile (tarPath);

        while (!tarFile.eof()) {

            if (tarFile.tellg() == fs::file_size(tarPath))
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

            tarFile.seekg(howMuchTo512(tarFile.tellg()), std::ios::cur);

            // бывают случаи, что файл записывается только в конце цикла
            {
                char *bufferFile = new char[size_file];
                tarFile.read(bufferFile, size_file);
                std::ofstream newFile(filePath + name_file);
                newFile.write(bufferFile, size_file);

                delete[] bufferFile;
            }


            // запись характеристик

            fs::permissions( filePath + name_file, static_cast<fs::perms> (byteToType(a, fieldName.mode, sizeof(fieldName.mode) / sizeof(fieldName.mode[0]) ) ) );

            std::chrono::time_point timePoint  = fs::file_time_type::clock::from_time_t( byteToType(a, fieldName.mtime, sizeof(fieldName.mtime) / sizeof(fieldName.mtime[0])));


            fs::last_write_time(filePath + name_file, timePoint);




            tarFile.seekg(howMuchTo512(tarFile.tellg()), std::ios::cur);


            for (auto &run : fieldNameVec)
                zeroingFields(run.first, run.second);

        }

    }

    void unArhciveFilesNow(){

        tarPlace();
        filePlace();
        parsingTar();
    }

};

int main() {

    InputText text;
    std::string answerText = text.choiceTar();
    if (answerText == "-a" ) {
        ArhciveFiles archiveFiles;
        archiveFiles.archiveFilesNow();
    }

    if (answerText == "-u") {
        UnArhciveFiles unArhciveFiles;
        unArhciveFiles.unArhciveFilesNow();
    }

    return 0;
}

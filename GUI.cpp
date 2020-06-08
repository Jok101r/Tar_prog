//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "GUI.h"

static const char USAGE[] =
        R"(Tar_prog.

    Usage:
      tar_prog atar <path to files> <path to save> <name file>
      tar_prog untar <path to file tar> <path to folder unarchiving>
      tar_prog (-h | --help)
      tar_prog (-v | --version)

    Options:
      -h --help        Show this screen.
      -v --version     Show version.

For example:
    tar_prog atar /admin/tmp /user/tmp archivename
)";

//чтение данных из командной строки
void GUI::parseArgs(int argc, const char** argv) {

    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               { argv + 1, argv + argc },
                                                               true,               // show help if requested
                                                               "Archive 1.0");  // version string



//    for(auto const& arg : args) {
//        std::cout << arg.first << ": " << arg.second << std::endl;
//    }

    std::map<std::string, docopt::value> ::iterator it;


    if (args.find("atar")->second.asBool())
    {
        m_aruments = "atar";
        it = args.find("<path to files>");
        m_path_to_files = it->second.asString();
        it = args.find("<path to save>");
        m_path_to_save = it->second.asString();
        it = args.find("<name file>");
        m_name_fileArh = it->second.asString();

    }

    if (args.find("untar")->second.asBool())
    {

        m_aruments = "untar";
        it = args.find("<path to file tar>");
        m_path_to_file = it->second.asString();
        it = args.find("<path to folder unarchiving>");
        m_path_to_files = it->second.asString();

    }


}

//ключи пользования программы
Key GUI::keys(){

    if(m_aruments == "atar")
        return m_keys[0];

    if(m_aruments == "untar")
        return m_keys[1];

    return m_keys[2];

}

//взаимодействие с пользованием
//загрузка пути
//std::string GUI::loadLine(bool is_directory) {
//
//    std::cin >> m_path;
//
//    return m_path;
//
//}

std::string GUI::nameCreateTar(const std::string &pathToResultFile){

    std::cout << "\nEnter a file name tar:\n";
    std::cin >> m_archiveFileName;
    m_archiveFileName+=".tar";

    m_archiveFileName = pathToResultFile + m_archiveFileName;

    return m_archiveFileName;

}

const std::string &GUI::getMPathToFiles() const {
    return m_path_to_files;
}

const std::string &GUI::getMPathToFileArch() const {
    return m_path_to_fileArch;
}

const std::string &GUI::getMPathToSave() const {
    return m_path_to_save;
}

const std::string &GUI::getMNameFileArh() const {
    return m_name_fileArh;
}

const std::string &GUI::getMPathToFile() const {
    return m_path_to_file;
}


//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "GUI.h"

static const char USAGE[] =
        R"(Tar_prog.

    Usage:
      Tar_prog -a  file archiving
      Tar_prog -untar file unarchiving
      Tar_prog (-h | --help)
      Tar_prog (-v | --version)

    Options:
      -a
      -h --help        Show this screen.
      -v --version     Show version.
)";

//чтение данных из командной строки
void GUI::parseArgs(int argc, const char** argv) {

    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               { argv + 1, argv + argc },
                                                               true,               // show help if requested
                                                               "Archive 1.0");  // version string


    for(auto const& arg : args) {
        std::cout << arg.first << ": " << arg.second << std::endl;
        if(arg.first.find("-a") || arg.first.find("-untar"))
            m_aruments += arg.first;
        arg.second.
    }



    /*for (int count = 0; count < argc; ++count) {
        std::cout << count << " " << argv[count] << '\n';
        m_aruments[count] = argv[count] ;
    }
     */

}

//ключи пользования программы
Key GUI::keys(){


//    if (strcmp(m_aruments[1],"atar") == 0)
//        return m_keys[0];
//    if (strcmp(m_aruments[1],"untar") == 0)
//        return m_keys[1];
    if(m_aruments == "-a")
        return m_keys[0];

    if(m_aruments == "-untar")
        return m_keys[1];



}

//взаимодействие с пользованием
//загрузка пути
std::string GUI::loadLine(bool is_directory) {

    std::cin >> m_path;

    return m_path;

}

std::string GUI::nameCreateTar(const std::string &pathToResultFile){

    std::cout << "\nEnter a file name tar:\n";
    std::cin >> m_archiveFileName;
    m_archiveFileName+=".tar";

    m_archiveFileName = pathToResultFile + m_archiveFileName;

    return m_archiveFileName;

}


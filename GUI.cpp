//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#include "GUI.h"

//чтение данных из командной строки
void GUI::parseArgs(int argc, char *argv[]) {

    for (int count = 0; count < argc; ++count) {
        std::cout << count << " " << argv[count] << '\n';
        m_aruments[count] = argv[count] ;
    }

}

//ключи пользования программы
Key GUI::keys(){


    if (strcmp(m_aruments[1],"atar") == 0)
        return m_keys[0];
    if (strcmp(m_aruments[1],"untar") == 0)
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


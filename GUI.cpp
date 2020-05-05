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
fs::path GUI::loadLine(bool is_directory) {

    while (1) {

        std::cin >> m_path;

        //некорретно отображает место разархивации, нужно указать в конце '\'
        if (is_directory)
            if (fs::is_directory(m_path))
                break;

        if (!is_directory)
            if (!fs::is_directory(m_path))
                break;

        std::cout << "No path found, try again!\n";
    }

    std::cout << "The path is found!\n";

    return m_path;

}

std::string GUI::nameCreateTar(const std::string &pathToResultFile){

    std::cout << "\nEnter a file name tar:\n";
    std::cin >> m_archiveFileName;
    m_archiveFileName+=".tar";

    m_archiveFileName = pathToResultFile + m_archiveFileName;

    return m_archiveFileName;

}


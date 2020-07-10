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


template <class T>
T checkArgs(T &args){


}
//чтение данных из командной строки
GUI::EntredArgs GUI::parseArgs(int argc, const char** argv) {

    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               { argv + 1, argv + argc },
                                                               true,               // show help if requested
                                                               "Archive 1.1");  // version string




    if (args.at("atar").asBool() == true){
        GUI::EntredArgs::Atar atar_(args.at("atar").asBool(),
                                    args.at("<path to files>").asString(),
                                    args.at("<path to save>").asString(),
                                    args.at("<name file>").asString());
        GUI::EntredArgs::Untar untar_;
        GUI::EntredArgs argss(atar_,untar_);
        return argss;
    }
    if (args.at("untar").asBool() == true){
        GUI::EntredArgs::Untar untar_(args.at("untar").asBool(),
                                      args.at("<path to file tar>").asString(),
                                      args.at("<path to folder unarchiving>").asString());
        GUI::EntredArgs::Atar atar_;

        GUI::EntredArgs argss(atar_,untar_);
        return argss;
    }


}

//ключи пользования программы
Key GUI::keys(GUI::EntredArgs &args){


    if(args.atar.flag == true)
        return m_keys[0];

    if(args.untar.flag == true)
        return m_keys[1];

    return m_keys[2];

}


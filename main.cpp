
#include "File.h"
#include "SeveralFiles.h"
#include "TarArchive.h"
#include "GUI.h"




int main(int argc, const char** argv)
{
    GUI gui;
    gui.parseArgs(argc, argv);



    switch (gui.keys())
    {


        case Key::ARCHIVE_TAR:
        {
            File file;
            SeveralFiles files;
            if (!file.isDirectory(gui.getMPathToFiles())){
                gui.writeLine(gui.getMPathToFiles());
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }
            if (!file.isDirectory(gui.getMPathToSave())){
                gui.writeLine(gui.getMPathToFiles());
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }
            if(files.load(gui.getMPathToFiles()))
            {
                TarArchive archive;
                archive.append(files);
                auto archiveFile = archive.archive();


                if (archiveFile.isValid())
                {
                    //исправить
                    auto pathTar = gui.getMPathToSave()+gui.getMNameFileArh()+".tar";

                    if (archiveFile.save(pathTar))
                    {
                        gui.writeLine("Ok");
                    }
                    else
                    {
                        gui.writeLine("Something wrong. Sorry");
                    }

                }
                else
                {
                    gui.writeLine("Something wrong. Sorry");
                }
            }else {gui.writeLine("Something wrong. Sorry");}

        };
        break;
        case Key::UNARCHIVE_TAR:
        {
            File file;

            TarArchive archive;
            if (file.isDirectory(gui.getMPathToFile())){
                gui.writeLine(gui.getMPathToFile());
                gui.writeLine("It's not file .tar Try again.\n");
                break;

            }
            if(!archive.isTar(gui.getMPathToFile())) {
                gui.writeLine(gui.getMPathToFile());
                gui.writeLine("It's not file .tar Try again.\n");
                break;
            }

            if (!file.isDirectory(gui.getMPathToFiles())){
                gui.writeLine(gui.getMPathToFiles());
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }

            if (file.load(gui.getMPathToFile()))
            {
                archive.append(file);
                auto resultFiles = archive.unarchive();
                if (resultFiles.isValid(resultFiles))
                {


                    if (resultFiles.save(gui.getMPathToFiles()))
                    {
                        gui.writeLine("Ok");
                    }
                    else
                    {
                        gui.writeLine("Something wrong. Sorry");
                    }

                }
                else
                {
                    gui.writeLine("Something wrong. Sorry");
                }
            }

        }; break;
        default:
            std::cout << "\nUnknown\n";
            break;
    }
}
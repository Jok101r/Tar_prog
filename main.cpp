
#include "File.h"
#include "SeveralFiles.h"
#include "TarArchive.h"
#include "GUI.h"



int main(int argc, char* argv[])
{
    GUI gui;
    gui.writeLine("Wellcome");
    gui.writeLine("Select program functionality:\n");
    gui.writeLine(" 'Atar'  file archiving");
    gui.writeLine(" 'Untar' file unarchiving");
    gui.parseArgs(argc, argv);

    switch (gui.keys())
    {

        case Key::ARCHIVE_TAR:
        {
            File file;
            gui.writeLine("Enter path to dir with files to archive:");

            //переделал раздел
            std::string pathToDir = "";
            while(1) {

                pathToDir = gui.loadLine(true);

                if(file.isDirectory(pathToDir) == false)
                    gui.writeLine("No path found, try again!\n");
                if(file.isDirectory(pathToDir) == true){
                    gui.writeLine("The path is found!\n");
                    break;
                }
            }

            gui.writeLine("Enter path to result file:");

            //
            std::string pathToResultFile = "";
            while(1) {

                pathToResultFile = gui.loadLine(true);

                if(file.isDirectory(pathToResultFile) == false)
                    gui.writeLine("No path found, try again!\n");
                if(file.isDirectory(pathToResultFile) == true) {
                    gui.writeLine("The path is found!\n");
                    break;
                }
            }


            SeveralFiles files;
            if (files.load(pathToDir))
            {
                TarArchive archive;
                archive.append(files);
                auto archiveFile = archive.archive();


                if (archiveFile.isValid())
                {
                    auto pathTar = gui.nameCreateTar(pathToResultFile);

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
            }

        };
        break;
        case Key::UNARCHIVE_TAR:
        {
            File file;
            std::string pathToArchiveFile;
            while(1) {
                gui.writeLine("Enter path to archive-file:");
                pathToArchiveFile = gui.loadLine(false);

                if(file.isDirectory(pathToArchiveFile) == true)
                    gui.writeLine("No path found, try again!\n");
                if(file.isDirectory(pathToArchiveFile) == false) {
                    gui.writeLine("The path is found!\n");
                    break;
                }
            }

            gui.writeLine("Enter path to result dir:");
            std::string pathToResultDir;
            while(1) {

                pathToResultDir = gui.loadLine(true);

                if(file.isDirectory(pathToResultDir) == false)
                    gui.writeLine("No path found, try again!\n");
                if(file.isDirectory(pathToResultDir) == true) {
                    gui.writeLine("The path is found!\n");
                    break;
                }
            }

            if (file.load(pathToArchiveFile))
            {
                TarArchive archive;
                archive.append(file);
                auto resultFiles = archive.unarchive();
                if (resultFiles.isValid(resultFiles))
                {


                    if (resultFiles.save(pathToResultDir))
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
#include "IArchive.h"
#include "File.h"
#include "SeveralFiles.h"
#include "TarArchive.h"
#include "GUI.h"


namespace fs = std::experimental::filesystem;

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
            gui.writeLine("Enter path to dir with files to archive:");
            auto pathToDir = gui.loadLine(true);

            gui.writeLine("Enter path to result file:");
            auto pathToResultFile = gui.loadLine(true);

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
            }

        };
        break;
        case Key::UNARCHIVE_TAR:
        {
            gui.writeLine("Enter path to archive-file:");
            auto pathToArchiveFile = gui.loadLine(false);

            gui.writeLine("Enter path to result dir:");
            auto pathToResultDir = gui.loadLine(true);

            File file;
            if (file.load(pathToArchiveFile))
            {
                TarArchive archive;
                archive.append(file);
                auto resultFiles = archive.unarchive();
                if (resultFiles.isValid())
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
            }

        }; break;
        default:
            std::cout << "\nUnknown\n";
            break;
    }
}
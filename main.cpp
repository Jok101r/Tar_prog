
#include "File.h"
#include "SeveralFiles.h"
#include "TarArchive.h"
#include "GUI.h"

namespace  fs = std::filesystem;
int main(int argc, const char** argv)
{

    GUI gui;
    GUI::EntredArgs args = gui.parseArgs(argc, argv);

    switch (gui.keys(args))
    {
        case Key::ARCHIVE_TAR:
        {

            File file;
            SeveralFiles files;

            if (!fs::is_directory(args.atar.pathToFiles) ) {
                gui.writeLine(args.atar.pathToFiles);
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }
            if (!fs::is_directory(args.atar.pathToSave)){
                gui.writeLine(args.atar.pathToSave);
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }

            if(files.load(args.atar.pathToFiles))
            {


                TarArchive archive;

                archive.append(files);



                auto archiveFile = archive.archive();


                if (archiveFile.isValid())
                {
                    //исправить
                    auto pathTar = args.atar.pathToSave+args.atar.pathToFile+".tar";

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
            if (fs::is_directory(args.untar.pathToFile)){
                gui.writeLine(args.untar.pathToFile);
                gui.writeLine("It's not file .tar Try again.\n");
                break;

            }
            if(!archive.isTar(args.untar.pathToFile)) {
                gui.writeLine(args.untar.pathToFile);
                gui.writeLine("It's not file .tar Try again.\n");
                break;
            }

            if (!fs::is_directory(args.untar.pathToFolder)){
                gui.writeLine(args.untar.pathToFolder);
                gui.writeLine("It's not directory. Try again.\n");
                break;
            }

            if (file.load(args.untar.pathToFile))
            {
                archive.append(file);
                auto resultFiles = archive.unarchive();
                if (resultFiles.isValid(resultFiles))
                {


                    if (resultFiles.save(args.untar.pathToFolder))
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

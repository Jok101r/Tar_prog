//
// Created by Aleksandr Akumyanskiy on 11.04.2020.
//

#ifndef TAR_PROG_IARCHIVE_H
#define TAR_PROG_IARCHIVE_H

#include "File.h"
#include "SeveralFiles.h"

class IArchive{

public:
    virtual void append(const File &) = 0;
    virtual void append(const SeveralFiles &) = 0;
    virtual File archive() = 0;
    virtual SeveralFiles unarchive() = 0;

};


#endif //TAR_PROG_IARCHIVE_H

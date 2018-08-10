#include "exceptions.h"

void FileOpenFailureException::raise() const {
    throw *this;
}

FileOpenFailureException* FileOpenFailureException::clone() const {
    return new FileOpenFailureException{*this};
}

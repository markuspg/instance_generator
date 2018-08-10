#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QException>

class FileOpenFailureException : public QException {
public:
    void raise() const override;
    FileOpenFailureException *clone() const override;
};

#endif // EXCEPTIONS_H

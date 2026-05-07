#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H
#include <QList>

#include "student.h"

class StudentManager
{
private:
    QList<Student> students;
public:
    StudentManager();
};

#endif // STUDENTMANAGER_H

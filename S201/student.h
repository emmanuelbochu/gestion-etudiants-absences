#ifndef STUDENT_H
#define STUDENT_H
#include <QString>
#include <QDate>
#include <QList>

#include "section.h"
#include "absence.h"

class Student
{
private:
    QString lastName;
    QString firstName;
    QDate birthDate;
    QString studentID;
    QString ine;
    QString phoneNumber;
    QString schoolEmail;
    QString personalEmail;
    QString adress;
    QString parentsAdress;
    QString photoId;
    Section section;
    QList<Absence> absences;



public:
    Student();
};

#endif // STUDENT_H

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
    QString address;         // Corrigé (deux 'd')
    QString parentsAddress;  // Corrigé
    QString photoId;
    Section section;
    QList<Absence> absences;

public:
    // Constructeurs
    Student();
    Student(const QString &lastName, const QString &firstName, const QString &studentID);

    // Getters
    QString getLastName() const;
    QString getFirstName() const;
    QDate getBirthDate() const;
    QString getStudentID() const;
    QString getIne() const;
    QString getPhoneNumber() const;
    QString getSchoolEmail() const;
    QString getPersonalEmail() const;
    QString getAddress() const;
    QString getParentsAddress() const;
    QString getPhotoId() const;
    Section getSection() const;
    QList<Absence> getAbsences() const;

    // Setters
    void setLastName(const QString &value);
    void setFirstName(const QString &value);
    void setBirthDate(const QDate &value);
    void setStudentID(const QString &value);
    void setIne(const QString &value);
    void setPhoneNumber(const QString &value);
    void setSchoolEmail(const QString &value);
    void setPersonalEmail(const QString &value);
    void setAddress(const QString &value);
    void setParentsAddress(const QString &value);
    void setPhotoId(const QString &value);
    void setSection(const Section &value);
    void setAbsences(const QList<Absence> &value);

    // Fonctions utilitaires pour le sous-formulaire du DetailWidget
    void addAbsence(const Absence &absence);
    void removeAbsence(int index);
};

#endif // STUDENT_H

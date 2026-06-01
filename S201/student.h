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
    int studentId;
    QString lastName;
    QString firstName;
    QString phoneNumber;
    QString amuEmail;
    QString personalEmail;
    QString personalAddress;
    QString parentsAddress;
    QString photoPath;
    QString ineNumber;
    QDate birthDate;
    Section section;
    QList<Absence> absences;

public:
    Student(int studentId, const QString &lastName, const QString &firstName, const QString &phoneNumber, const QString &amuEmail, const QString &personalEmail, const QString &personalAddress, const QString &parentsAddress, const QString &photoPath, const QString &ineNumber, const QDate &birthDate, Section section);
    ~Student();

    // Getters
    int getStudentId() const;
    QString getLastName() const;
    QString getFirstName() const;
    QString getPhoneNumber() const;
    QString getAmuEmail() const;
    QString getPersonalEmail() const;
    QString getPersonalAddress() const;
    QString getParentsAddress() const;
    QString getPhotoPath() const;
    QString getIneNumber() const;
    QDate getBirthDate() const;
    Section getSection() const;
    QList<Absence> getAbsences() const;

    // Setters
    void setStudentId(int studentId);
    void setLastName(const QString &lastName);
    void setFirstName(const QString &firstName);
    void setPhoneNumber(const QString &phoneNumber);
    void setAmuEmail(const QString &amuEmail);
    void setPersonalEmail(const QString &personalEmail);
    void setPersonalAddress(const QString &personalAddress);
    void setParentsAddress(const QString &parentsAddress);
    void setPhotoPath(const QString &photoPath);
    void setIneNumber(const QString &ineNumber);
    void setBirthDate(const QDate &birthDate);
    void setSection(Section section);
    void setAbsences(const QList<Absence> &absences);

    // Méthodes de gestion des absences
    void addAbsence(const Absence &absence);
    void removeAbsence(int index);
};

#endif // STUDENT_H
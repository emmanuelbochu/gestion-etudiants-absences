#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDomDocument>
#include <QDomElement>
#include "section.h"
#include "absence.h"

class Student
{
private:
    QString lastName;
    QString firstName;
    QDate birthDate;
    QString studentId;
    QString ineNumber;
    QString phoneNumber;
    QString amuEmail;
    QString personalEmail;
    QString personalAddress;
    QString parentAddress;
    QString photoPath;
    Section section;
    QList<Absence> absences;

public:
    // Constructeurs
    Student();
    Student(QString lastName, QString firstName, QString studentId);

    // Getters
    QString getLastName() const;
    QString getFirstName() const;
    QDate getBirthDate() const;
    QString getStudentId() const;
    QString getIneNumber() const;
    QString getPhoneNumber() const;
    QString getAmuEmail() const;
    QString getPersonalEmail() const;
    QString getPersonalAddress() const;
    QString getParentAddress() const;
    QString getPhotoPath() const;
    Section getSection() const;
    QList<Absence> getAbsences() const;

    // Setters
    void setLastName(QString v);
    void setFirstName(QString v);
    void setBirthDate(QDate v);
    void setStudentId(QString v);
    void setIneNumber(QString v);
    void setPhoneNumber(QString v);
    void setAmuEmail(QString v);
    void setPersonalEmail(QString v);
    void setPersonalAddress(QString v);
    void setParentAddress(QString v);
    void setPhotoPath(QString v);
    void setSection(Section v);

    // Gestion des absences
    void addAbsence(Absence a);
    void removeAbsence(int index);
    void editAbsence(int index, Absence a);

    // Nom d'affichage pour la liste Master (Nom Prénom)
    QString getDisplayName() const;

    // Sérialisation JSON et XML
    QJsonObject toJson() const;
    QDomElement toXml(QDomDocument &doc) const;

    // Désérialisation
    static Student fromJson(const QJsonObject &obj);
    static Student fromXml(const QDomElement &elem);
};

#endif // STUDENT_H
#include "student.h"

// Constructeur par défaut
Student::Student() {
    lastName = "";
    firstName = "";
    studentID = "";
    ine = "";
    phoneNumber = "";
    schoolEmail = "";
    personalEmail = "";
    address = "";
    parentsAddress = "";
    photoId = "";
    // birthDate et section s'initialisent avec leurs constructeurs par défaut
}

// Constructeur surchargé
Student::Student(const QString &lName, const QString &fName, const QString &sID) {
    lastName = lName;
    firstName = fName;
    studentID = sID;
    ine = "";
    phoneNumber = "";
    schoolEmail = "";
    personalEmail = "";
    address = "";
    parentsAddress = "";
    photoId = "";
}

// GETTERS
QString Student::getLastName() const { return lastName; }
QString Student::getFirstName() const { return firstName; }
QDate Student::getBirthDate() const { return birthDate; }
QString Student::getStudentID() const { return studentID; }
QString Student::getIne() const { return ine; }
QString Student::getPhoneNumber() const { return phoneNumber; }
QString Student::getSchoolEmail() const { return schoolEmail; }
QString Student::getPersonalEmail() const { return personalEmail; }
QString Student::getAddress() const { return address; }
QString Student::getParentsAddress() const { return parentsAddress; }
QString Student::getPhotoId() const { return photoId; }
Section Student::getSection() const { return section; }
QList<Absence> Student::getAbsences() const { return absences; }

// SETTERS
void Student::setLastName(const QString &value) { lastName = value; }
void Student::setFirstName(const QString &value) { firstName = value; }
void Student::setBirthDate(const QDate &value) { birthDate = value; }
void Student::setStudentID(const QString &value) { studentID = value; }
void Student::setIne(const QString &value) { ine = value; }
void Student::setPhoneNumber(const QString &value) { phoneNumber = value; }
void Student::setSchoolEmail(const QString &value) { schoolEmail = value; }
void Student::setPersonalEmail(const QString &value) { personalEmail = value; }
void Student::setAddress(const QString &value) { address = value; }
void Student::setParentsAddress(const QString &value) { parentsAddress = value; }
void Student::setPhotoId(const QString &value) { photoId = value; }
void Student::setSection(const Section &value) { section = value; }
void Student::setAbsences(const QList<Absence> &value) { absences = value; }

// FONCTIONS UTILITAIRES
void Student::addAbsence(const Absence &absence) {
    absences.append(absence); // Ajoute une absence à la liste
}

void Student::removeAbsence(int index) {
    // On vérifie que l'index existe bien dans la liste avant de supprimer
    if (index >= 0 && index < absences.size()) {
        absences.removeAt(index);
    }
}

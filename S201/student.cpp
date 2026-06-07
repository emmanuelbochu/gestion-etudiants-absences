#include "student.h"

Student::Student() {
    section = Section::INFO_BUT_1;
    birthDate = QDate::currentDate();
}

Student::Student(QString lastName, QString firstName, QString studentId) {
    this->lastName = lastName;
    this->firstName = firstName;
    this->studentId = studentId;
    this->section = Section::INFO_BUT_1;
    this->birthDate = QDate(2006, 1, 1); // Date par défaut générique
}

// --- Getters ---
QString Student::getLastName() const { return lastName; }
QString Student::getFirstName() const { return firstName; }
QDate Student::getBirthDate() const { return birthDate; }
QString Student::getStudentId() const { return studentId; }
QString Student::getIneNumber() const { return ineNumber; }
QString Student::getPhoneNumber() const { return phoneNumber; }
QString Student::getAmuEmail() const { return amuEmail; }
QString Student::getPersonalEmail() const { return personalEmail; }
QString Student::getPersonalAddress() const { return personalAddress; }
QString Student::getParentAddress() const { return parentAddress; }
QString Student::getPhotoPath() const { return photoPath; }
Section Student::getSection() const { return section; }
QList<Absence> Student::getAbsences() const { return absences; }

// --- Setters ---
void Student::setLastName(QString v) { lastName = v; }
void Student::setFirstName(QString v) { firstName = v; }
void Student::setBirthDate(QDate v) { birthDate = v; }
void Student::setStudentId(QString v) { studentId = v; }
void Student::setIneNumber(QString v) { ineNumber = v; }
void Student::setPhoneNumber(QString v) { phoneNumber = v; }
void Student::setAmuEmail(QString v) { amuEmail = v; }
void Student::setPersonalEmail(QString v) { personalEmail = v; }
void Student::setPersonalAddress(QString v) { personalAddress = v; }
void Student::setParentAddress(QString v) { parentAddress = v; }
void Student::setPhotoPath(QString v) { photoPath = v; }
void Student::setSection(Section v) { section = v; }

// --- Gestion des Absences ---
void Student::addAbsence(Absence a) { absences.append(a); }

void Student::removeAbsence(int index) {
    if (index >= 0 && index < absences.size()) {
        absences.removeAt(index);
    }
}

void Student::editAbsence(int index, Absence a) {
    if (index >= 0 && index < absences.size()) {
        absences[index] = a;
    }
}

// Format d'affichage attendu pour la liste Master gauche
QString Student::getDisplayName() const {
    return lastName.toUpper() + " " + firstName + " (" + studentId + ")";
}

// --- Sérialisation JSON ---
QJsonObject Student::toJson() const {
    QJsonObject obj;
    obj["lastName"] = lastName;
    obj["firstName"] = firstName;
    obj["birthDate"] = birthDate.toString(Qt::ISODate);
    obj["studentId"] = studentId;
    obj["ineNumber"] = ineNumber;
    obj["phoneNumber"] = phoneNumber;
    obj["amuEmail"] = amuEmail;
    obj["personalEmail"] = personalEmail;
    obj["personalAddress"] = personalAddress;
    obj["parentAddress"] = parentAddress;
    obj["photoPath"] = photoPath;
    obj["section"] = sectionToString(section);

    QJsonArray absArray;
    for (const Absence &a : absences) {
        absArray.append(a.toJson());
    }
    obj["absences"] = absArray;

    return obj;
}

Student Student::fromJson(const QJsonObject &obj) {
    Student s;
    s.lastName = obj["lastName"].toString();
    s.firstName = obj["firstName"].toString();
    s.birthDate = QDate::fromString(obj["birthDate"].toString(), Qt::ISODate);
    s.studentId = obj["studentId"].toString();
    s.ineNumber = obj["ineNumber"].toString();
    s.phoneNumber = obj["phoneNumber"].toString();
    s.amuEmail = obj["amuEmail"].toString();
    s.personalEmail = obj["personalEmail"].toString();
    s.personalAddress = obj["personalAddress"].toString();
    s.parentAddress = obj["parentAddress"].toString();
    s.photoPath = obj["photoPath"].toString();
    s.section = sectionFromString(obj["section"].toString());

    QJsonArray absArray = obj["absences"].toArray();
    for (int i = 0; i < absArray.size(); ++i) {
        s.absences.append(Absence::fromJson(absArray[i].toObject()));
    }
    return s;
}

// --- Sérialisation XML ---
QDomElement Student::toXml(QDomDocument &doc) const {
    QDomElement elem = doc.createElement("student");
    elem.setAttribute("studentId", studentId);
    elem.setAttribute("section", sectionToString(section));

    auto addTextElem = [&](QString tagName, QString value) {
        QDomElement t = doc.createElement(tagName);
        t.appendChild(doc.createTextNode(value));
        elem.appendChild(t);
    };

    addTextElem("lastName", lastName);
    addTextElem("firstName", firstName);
    addTextElem("birthDate", birthDate.toString(Qt::ISODate));
    addTextElem("ineNumber", ineNumber);
    addTextElem("phoneNumber", phoneNumber);
    addTextElem("amuEmail", amuEmail);
    addTextElem("personalEmail", personalEmail);
    addTextElem("personalAddress", personalAddress);
    addTextElem("parentAddress", parentAddress);
    addTextElem("photoPath", photoPath);

    QDomElement absContainer = doc.createElement("absences");
    for (const Absence &a : absences) {
        absContainer.appendChild(a.toXml(doc));
    }
    elem.appendChild(absContainer);

    return elem;
}

Student Student::fromXml(const QDomElement &elem) {
    Student s;
    s.studentId = elem.attribute("studentId");
    s.section = sectionFromString(elem.attribute("section"));

    s.lastName = elem.firstChildElement("lastName").text();
    s.firstName = elem.firstChildElement("firstName").text();
    s.birthDate = QDate::fromString(elem.firstChildElement("birthDate").text(), Qt::ISODate);
    s.ineNumber = elem.firstChildElement("ineNumber").text();
    s.phoneNumber = elem.firstChildElement("phoneNumber").text();
    s.amuEmail = elem.firstChildElement("amuEmail").text();
    s.personalEmail = elem.firstChildElement("personalEmail").text();
    s.personalAddress = elem.firstChildElement("personalAddress").text();
    s.parentAddress = elem.firstChildElement("parentAddress").text();
    s.photoPath = elem.firstChildElement("photoPath").text();

    QDomElement absContainer = elem.firstChildElement("absences");
    QDomNodeList list = absContainer.elementsByTagName("absence");
    for (int i = 0; i < list.size(); ++i) {
        s.absences.append(Absence::fromXml(list.at(i).toElement()));
    }
    return s;
}
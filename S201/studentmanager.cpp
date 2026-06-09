#include "studentmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDomDocument>
#include <QRegularExpression>
#include <QTextStream>

StudentManager::StudentManager() {
    modified = false;
    currentFilePath = "";
}

QList<Student> StudentManager::getStudents() const { return students; }

Student& StudentManager::getStudent(int index) { return students[index]; }

QString StudentManager::getCurrentFilePath() const { return currentFilePath; }

bool StudentManager::isModified() const { return modified; }

void StudentManager::setModified(bool v) { modified = v; }

void StudentManager::addStudent(Student s) {
    students.append(s);
    modified = true;
}

void StudentManager::removeStudent(int index) {
    if (index >= 0 && index < students.size()) {
        students.removeAt(index);
        modified = true;
    }
}

void StudentManager::updateStudent(int index, Student s) {
    if (index >= 0 && index < students.size()) {
        students[index] = s;
        modified = true;
    }
}

int StudentManager::count() const { return students.size(); }

void StudentManager::clear() {
    students.clear();
    modified = true;
}

// --- Filtrage Regex avancé ---
QList<int> StudentManager::filter(QString pattern) {
    QList<int> matchingIndices;
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < students.size(); ++i) {
        if (students[i].getDisplayName().contains(regex)) {
            matchingIndices.append(i);
        }
    }
    return matchingIndices;
}

// --- Fichiers JSON ---
bool StudentManager::loadJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;

    students.clear();
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i) {
        students.append(Student::fromJson(array[i].toObject()));
    }

    currentFilePath = path;
    modified = false;
    return true;
}

bool StudentManager::saveJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonArray array;
    for (const Student &s : students) {
        array.append(s.toJson());
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
    currentFilePath = path;
    modified = false;
    return true;
}

// --- Fichiers XML ---
bool StudentManager::loadXml(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDomDocument doc;
    if (!doc.setContent(&file)) return false;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "students") return false;

    students.clear();
    QDomNodeList list = root.elementsByTagName("student");
    for (int i = 0; i < list.size(); ++i) {
        students.append(Student::fromXml(list.at(i).toElement()));
    }

    currentFilePath = path;
    modified = false;
    return true;
}

bool StudentManager::saveXml(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QDomDocument doc;
    QDomElement root = doc.createElement("students");
    doc.appendChild(root);

    for (const Student &s : students) {
        root.appendChild(s.toXml(doc));
    }

    QTextStream stream(&file);
    doc.save(stream, 4);
    currentFilePath = path;
    modified = false;
    return true;
}
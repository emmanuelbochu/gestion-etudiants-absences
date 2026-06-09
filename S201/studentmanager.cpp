#include "studentmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDomDocument>
#include <QRegularExpression>
#include <QTextStream>

// constructeur : au départ pas de modifs et pas de fichier chargé
StudentManager::StudentManager() {
    modified = false;
    currentFilePath = "";
}

// getters basiques
QList<Student> StudentManager::getStudents() const { return students; }
Student& StudentManager::getStudent(int index) { return students[index]; } // retourne une référence pour pouvoir modifier directement
QString StudentManager::getCurrentFilePath() const { return currentFilePath; }
bool StudentManager::isModified() const { return modified; }
void StudentManager::setModified(bool v) { modified = v; }

// ajoute un étudiant à la liste
void StudentManager::addStudent(Student s) {
    students.append(s);
    modified = true;
}

// supprime l'étudiant à l'index donné (on vérifie que l'index est valide)
void StudentManager::removeStudent(int index) {
    if (index >= 0 && index < students.size()) {
        students.removeAt(index);
        modified = true;
    }
}

// remplace l'étudiant à l'index par le nouveau
void StudentManager::updateStudent(int index, Student s) {
    if (index >= 0 && index < students.size()) {
        students[index] = s;
        modified = true;
    }
}

// retourne le nombre d'étudiants dans la liste
int StudentManager::count() const { return students.size(); }

// vide toute la liste
void StudentManager::clear() {
    students.clear();
    modified = true;
}

// --- Filtrage Regex avancé ---

// retourne les indices des étudiants dont le nom correspond au pattern (regex, insensible à la casse)
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

// charge la liste des étudiants depuis un fichier JSON
bool StudentManager::loadJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false; // fichier pas accessible

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false; // le JSON doit être un tableau à la racine

    students.clear();
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i) {
        students.append(Student::fromJson(array[i].toObject()));
    }
    currentFilePath = path;
    modified = false;
    return true;
}

// sauvegarde tous les étudiants dans un fichier JSON
bool StudentManager::saveJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonArray array;
    for (const Student &s : students) {
        array.append(s.toJson()); // chaque étudiant se sérialise lui-même
    }
    QJsonDocument doc(array);
    file.write(doc.toJson());
    currentFilePath = path;
    modified = false;
    return true;
}

// --- Fichiers XML ---

// charge la liste depuis un fichier XML (balise racine attendue : <students>)
bool StudentManager::loadXml(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDomDocument doc;
    if (!doc.setContent(&file)) return false; // parsing XML raté

    QDomElement root = doc.documentElement();
    if (root.tagName() != "students") return false; // mauvais format de fichier

    students.clear();
    QDomNodeList list = root.elementsByTagName("student");
    for (int i = 0; i < list.size(); ++i) {
        students.append(Student::fromXml(list.at(i).toElement()));
    }
    currentFilePath = path;
    modified = false;
    return true;
}

// sauvegarde tous les étudiants dans un fichier XML
bool StudentManager::saveXml(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QDomDocument doc;
    // on crée la balise racine <students>
    QDomElement root = doc.createElement("students");
    doc.appendChild(root);

    for (const Student &s : students) {
        root.appendChild(s.toXml(doc)); // chaque étudiant génère son propre nœud XML
    }

    QTextStream stream(&file);
    doc.save(stream, 4); // indentation de 4 espaces pour que ce soit lisible
    currentFilePath = path;
    modified = false;
    return true;
}
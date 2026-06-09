#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <QList>
#include <QString>
#include "student.h"

class StudentManager
{
private:
    QList<Student> students;
    QString currentFilePath;
    bool modified;

public:
    StudentManager();

    // Accès aux données
    QList<Student> getStudents() const;
    Student& getStudent(int index);
    QString getCurrentFilePath() const;
    
    // Suivi des modifications (Alerte sauvegarde)
    bool isModified() const;
    void setModified(bool v);

    // Opérations CRUD
    void addStudent(Student s);
    void removeStudent(int index);
    void updateStudent(int index, Student s);

    // Filtrage par expression régulière (Retourne les indices correspondants)
    QList<int> filter(QString pattern);

    // Gestion Fichiers JSON
    bool loadJson(QString path);
    bool saveJson(QString path);

    // Gestion Fichiers XML
    bool loadXml(QString path);
    bool saveXml(QString path);

    // Utilitaires de gestion de conteneur
    int count() const;
    void clear();
};

#endif // STUDENTMANAGER_H

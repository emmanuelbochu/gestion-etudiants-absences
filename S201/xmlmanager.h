#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QList>
#include <QString>
#include "student.h"

class XmlManager
{
public:
    // Charge tous les étudiants depuis un fichier XML
    static QList<Student> loadFromXml(const QString &filePath);

    // Sauvegarde la liste des étudiants dans un fichier XML
    static bool saveToXml(const QString &filePath, const QList<Student> &students);
};

#endif // XMLMANAGER_H

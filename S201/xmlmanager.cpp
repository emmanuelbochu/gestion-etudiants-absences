#include "xmlmanager.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

// --- SAUVEGARDE EN XML ---
bool XmlManager::saveToXml(const QString &filePath, const QList<Student> &students) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // Impossible d'ouvrir le fichier en écriture
    }

    QDomDocument doc;
    // Balise racine <students>
    QDomElement root = doc.createElement("students");
    doc.appendChild(root);

    // On parcourt chaque étudiant de la liste
    for (int i = 0; i < students.size(); ++i) {
        Student s = students.at(i);

        QDomElement studentElem = doc.createElement("student");
        studentElem.setAttribute("id", s.getStudentID()); // Attribut id

        // Création des balises enfants pour chaque info
        QDomElement lastNameElem = doc.createElement("lastName");
        lastNameElem.appendChild(doc.createTextNode(s.getLastName()));
        studentElem.appendChild(lastNameElem);

        QDomElement firstNameElem = doc.createElement("firstName");
        firstNameElem.appendChild(doc.createTextNode(s.getFirstName()));
        studentElem.appendChild(firstNameElem);

        QDomElement birthElem = doc.createElement("birthDate");
        birthElem.appendChild(doc.createTextNode(s.getBirthDate().toString(Qt::ISODate)));
        studentElem.appendChild(birthElem);

        QDomElement ineElem = doc.createElement("ine");
        ineElem.appendChild(doc.createTextNode(s.getIne()));
        studentElem.appendChild(ineElem);

        QDomElement phoneElem = doc.createElement("phoneNumber");
        phoneElem.appendChild(doc.createTextNode(s.getPhoneNumber()));
        studentElem.appendChild(phoneElem);

        QDomElement schoolEmailElem = doc.createElement("schoolEmail");
        schoolEmailElem.appendChild(doc.createTextNode(s.getSchoolEmail()));
        studentElem.appendChild(schoolEmailElem);

        QDomElement personalEmailElem = doc.createElement("personalEmail");
        personalEmailElem.appendChild(doc.createTextNode(s.getPersonalEmail()));
        studentElem.appendChild(personalEmailElem);

        QDomElement addressElem = doc.createElement("address");
        addressElem.appendChild(doc.createTextNode(s.getAddress()));
        studentElem.appendChild(addressElem);

        QDomElement parentsAddressElem = doc.createElement("parentsAddress");
        parentsAddressElem.appendChild(doc.createTextNode(s.getParentsAddress()));
        studentElem.appendChild(parentsAddressElem);

        QDomElement photoElem = doc.createElement("photoId");
        photoElem.appendChild(doc.createTextNode(s.getPhotoId()));
        studentElem.appendChild(photoElem);

        // Section (on suppose qu'on peut récupérer son nom en QString ou via une conversion)
        QDomElement sectionElem = doc.createElement("section");
        // Remplace par la méthode appropriée de ta classe Section si besoin (ex: s.getSection().getName())
        sectionElem.appendChild(doc.createTextNode(s.getSection().toString()));
        studentElem.appendChild(sectionElem);

        // --- SOUS-FORMULAIRE : ABSENCES ---
        QDomElement absencesElem = doc.createElement("absences");
        QList<Absence> absenceList = s.getAbsences();
        for (int j = 0; j < absenceList.size(); ++j) {
            Absence abs = absenceList.at(j);
            QDomElement absElem = doc.createElement("absence");

            QDomElement absDate = doc.createElement("date");
            absDate.appendChild(doc.createTextNode(abs.getDate().toString(Qt::ISODate)));
            absElem.appendChild(absDate);

            QDomElement absJustified = doc.createElement("justified");
            absJustified.appendChild(doc.createTextNode(abs.isJustified() ? "true" : "false"));
            absElem.appendChild(absJustified);

            QDomElement absReason = doc.createElement("reason");
            absReason.appendChild(doc.createTextNode(abs.getReason()));
            absElem.appendChild(absReason);

            absencesElem.appendChild(absElem);
        }
        studentElem.appendChild(absencesElem);

        // On ajoute l'étudiant à la racine
        root.appendChild(studentElem);
    }

    // Écriture dans le fichier avec une indentation de 4 espaces (propre)
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
    return true;
}

// --- CHARGEMENT DEPUIS XML ---
QList<Student> XmlManager::loadFromXml(const QString &filePath) {
    QList<Student> list;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return list; // Retourne une liste vide si le fichier n'existe pas
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return list; // Erreur de parsing XML
    }
    file.close();

    QDomElement root = doc.documentElement();
    // On récupère tous les nœuds <student>
    QDomNodeList studentNodes = root.elementsByTagName("student");

    for (int i = 0; i < studentNodes.count(); ++i) {
        QDomNode node = studentNodes.at(i);
        if (node.isElement()) {
            QDomElement studentElem = node.toElement();
            Student s;

            // Récupération de l'attribut ID
            s.setStudentID(studentElem.attribute("id"));

            // Récupération des balises textes simples
            s.setLastName(studentElem.firstChildElement("lastName").text());
            s.setFirstName(studentElem.firstChildElement("firstName").text());
            s.setBirthDate(QDate::fromString(studentElem.firstChildElement("birthDate").text(), Qt::ISODate));
            s.setIne(studentElem.firstChildElement("ine").text());
            s.setPhoneNumber(studentElem.firstChildElement("phoneNumber").text());
            s.setSchoolEmail(studentElem.firstChildElement("schoolEmail").text());
            s.setPersonalEmail(studentElem.firstChildElement("personalEmail").text());
            s.setAddress(studentElem.firstChildElement("address").text());
            s.setParentsAddress(studentElem.firstChildElement("parentsAddress").text());
            s.setPhotoId(studentElem.firstChildElement("photoId").text());

            // Section (A adapter selon comment ta classe Section prend une chaîne en paramètre)
            Section sec;
            sec.fromString(studentElem.firstChildElement("section").text());
            s.setSection(sec);

            // Récupération des absences
            QDomElement absencesElem = studentElem.firstChildElement("absences");
            QDomNodeList absNodes = absencesElem.elementsByTagName("absence");
            for (int j = 0; j < absNodes.count(); ++j) {
                QDomNode absNode = absNodes.at(j);
                if (absNode.isElement()) {
                    QDomElement absElem = absNode.toElement();
                    Absence abs;
                    abs.setDate(QDate::fromString(absElem.firstChildElement("date").text(), Qt::ISODate));
                    abs.setJustified(absElem.firstChildElement("justified").text() == "true");
                    abs.setReason(absElem.firstChildElement("reason").text());

                    s.addAbsence(abs);
                }
            }
            list.append(s); // On ajoute l'étudiant complet à la liste
        }
    }
    return list;
}

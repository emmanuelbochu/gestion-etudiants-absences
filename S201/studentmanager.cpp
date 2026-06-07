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
    // Configuration de l'expression régulière en ignorant la casse (CaseInsensitive)
    QList<int> matchingIndices;
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);

    // Parcours de l'ensemble du conteneur pour trouver les correspondances
    for (int i = 0; i < students.size(); ++i) {
        // Le filtrage s'applique sur le texte affiché (Nom, prénom ou Numéro d'étudiant)
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
    // Analyse de la structure syntaxique du document JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false; // La racine doit obligatoirement être un tableau JSON

    students.clear();
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i) {
        students.append(Student::fromJson(array[i].toObject()));
    }

    currentFilePath = path;
    modified = false; // Réinitialisation du flag de modification suite au chargement réussi
    return true;
}

bool StudentManager::saveJson(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false; // Erreur d'ouverture en écriture

    QJsonArray array;
    // Sérialisation de toutes les entités du modèle
    for (const Student &s : students) {
        array.append(s.toJson());
    }

    // Encapsulation et conversion brute en chaîne de texte exploitable
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
    // Extraction de la liste de toutes les balises enfants nommées <student>
    QDomNodeList list = root.elementsByTagName("student");
    for (int i = 0; i < list.size(); ++i) {
        // Reconstruction de l'objet Student d'après le nœud XML courant
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
    doc.save(stream, 4); // Indentation propre de 4 espaces
    currentFilePath = path;
    modified = false;
    return true;
}

void StudentManager::populateWithDefaultTestData() {
    students.clear();

    // 1. Fiche  exemple de Maxence Mine
    Student s1("Mine", "Maxence", "22501432");
    s1.setBirthDate(QDate(2007, 4, 11));
    s1.setIneNumber("1105948869L");
    s1.setPhoneNumber("07-69-51-70-64");
    s1.setAmuEmail("maxence.mine@etu.univ-amu.fr");
    s1.setPersonalEmail("mimaxence1104@gmail.com");
    s1.setPersonalAddress("15 rue Bel horizons,30000 Nimes");
    s1.setParentAddress("15 rue Bel horizons,30000 Nimes");
    s1.setSection(Section::INFO_BUT_1);
    s1.addAbsence(Absence(QDate(2026, 3, 30), true, "Soutenance Flyer Salon"));
    students.append(s1);

    // 2. Fiche exemple de Emmanuel Bochu
    Student s2("Bochu", "Emmanuel", "22509876");
    s2.setBirthDate(QDate(2006, 8, 22));
    s2.setIneNumber("1402847593B");
    s2.setPhoneNumber("0699887766");
    s2.setAmuEmail("lucas.leclerc@etu.univ-amu.fr");
    s2.setPersonalEmail("lucas.leclerc.dev@gmail.com");
    s2.setPersonalAddress("Rue de la République, 13200 Arles");
    s2.setParentAddress("Rue de la République, 13200 Arles");
    s2.setSection(Section::INFO_BUT_1);
    s2.addAbsence(Absence(QDate(2026, 4, 12), false, "Pas de motif fourni"));
    students.append(s2);

    // 3. Fiche 3
    Student s3("Allemand", "Chloé", "22502314");
    s3.setBirthDate(QDate(2005, 11, 3));
    s3.setIneNumber("18274930685");
    s3.setPhoneNumber("06-06-06-06-06");
    s3.setAmuEmail("chloe.allemand@etu.univ-amu.fr");
    s3.setPersonalEmail("chloall@outlook.fr");
    s3.setPersonalAddress("rue Des arenes ,13200 Arles");
    s3.setParentAddress("rue Des arenes ,13200 Arles");
    s3.setSection(Section::INFO_BUT_1);
    students.append(s3);

    // 4. Fiche 4
    Student s4("Roux", "Thomas", "22401122");
    s4.setBirthDate(QDate(2005, 2, 17));
    s4.setIneNumber("0594837261D");
    s4.setPhoneNumber("0712543698");
    s4.setAmuEmail("thomas.roux@etu.univ-amu.fr");
    s4.setPersonalEmail("thomass.roux@hotmail.com");
    s4.setPersonalAddress("Place du Forum, 13200 Arles");
    s4.setParentAddress("Avenue Jean Jaurès, 30000 Nîmes");
    s4.setSection(Section::INFO_BUT_2);
    s4.addAbsence(Absence(QDate(2026, 2, 10), true, "Rendez-vous Médical"));
    students.append(s4);

    // 5. Fiche 5
    Student s5("Martinez", "Sofia", "22407744");
    s5.setBirthDate(QDate(2005, 7, 29));
    s5.setIneNumber("2938475610E");
    s5.setPhoneNumber("0645127893");
    s5.setAmuEmail("sofia.martinez@etu.univ-amu.fr");
    s5.setPersonalEmail("sofia.mtz@gmail.com");
    s5.setPersonalAddress("Boulevard des Lices, 13200 Arles");
    s5.setParentAddress("Boulevard des Lices, 13200 Arles");
    s5.setSection(Section::INFO_BUT_2);
    students.append(s5);

    // 6. Fiche 6
    Student s6("Giraud", "Matthieu", "22305599");
    s6.setBirthDate(QDate(2004, 12, 1));
    s6.setIneNumber("9483726150F");
    s6.setPhoneNumber("0689562314");
    s6.setAmuEmail("matthieu.giraud@etu.univ-amu.fr");
    s6.setPersonalEmail("matt.giraud@orange.fr");
    s6.setPersonalAddress("Rue Gambetta, 13200 Arles");
    s6.setParentAddress("Route d'Avignon, 13150 Tarascon");
    s6.setSection(Section::INFO_BUT_3);
    s6.addAbsence(Absence(QDate(2025, 11, 20), true, "Entretien Stage"));
    students.append(s6);

    // 7. Fiche 7
    Student s7("Morel", "Emma", "22504481");
    s7.setBirthDate(QDate(2006, 3, 10));
    s7.setIneNumber("3847592011G");
    s7.setPhoneNumber("0751624395");
    s7.setAmuEmail("emma.morel@etu.univ-amu.fr");
    s7.setPersonalEmail("emma.mrl.13@gmail.com");
    s7.setPersonalAddress("Avenue de Stalingrad, 13200 Arles");
    s7.setParentAddress("Avenue de Stalingrad, 13200 Arles");
    s7.setSection(Section::MMI_BUT_1);
    students.append(s7);

    // 8. Fiche 8
    Student s8("Dubois", "Nathan", "22503377");
    s8.setBirthDate(QDate(2006, 9, 5));
    s8.setIneNumber("4738291045H");
    s8.setPhoneNumber("0632145698");
    s8.setAmuEmail("nathan.dubois@etu.univ-amu.fr");
    s8.setPersonalEmail("nathan.dubois.mmi@outlook.com");
    s8.setPersonalAddress("Chemin de Barriol, 13200 Arles");
    s8.setParentAddress("Rue de la Couronne, 13100 Aix-en-Provence");
    s8.setSection(Section::MMI_BUT_1);
    s8.addAbsence(Absence(QDate(2026, 5, 02), false, "Train annulé"));
    students.append(s8);

    // 9. Fiche 9
    Student s9("Masson", "Laura", "22408811");
    s9.setBirthDate(QDate(2005, 4, 14));
    s9.setIneNumber("5849302174I");
    s9.setPhoneNumber("0611223344");
    s9.setAmuEmail("laura.masson@etu.univ-amu.fr");
    s9.setPersonalEmail("lau.masson@gmail.com");
    s9.setPersonalAddress("Quartier Trinquetaille, 13200 Arles");
    s9.setParentAddress("Avenue de la Toison d'Or, 30000 Nîmes");
    s9.setSection(Section::MMI_BUT_2);
    students.append(s9);

    // 10. Fiche 10
    Student s10("Vincent", "Hugo", "22306622");
    s10.setBirthDate(QDate(2004, 6, 18));
    s10.setIneNumber("6948302157J");
    s10.setPhoneNumber("0788552211");
    s10.setAmuEmail("hugo.vincent@etu.univ-amu.fr");
    s10.setPersonalEmail("hugo.vincent.mmi@gmail.com");
    s10.setPersonalAddress("Rue Voltaire, 13200 Arles");
    s10.setParentAddress("Route de Beaucaire, 30000 Nîmes");
    s10.setSection(Section::MMI_BUT_3);
    students.append(s10);
}
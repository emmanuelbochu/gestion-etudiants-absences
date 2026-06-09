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

void StudentManager::populateWithDefaultTestData() {
    students.clear();

    // 1. Fiche exemple de Maxence Mine
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

    // 11. Fiche 11
    Student s11("Lemoine", "Arthur", "22501111");
    s11.setBirthDate(QDate(2006, 1, 15));
    s11.setIneNumber("7059483829K");
    s11.setPhoneNumber("0612345678");
    s11.setAmuEmail("arthur.lemoine@etu.univ-amu.fr");
    s11.setPersonalEmail("arthur.lmn@gmail.com");
    s11.setPersonalAddress("Rue des Carmes, 13200 Arles");
    s11.setParentAddress("Avenue Victor Hugo, 84000 Avignon");
    s11.setSection(Section::INFO_BUT_1);
    students.append(s11);

    // 12. Fiche 12
    Student s12("Blanc", "Julie", "22402222");
    s12.setBirthDate(QDate(2005, 5, 20));
    s12.setIneNumber("8160594930L");
    s12.setPhoneNumber("0798765432");
    s12.setAmuEmail("julie.blanc@etu.univ-amu.fr");
    s12.setPersonalEmail("jblanc05@hotmail.fr");
    s12.setPersonalAddress("Boulevard Emile Combes, 13200 Arles");
    s12.setParentAddress("Boulevard Emile Combes, 13200 Arles");
    s12.setSection(Section::INFO_BUT_2);
    s12.addAbsence(Absence(QDate(2025, 10, 15), true, "Problème de transport"));
    students.append(s12);

    // 13. Fiche 13
    Student s13("Perrin", "Lucas", "22303333");
    s13.setBirthDate(QDate(2004, 8, 30));
    s13.setIneNumber("9271605041M");
    s13.setPhoneNumber("0645678912");
    s13.setAmuEmail("lucas.perrin@etu.univ-amu.fr");
    s13.setPersonalEmail("l.perrin.dev@gmail.com");
    s13.setPersonalAddress("Chemin des Moines, 13200 Arles");
    s13.setParentAddress("Route de Nîmes, 30129 Manduel");
    s13.setSection(Section::INFO_BUT_3);
    students.append(s13);

    // 14. Fiche 14
    Student s14("Rousseau", "Camille", "22504444");
    s14.setBirthDate(QDate(2006, 12, 12));
    s14.setIneNumber("0382716152N");
    s14.setPhoneNumber("0756473829");
    s14.setAmuEmail("camille.rousseau@etu.univ-amu.fr");
    s14.setPersonalEmail("cam.rousseau@outlook.com");
    s14.setPersonalAddress("Rue de la Roquette, 13200 Arles");
    s14.setParentAddress("Rue de la Roquette, 13200 Arles");
    s14.setSection(Section::MMI_BUT_1);
    students.append(s14);

    // 15. Fiche 15
    Student s15("Richard", "Nicolas", "22405555");
    s15.setBirthDate(QDate(2005, 2, 28));
    s15.setIneNumber("1493827263O");
    s15.setPhoneNumber("0687654321");
    s15.setAmuEmail("nicolas.richard@etu.univ-amu.fr");
    s15.setPersonalEmail("nico.richard.mmi@gmail.com");
    s15.setPersonalAddress("Place de la République, 13200 Arles");
    s15.setParentAddress("Avenue Georges Pompidou, 30900 Nîmes");
    s15.setSection(Section::MMI_BUT_2);
    s15.addAbsence(Absence(QDate(2026, 1, 10), false, ""));
    students.append(s15);

    // 16. Fiche 16
    Student s16("Guerin", "Manon", "22306666");
    s16.setBirthDate(QDate(2004, 10, 5));
    s16.setIneNumber("2504938374P");
    s16.setPhoneNumber("0711223344");
    s16.setAmuEmail("manon.guerin@etu.univ-amu.fr");
    s16.setPersonalEmail("manon.guerin.pro@yahoo.fr");
    s16.setPersonalAddress("Quartier de l'Hauture, 13200 Arles");
    s16.setParentAddress("Rue Thiers, 84000 Avignon");
    s16.setSection(Section::MMI_BUT_3);
    students.append(s16);

    // 17. Fiche 17
    Student s17("Marchand", "Leo", "22507777");
    s17.setBirthDate(QDate(2007, 3, 22));
    s17.setIneNumber("3615049485Q");
    s17.setPhoneNumber("0634567890");
    s17.setAmuEmail("leo.marchand@etu.univ-amu.fr");
    s17.setPersonalEmail("leo.mchd@gmail.com");
    s17.setPersonalAddress("Rue Tour de Fabre, 13200 Arles");
    s17.setParentAddress("Rue Tour de Fabre, 13200 Arles");
    s17.setSection(Section::INFO_BUT_1);
    students.append(s17);

    // 18. Fiche 18
    Student s18("Clement", "Clara", "22408888");
    s18.setBirthDate(QDate(2005, 9, 14));
    s18.setIneNumber("4726150596R");
    s18.setPhoneNumber("0765432109");
    s18.setAmuEmail("clara.clement@etu.univ-amu.fr");
    s18.setPersonalEmail("clara.clmt@hotmail.com");
    s18.setPersonalAddress("Avenue des Alyscamps, 13200 Arles");
    s18.setParentAddress("Boulevard de Prague, 30000 Nîmes");
    s18.setSection(Section::INFO_BUT_2);
    students.append(s18);

    // 19. Fiche 19
    Student s19("Colin", "Antoine", "22509999");
    s19.setBirthDate(QDate(2006, 7, 7));
    s19.setIneNumber("5837261607S");
    s19.setPhoneNumber("0609876543");
    s19.setAmuEmail("antoine.colin@etu.univ-amu.fr");
    s19.setPersonalEmail("antoine.c.dev@gmail.com");
    s19.setPersonalAddress("Chemin de la Plaine, 13200 Arles");
    s19.setParentAddress("Chemin de la Plaine, 13200 Arles");
    s19.setSection(Section::MMI_BUT_1);
    s19.addAbsence(Absence(QDate(2026, 4, 18), true, "Convocation administrative"));
    students.append(s19);

    // 20. Fiche 20
    Student s20("Vidal", "Juliette", "22300000");
    s20.setBirthDate(QDate(2004, 11, 25));
    s20.setIneNumber("6948372718T");
    s20.setPhoneNumber("0787654321");
    s20.setAmuEmail("juliette.vidal@etu.univ-amu.fr");
    s20.setPersonalEmail("ju.vidal@outlook.fr");
    s20.setPersonalAddress("Rue Condorcet, 13200 Arles");
    s20.setParentAddress("Route de Marseille, 13080 Aix-en-Provence");
    s20.setSection(Section::MMI_BUT_3);
    students.append(s20);
}
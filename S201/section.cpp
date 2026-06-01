#include "section.h"

// Convertit l'enum en texte lisible pour le XML et l'IHM
QString sectionToString(Section section) {
    switch (section) {
    case Section::INFO_BUT_1: return "Info BUT 1";
    case Section::INFO_BUT_2: return "Info BUT 2";
    case Section::INFO_BUT_3: return "Info BUT 3";
    case Section::MMI_BUT_1:  return "MMI BUT 1";
    case Section::MMI_BUT_2:  return "MMI BUT 2";
    case Section::MMI_BUT_3:  return "MMI BUT 3";
    }
    return "Info BUT 1"; // Valeur par défaut au cas où
}

// Convertit le texte du XML ou de la ComboBox en enum Section
Section sectionFromString(const QString &str) {
    if (str == "Info BUT 1") return Section::INFO_BUT_1;
    if (str == "Info BUT 2") return Section::INFO_BUT_2;
    if (str == "Info BUT 3") return Section::INFO_BUT_3;
    if (str == "MMI BUT 1")  return Section::MMI_BUT_1;
    if (str == "MMI BUT 2")  return Section::MMI_BUT_2;
    if (str == "MMI BUT 3")  return Section::MMI_BUT_3;
    return Section::INFO_BUT_1; // Valeur par défaut au cas où
}

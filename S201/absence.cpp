#include "absence.h"

// Constructeur par défaut
Absence::Absence() {
    date = QDate::currentDate();
    justified = false;
    reason = "";
}

// Constructeur surchargé
Absence::Absence(QDate date, bool justified, QString reason) {
    this->date = date;
    this->justified = justified;
    this->reason = reason;
}

// --- Getters et Setters ---
QDate Absence::getDate() const { return date; }
bool Absence::isJustified() const { return justified; }
QString Absence::getReason() const { return reason; }

void Absence::setDate(QDate date) { this->date = date; }
void Absence::setJustified(bool justified) { this->justified = justified; }
void Absence::setReason(QString reason) { this->reason = reason; }

// --- Sérialisation JSON ---
QJsonObject Absence::toJson() const {
    QJsonObject obj;
    obj["date"] = date.toString(Qt::ISODate); // Format standard YYYY-MM-DD
    obj["justified"] = justified;
    obj["reason"] = reason;
    return obj;
}

Absence Absence::fromJson(const QJsonObject &obj) {
    Absence a;
    if (obj.contains("date")) {
        a.setDate(QDate::fromString(obj["date"].toString(), Qt::ISODate));
    }
    if (obj.contains("justified")) {
        a.setJustified(obj["justified"].toBool());
    }
    if (obj.contains("reason")) {
        a.setReason(obj["reason"].toString());
    }
    return a;
}

// --- Sérialisation XML ---
QDomElement Absence::toXml(QDomDocument &doc) const {
    // Crée la balise principale <absence>
    QDomElement elem = doc.createElement("absence");
    elem.setAttribute("date", date.toString(Qt::ISODate));
    elem.setAttribute("justified", justified ? "true" : "false");

    // Ajoute le motif comme sous-élément de texte
    QDomElement reasonElem = doc.createElement("reason");
    reasonElem.appendChild(doc.createTextNode(reason));
    elem.appendChild(reasonElem);

    return elem;
}

Absence Absence::fromXml(const QDomElement &elem) {
    Absence a;
    if (elem.hasAttribute("date")) {
        a.setDate(QDate::fromString(elem.attribute("date"), Qt::ISODate));
    }
    if (elem.hasAttribute("justified")) {
        a.setJustified(elem.attribute("justified") == "true");
    }
    QDomElement reasonElem = elem.firstChildElement("reason");
    if (!reasonElem.isNull()) {
        a.setReason(reasonElem.text());
    }
    return a;
}
#ifndef ABSENCE_H
#define ABSENCE_H

#include <QDate>
#include <QString>
#include <QJsonObject>
#include <QDomDocument>
#include <QDomElement>

class Absence
{
private:
    QDate date;         // Date de l'absence
    bool justified;     // Statut de justification (true = justifiée)
    QString reason;     // Motif de l'absence

public:
    // Constructeurs
    Absence();
    Absence(QDate date, bool justified, QString reason);

    // Getters
    QDate getDate() const;
    bool isJustified() const;
    QString getReason() const;

    // Setters
    void setDate(QDate date);
    void setJustified(bool justified);
    void setReason(QString reason);

    // Sérialisation JSON et XML (Exigences de la Partie 1)
    QJsonObject toJson() const;
    QDomElement toXml(QDomDocument &doc) const;

    // Désérialisation (Méthodes statiques retournant un objet Absence)
    static Absence fromJson(const QJsonObject &obj);
    static Absence fromXml(const QDomElement &elem);
};

#endif // ABSENCE_H
#ifndef ABSENCE_H
#define ABSENCE_H

#include <QDate>
#include <QString>

class Absence
{
private:
    QDate date;
    bool isJustified;
    QString reason;

public:
    // Constructeur et Destructeur
    Absence(const QDate &date, bool isJustified, const QString &reason);
    ~Absence();

    // Getters
    QDate getDate() const;
    bool getIsJustified() const;
    QString getReason() const;

    // Setters
    void setDate(const QDate &date);
    void setIsJustified(bool isJustified);
    void setReason(const QString &reason);
};

#endif // ABSENCE_H
#ifndef ABSENCE_H
#define ABSENCE_H

class Absence
{
private :
    QDate absDate;
    bool isJustified;
    QString reason;
public:
    Absence();
    Absence(const QDate &date, bool justified, const QString &reason);

    // Getters
    QDate getDate() const;
    bool isJustified() const;
    QString getReason() const;

    // Setters
    void setDate(const QDate &value);
    void setJustified(bool value);
    void setReason(const QString &value);
};

#endif // ABSENCE_H

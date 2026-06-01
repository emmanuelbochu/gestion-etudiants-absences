#include "absence.h"

Absence::Absence() {
    date = QDate::currentDate(); // Par défaut, date du jour
    justified = false;
    reason = "";
}

Absence::Absence(const QDate &d, bool j, const QString &r) {
    date = d;
    justified = j;
    reason = r;
}

QDate Absence::getDate() const { return date; }
bool Absence::isJustified() const { return justified; }
QString Absence::getReason() const { return reason; }

void Absence::setDate(const QDate &value) { date = value; }
void Absence::setJustified(bool value) { justified = value; }
void Absence::setReason(const QString &value) { reason = value; }

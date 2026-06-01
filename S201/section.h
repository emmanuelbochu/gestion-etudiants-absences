#ifndef SECTION_H
#define SECTION_H

#include <QString>

enum class Section
{
    Info_BUT_1,
    Info_BUT_2,
    Info_BUT_3,
    MMI_BUT_1,
    MMI_BUT_2,
    MMI_BUT_3
};

QString sectionToString(Section section);
Section sectionFromString(const QString &str);

#endif // SECTION_H
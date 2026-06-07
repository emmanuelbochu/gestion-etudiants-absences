#ifndef SECTION_H
#define SECTION_H

#include <QString>

enum class Section
{
    INFO_BUT_1,
    INFO_BUT_2,
    INFO_BUT_3,
    MMI_BUT_1,
    MMI_BUT_2,
    MMI_BUT_3
};

QString sectionToString(Section section);
Section sectionFromString(const QString &str);

#endif // SECTION_H
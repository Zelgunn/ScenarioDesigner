#ifndef SCENARIO_H
#define SCENARIO_H

#include <QString>
#include <QDomDocument>
#include <QDebug>

class Scenario
{
public:
    Scenario();
    void setName(QString m_name);
    QString getName();
    void setSaved(bool m_saved);
    bool isSaved();

private:
    QString name;
    QString story;
    bool saved;
};

#endif // SCENARIO_H

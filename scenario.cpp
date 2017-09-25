#include "scenario.h"

Scenario::Scenario()
{
    saved = true;
}

void Scenario::setName(QString m_name)
{
    this->name = m_name;
}

QString Scenario::getName()
{
    return this->name;
}

void Scenario::setSaved(bool m_saved)
{
    this->saved = m_saved;
}

bool Scenario::isSaved()
{
    return this->saved;
}

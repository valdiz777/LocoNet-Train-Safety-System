#include "enginepath.h"

EnginePath::EnginePath()
{

}

EnginePath::~EnginePath()
{

}

EnginePath::EnginePath(Section parent, Section previous, Section current, Section future)
{
    parentSection = parent;
    previousSection = previous;
    currentSection = current;
    futureSection = future;
}

Section EnginePath::getParentSection() const
{
    return parentSection;
}

void EnginePath::setParentSection(const Section &value)
{
    parentSection = value;
}

Section EnginePath::getPreviousSection() const
{
    return previousSection;
}

void EnginePath::setPreviousSection(const Section &value)
{
    previousSection = value;
}

Section EnginePath::getCurrentSection() const
{
    return currentSection;
}

void EnginePath::setCurrentSection(const Section &value)
{
    currentSection = value;
}

Section EnginePath::getFutureSection() const
{
    return futureSection;
}

void EnginePath::setFutureSection(const Section &value)
{
    futureSection = value;
}

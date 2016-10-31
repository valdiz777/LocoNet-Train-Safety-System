#include <QString>
#include "section.h"

#ifndef ENGINEPATH_H
#define ENGINEPATH_H


class EnginePath
{
public:
	EnginePath();
	EnginePath(Section parent, Section previous, Section current, Section future);
	~EnginePath();

	Section getParentSection() const;
	void setParentSection(const Section &value);

	Section getPreviousSection() const;
	void setPreviousSection(const Section &value);

	Section getCurrentSection() const;
	void setCurrentSection(const Section &value);

	Section getFutureSection() const;
	void setFutureSection(const Section &value);

private:
	Section parentSection;
	Section previousSection;
	Section currentSection;
	Section futureSection;
};

#endif // ENGINEPATH_H

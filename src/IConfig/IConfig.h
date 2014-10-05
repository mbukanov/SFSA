#ifndef	ICONFIG_H
#define	ICONFIG_H

#include <set>
#include "iniparser.h"


class IConfig
{
protected:
	//std::map<std::string, std::string> settings;
	std::map<std::string, std::map<std::string, std::string> > settings;

	static IConfig* _self;
    IConfig();
    virtual ~IConfig();

public:
    static IConfig* Instance();

    std::string get(std::string section, std::string entry);

    static bool DeleteInstance();
};

#endif
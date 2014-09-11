#ifndef	CONFIG_H
#define	CONFIG_H

#include <set>
#include "iniparser.h"

class Config
{
protected:
	//std::map<std::string, std::string> settings;
	std::map<std::string, std::map<std::string, std::string> > settings;

	static Config* _self;
	Config(){}
	virtual ~Config(){}

public:
	static Config* Instance()
	{
		if(!_self)
		{
			_self = new Config();
			IniParser parser((char*)"config.ini");
			_self->settings = parser.getAll();
		}
		return _self;
	}

	std::string get(std::string section, std::string entry)
	{
		return _self->settings[section][entry];
	}

	static bool DeleteInstance()
	{
		if(_self)
		{
			delete _self;
			_self = 0;
			return true;
		}
		return false;
	}
};

Config* Config::_self = 0;


#endif
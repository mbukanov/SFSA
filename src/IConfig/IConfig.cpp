#include "IConfig.h"

IConfig::IConfig()
{}

IConfig::~IConfig()
{
    IConfig::DeleteInstance();
}

IConfig* IConfig::Instance()
{
    if(!_self)
    {
        _self = new IConfig();
        IniParser parser((char*)"config.ini");
        _self->settings = parser.getAll();
    }
    return _self;
}

std::string IConfig::get(std::string section, std::string entry)
{
    return _self->settings[section][entry];
}

bool IConfig::DeleteInstance()
{
    if(_self)
    {
        delete _self;
        _self = 0;
        return true;
    }
    return false;
}

IConfig* IConfig::_self = 0;
#include "iniparser.h"

IniParser::IniParser()
{
}

IniParser::IniParser(char * filename_)
{
    this->setIniFile(filename_);
}

// debug
void IniParser::show()
{
    for(Inidata::iterator its = inidata.begin(); its != inidata.end(); its++ )
    {
        // it - section
        std::cout<<"Section: "<<its->first<<std::endl;

        for(Entries::iterator ite = its->second.begin(); ite != its->second.end(); ite++)
        {
            // ite - entry
            std::cout<<"Entry: "<<ite->first<<" |=| "<<ite->second<<std::endl;
        }

    }
}

std::map<std::string, std::map<std::string, std::string> > IniParser::getAll()
{
    std::map<std::string, std::map<std::string, std::string> > values;

    for(Inidata::iterator its = inidata.begin(); its != inidata.end(); its++ )
    {
        // its - section
        for(Entries::iterator ite = its->second.begin(); ite != its->second.end(); ite++)
        {
            // ite - entry
            std::string section = (its->first).substr(1, (its->first).size()-2);
            values[section][ite->first] = ite->second;
        }
    }

    return values;

}

void IniParser::setIniFile(char *filename_)
{
    this->filename = (const char*)filename_;
    std::ifstream file;
    file.open(filename_, std::ios_base::in);

    if(!file.is_open())
    {
        std::cout<<"INIPARSER: File not open"<<std::endl;
        exit(0);
    }


    Section section;

    std::vector<std::string> v;

    int max_len = 4095;
    char buff[max_len];
    while(file.getline(buff, max_len))
    {
        std::string str(buff);
        v.push_back(str);
    }

    std::vector<std::string>::iterator i;
    for(i = v.begin(); i != v.end(); ++i)
    {
        if((*i).empty())    continue;
        if((*i)[0] == '#') continue;

        if((*i)[0] == '[')
        {
            if(!section.first.empty())
            {
                inidata.push_back(section);
                section.first.clear();
                section.second.clear();
            }
            section.first = *i;
        }else{
            Entry entry(this->str_split('=', (*i)));
            section.second.push_back(entry);
        }
    }

    inidata.push_back(section);

    file.close();
}

std::string IniParser::convertToIniString()
{
    std::string result = "";
    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        result += "["+it_section->first+"]\n";
        for(Entries::iterator it_entry = it_section->second.begin(); it_entry != it_section->second.end(); it_entry++)
        {
            result += it_entry->first+"="+it_entry->second+"\n";
        }
    }
    return result;
}

bool IniParser::saveToCurrentFile()
{
    saveToOtherFile(this->filename);
}

bool IniParser::saveToOtherFile(std::string filename)
{
    std::ofstream file;
    file.open(filename.data());
    std::string data = convertToIniString();
    file << data;
    file.close();
}

std::string IniParser::getValFromArg(std::string section_, std::string arg_)
{
    if(section_.empty() || arg_.empty())
        return "";

    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); ++it_section)
    {
        if(it_section->first == ( '[' + section_ + ']'))
        {
            for(Entries::iterator it_entry = it_section->second.begin(); it_entry != it_section->second.end(); it_entry++)
            {
                if(it_entry->first == arg_)
                {
                    return it_entry->second;
                }
            }
        }
    }
    return "";

}

std::string IniParser::getArgFromVal(std::string section_, std::string val_)
{

    if(section_.empty() || val_.empty())
        return "";

    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        if(it_section->first == section_)
        {
            for(Entries::iterator it_entry = it_section->second.begin(); it_entry != it_section->second.end(); it_entry++)
            {
                if(it_entry->second == val_)
                {
                    return it_entry->first;
                }
            }
        }
    }
    return "";

}

std::pair<std::string, std::string> IniParser::str_split(char pattern_, std::string str_)
{
    //std::vector<std::string> result;
    std::pair<std::string, std::string> result;
    std::string res1 = "";
    bool flag = false;

    for(int i = 0; i < str_.length(); i++)
    {
        if(str_[i] == pattern_ && !res1.empty() && flag == false)
        {
            result.first = res1;
            res1.clear();
            flag = true;
        }else{
            res1 += str_[i];
        }
    }
    if(res1.empty())
        res1 = "";

    result.second = res1;
    return result;
}

std::string IniParser::str_remlrchars(std::string str)
{
    std::string result;
    if(str.empty() || str.length() < 3)
        return "";
    for(int i = 1; i < str.length()-1; i++)
        result.push_back(str[i]);


    return result;
}

bool IniParser::changeValue(std::string section_, std::string arg_, std::string new_val_)
{
    if(section_.empty() || arg_.empty())
        return false;

    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        if(it_section->first == section_)
        {
            for(Entries::iterator it_entry = it_section->second.begin(); it_entry != it_section->second.end(); it_entry++)
            {
                if(it_entry->first == arg_)
                {
                    it_entry->second = new_val_;
                    saveToCurrentFile();
                    return true;
                }
            }
        }
    }
    return false;
}

bool IniParser::addSection(std::string sectionName)
{
    if(sectionName.empty())
        return false;

    std::string result = convertToIniString();
    result += "["+sectionName+"]\n";
    if(saveToCurrentFile())
        return true;
    return false;
}

bool IniParser::delSection(std::string sectionName)
{
    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        if(it_section->first == sectionName)
        {
            inidata.remove(*it_section);
            if(saveToCurrentFile())
                return true;
        }
    }
    return false;
}

bool IniParser::addArg(std::string section_, std::string arg_, std::string val_)
{
    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        if(it_section->first == section_)
        {
            Entry temp_entry;
            temp_entry.first = arg_;
            temp_entry.second = val_;
            it_section->second.push_back(temp_entry);
            if(saveToCurrentFile())
                return true;
        }
    }
    return false;
}

bool IniParser::delArg(std::string section_, std::string arg_)
{
    for(Inidata::iterator it_section = inidata.begin(); it_section != inidata.end(); it_section++)
    {
        if(it_section->first == section_)
        {
            for(Entries::iterator it_entry = it_section->second.begin(); it_entry != it_section->second.end(); it_entry++)
            {
                if(it_entry->first == arg_)
                {
                    it_section->second.remove(*it_entry);
                    if(saveToCurrentFile())
                        return true;
                }
            }
        }
    }
    return false;
}














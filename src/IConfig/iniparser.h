#ifndef INIPARSER_H
#define INIPARSER_H

/*
    qeed
*/

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <iterator>

    typedef std::pair<std::string, std::string> Entry;
    typedef std::list<Entry> Entries;
    typedef std::pair<std::string, Entries> Section;
    typedef std::list<Section> Inidata;

    class IniParser
    {
    public:
        IniParser();
        IniParser(char *filename_);
        void setIniFile(char *filename_);
        void show();
        std::map<std::string, std::map<std::string, std::string> > getAll();
        std::string getValFromArg(std::string section_, std::string arg_);
        std::string getArgFromVal(std::string section_, std::string val_);
        bool changeValue(std::string section_, std::string arg_, std::string new_val_);
        bool addSection(std::string sectionName);
        bool delSection(std::string sectionName);
        bool addArg(std::string section_, std::string arg_, std::string val_);
        bool delArg(std::string section_, std::string arg_);

    private:
        std::string convertToIniString();
        bool saveToCurrentFile();
        bool saveToOtherFile(std::string filename);

        std::pair<std::string, std::string> str_split(char pattern_, std::string str_);
        std::string str_remlrchars(std::string str);
        std::string filename;
        Inidata inidata;
    };
#endif // INIPARSER_H

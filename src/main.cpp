#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <iostream>
#include <time.h>
#include <map>
#include <utility>
#include <complex>
#include <tuple>
#include <algorithm>

#include "notification/notifier.h"
#include "IConfig/IConfig.h"
#include "mols.h"

void do_ls(char*);
void dostat(char*,char*);

std::map<int, double> file_list;
unsigned long size;

int main(int argc,char *argv[])
{
    size = 0;
    struct timeval start, end;
    long mtime, secs, usecs;    
    gettimeofday(&start, NULL);

    char path[256];
    strcpy(path, IConfig::Instance()->get("global", "path").c_str());
    do_ls(path);

    double tmp = 0;
    for(std::map<int, double>::iterator it = file_list.begin(); it != file_list.end(); it++)
    {
        it->second += tmp;
        tmp = it->second;
    }

    for(std::map<int, double>::iterator it = file_list.begin(); it != file_list.end(); it++)
    {
        std::cout<<it->first<<";"<<it->second<<std::endl;
    }

    std::map<int, double>::iterator i;
    std::vector<double> xv;
    std::vector<double> yv;
    for(i = file_list.begin(); i != file_list.end(); i++)
    {
        xv.push_back((double)(i->first));
        yv.push_back(i->second);
    }

    /* Notification Type */
    MOLS *mols = new MOLS(xv, yv);
    Notifier *notifier = NULL;
    std::string type = IConfig::Instance()->get("alert", "type");
//    std::string type = config.getValFromArg("alert", "type");
    if(type == "email")
        notifier = new Notifier(new NotificationEmail());
    // if(type == ...) notifier = new ...
    else
        notifier = new Notifier(new NotificationNone());

    mols->defW();

    Matrix<double> A;
    Matrix<double> w;
    Matrix<double> b;

    A = mols->getX();
    w = mols->getW();
    mols->defY();

    Matrix<double> Y = mols->getY();
    Matrix<double> X = mols->getX();
 
    int last_i = Y.getSize().rows-1;
    int last_j = 2;
    std::cout<<"OK: "<<IConfig::Instance()->get("global", "size_limit")<<std::endl;

    double sizeLimit = std::stod(IConfig::Instance()->get("global", "size_limit"));
    double freeSpaceEnd = mols->defTimeLimit(sizeLimit) - X.getElement(last_i, last_j);
    
    std::cout<<"FREE SPACE LIMIT: "<<sizeLimit<<std::endl;
    std::cout<<"Free space ends at: "<<freeSpaceEnd<<" days."<<std::endl;

    int alert = stoi(IConfig::Instance()->get("global", "alert"));
//    int alert = stoi(config.getValFromArg("global", "alert"));
    if(alert)
    {
        double size_limit_alert = std::stod(IConfig::Instance()->get("global", "size_limit_alert"));
//        double size_limit_alert = stod(config.getValFromArg("global", "size_limit_alert"));
        if(size_limit_alert >= freeSpaceEnd)
            notifier->Alert();
    }


    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    printf("Elapsed time: %ld millisecs\n", mtime);
    return 0;
}


void do_ls(char *dirname)
{
    DIR *dir_ptr;
    dirent *direntp;
    if((dir_ptr=opendir(dirname))==NULL)
    {
        fprintf(stderr,"myls:cannot open %s\n",dirname);
    }else{
        while((direntp=readdir(dir_ptr))!=NULL){
            if(*(direntp->d_name) != '.' ){
                if(direntp->d_type == DT_DIR)
                {
                    char *newdir = new char[256];
                    memset(newdir, 0, sizeof(newdir));
                    strcpy(newdir, dirname);
                    if(newdir[strlen(dirname)-1] != '/') strcat(newdir, "/");
                    strcat(newdir, direntp->d_name);
                    do_ls(newdir);
                    delete[] newdir;
                }else
                if(direntp->d_type != DT_LNK)
                    dostat(direntp->d_name,dirname);
                //printf("%8d ",(int)direntp->d_ino);
            }
        }
    closedir(dir_ptr);
    }
}

void dostat(char *filename,char *dirname)
{
    struct stat info;
    char* way_to_file=new char [strlen(filename)+strlen(dirname)+2];
    strcpy(way_to_file,dirname);
    strcpy(way_to_file+strlen(dirname),"/\0");
    strcpy(way_to_file+strlen(way_to_file),filename);

    if(stat(way_to_file,&info)==-1){
        printf("Error %s\n",way_to_file);
        perror(filename);
    }else{
        tm* gmtm = gmtime(&(info.st_mtime));
        if(gmtm->tm_yday != 0)
        {
            file_list[gmtm->tm_yday] += (double)info.st_size/1024/1024; // MBytes
        }
    }
    //show_file_info(filename,&info);
    delete way_to_file;
}
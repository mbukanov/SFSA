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

#include "config.h"
#include "mols.h"

void do_ls(char*);
void dostat(char*,char*);

std::map<int, double> file_list;

int main(int argc,char *argv[])
{
    struct timeval start, end;
    long mtime, secs, usecs;    
    gettimeofday(&start, NULL);

    char path[256];
    strcpy(path, Config::Instance()->get("global", "path").c_str());
//    strcpy(path, config.getValFromArg("global", "path").c_str());
    do_ls(path);
    //do_ls((char*)"/home/qeed/Video");
    //do_ls((char*)"/backup/");

    std::map<int, double>::iterator i;
    std::vector<double> xv;
    std::vector<double> yv;
    double sum = 0;
    for(i = file_list.begin(); i != file_list.end(); i++)
    {
        //std::cout<<"day: "<<i->first<<" | bytes: "<<i->second<<std::endl;
        if(i->second <= 10) continue;
        xv.push_back((double)(i->first));
        yv.push_back(i->second + sum);
        sum += i->second;
    }

    /* Notification Type */
    BaseMOLS* mols;
    std::string type = Config::Instance()->get("alert", "type");
//    std::string type = config.getValFromArg("alert", "type");
    if(type == "email")
        mols = new MOLS<NotificationEmail>(xv, yv);
    // if(type == ...) mols = new ...
    else
        mols = new MOLS<NotificationNone>(xv, yv);

    mols->defW();

    Matrix<double> A;
    Matrix<double> w;
    Matrix<double> b;

    A = mols->getX();
    w = mols->getW();
    mols->defY();

    Matrix<double> Y = mols->getY();
    Matrix<double> X = mols->getX();
 
    // output
/*

    int rows = Y.getSize().rows;
    Elements<double> elY = Y.getElements();
    Elements<double> elX = X.getElements();

    std::vector< std::pair<double, double>* > values;
    std::vector<int> skipVector;
    int jj;
    for(jj = 1; jj <= X.getSize().rows; jj++)
    {
        if(X.getElement(jj, 2) == 0)
        {
            skipVector.push_back(jj-1);
            continue;
        }
        values.push_back(new std::pair<double, double>(X.getElement(jj, 2), 0));
    }


    Elements<double>::iterator it;
    int ki = 0;
    for(it = elY.begin(); it != elY.end(); it++)
    {
        if(std::find(skipVector.begin(), skipVector.end(), ki) != skipVector.end())
            continue;

        values[ki]->second = (*it)->getValue();
        ki++;
    }

    std::vector<std::pair<double, double> * >::iterator iter;
    for(iter = values.begin(); iter != values.end(); iter++)
    {
        std::cout<<(*iter)->first<<"; "<<(*iter)->second<<std::endl;
    }

    */

    int last_i = Y.getSize().rows-1;
    int last_j = 2;
    std::cout<<"OK: "<<Config::Instance()->get("global", "size_limit")<<std::endl;

    double sizeLimit = std::stod(Config::Instance()->get("global", "size_limit"));
//    double sizeLimit = stod(config.getValFromArg("global", "size_limit")); // mb
    double freeSpaceEnd = mols->defTimeLimit(sizeLimit) - X.getElement(last_i, last_j);
    std::cout<<"FREE SPACE LIMIT: "<<sizeLimit<<std::endl;
    std::cout<<"Free space ends at: "<<freeSpaceEnd<<" days."<<std::endl;

    int alert = stoi(Config::Instance()->get("global", "alert"));
//    int alert = stoi(config.getValFromArg("global", "alert"));
    if(alert)
    {
        double size_limit_alert = std::stod(Config::Instance()->get("global", "size_limit_alert"));
//        double size_limit_alert = stod(config.getValFromArg("global", "size_limit_alert"));
        if(size_limit_alert >= freeSpaceEnd)
            mols->Alert();
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
            file_list[gmtm->tm_yday] += (double)info.st_size/1024/1024; // MBytes
    }
    //show_file_info(filename,&info);
    delete way_to_file;
}
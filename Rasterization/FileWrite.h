#ifndef FILEWRITE_H
#define FILEWRITE_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "Vector.h"

class FileWriter
{
private:
    /* data */
    std::string fileName;
    std::string dir;
    std::ofstream out;

public:
    FileWriter(std::string fileName, std::string dir = "./");
    FileWriter();
    void changeFile(std::string fileName, std::string dir = "./");
    bool openFile();
    bool writePointToFile(Point A);
    bool writeTextToFile(std::string s);
    void closeFile();
    bool writeDoubleToFile(double x);
    ~FileWriter();
};

FileWriter::FileWriter(std::string fileName, std::string dir)
{
    this->fileName = fileName;
    this->dir = dir;
}

FileWriter::FileWriter()
{
    this->fileName = "";
    this->dir = "./";
}

void FileWriter::changeFile(std::string fileName, std::string dir)
{
    this->fileName = fileName;
    this->dir = dir;
}

bool FileWriter::openFile()
{
    std::string filepath = dir + fileName;
    out.open(filepath);

    return out.is_open();
}

bool FileWriter::writePointToFile(Point A)
{
    if (out.is_open())
    {
        out << std::fixed <<std::setprecision(7)<<A.x<<" "<<A.y<<" "<<A.z<<std::endl;
        return true;
    }

    return false;
}

bool FileWriter::writeTextToFile(std::string s)
{
    if (out.is_open())
    {
        out << std::fixed <<std::setprecision(7)<<s<<std::endl;
        return true;
    }

    return false;
}

bool FileWriter::writeDoubleToFile(double x)
{
    if (out.is_open())
    {
        out << std::fixed <<std::setprecision(6)<<x<<'\t'<<std::endl;
        return true;
    }

    return false;
}

void FileWriter::closeFile()
{
    out.close();
}

FileWriter::~FileWriter()
{
}

#endif
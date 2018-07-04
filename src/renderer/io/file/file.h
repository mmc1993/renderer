#pragma once
//  ����: �ļ���д
//  ����: mmc
//  ����: 2018��7��3��

#include "../../../base.h"
#include <fstream>

class Mesh;

class File {
public:
    File()
    { }
    virtual ~File()
    { }

    static bool LoadMesh(const std::string & fname, Mesh * mesh);
    static std::string GetFileString(const std::string & fname);
    static size_t GetFileSize(const std::string & fname);

private:
    static size_t GetFileSize(std::ifstream & ifile);
    static std::string GetFileString(std::ifstream & ifile);
    static bool CheckSuffix(const std::string & fname, const std::string & suffix);
};
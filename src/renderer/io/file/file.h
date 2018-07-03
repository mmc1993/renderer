#pragma once
//  ����: �ļ���д
//  ����: mmc
//  ����: 2018��7��3��

#include "../../../base.h"
#include <fstream>

class Mesh;

class File {
public:
    File();
    ~File();

    bool LoadMesh(const std::string & fname, Mesh * mesh);
    std::string LoadFile(const std::string & fname) const;
    size_t GetFileSize(const std::string & fname) const;

private:
    size_t GetFileSize(std::ifstream & ifile) const;
    std::string LoadFile(std::ifstream & ifile) const;
    bool CheckSuffix(const std::string & fname, const std::string & suffix);
};
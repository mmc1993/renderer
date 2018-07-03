#pragma once
//  功能: 文件读写
//  作者: mmc
//  日期: 2018年7月3日

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
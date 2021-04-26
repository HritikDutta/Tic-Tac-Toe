#include "fileio.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "universal/types.h"

std::string LoadFile(const char filepath[])
{
    FILE* file = fopen(filepath, "rb");

    if (!file)
        return "";

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*) malloc((length + 1) * sizeof(char));
    fread(buffer, sizeof(char), length, file);
    buffer[length] = '\0';

    std::string contents = buffer;
    free(buffer);

    fclose(file);
    return std::move(contents);
}

std::vector<Byte> LoadBinaryFile(const char filepath[])
{
    FILE* file = fopen(filepath, "rb");

    if (!file)
        return std::vector<Byte>();

    std::vector<u8> contents;

    fseek(file, 0, SEEK_END);
    contents.resize(ftell(file));
    fseek(file, 0, SEEK_SET);

    fread(contents.data(), sizeof(Byte), contents.size(), file);

    fclose(file);
    return std::move(contents);
}
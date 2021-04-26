#pragma once

#include <string>
#include <vector>
#include "universal/types.h"

std::string LoadFile(const char filepath[]);
std::vector<Byte> LoadBinaryFile(const char filepath[]);
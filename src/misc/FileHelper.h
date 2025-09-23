#pragma once
#include <fstream>
#include <vector>
#include "Logger.h"

static std::vector<char> readFile(const char *filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        ERR("Failed to open shader file!");
        return { };
    }
    const auto fileSize = file.tellg();

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();
private:
    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::Context> context;
};

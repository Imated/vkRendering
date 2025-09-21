#pragma once
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "glm/vec2.hpp"
#include <iostream>
#include <memory>
#include "misc/Logger.h"
#include "vulkan/vulkan_raii.hpp"

class Window {
public:
    Window(int width, int height, const std::string &title = "Game", bool fullscreen = false);
    ~Window();

    void shutdown();

    void createWindowSurface(vk::raii::Instance& instance);

    GLFWwindow& getWindow() const;
    vk::raii::SurfaceKHR& getSurface() const;
    glm::vec2 getSize() { return {width, height}; }

private:
    GLFWwindow* window = nullptr;
    std::unique_ptr<vk::raii::SurfaceKHR> surface;
    int width;
    int height;
};

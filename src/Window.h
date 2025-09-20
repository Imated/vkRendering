﻿#pragma once
#include <string>

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include <iostream>
#include "misc/Logger.h"

namespace physim {

    class Window {
    public:
        Window(int width, int height, const std::string& title = "Game", bool fullscreen = false);
        ~Window();

        GLFWwindow* getWindow() const;
        glm::vec2 getSize() { return { params.width, params.height }; }

        static inline struct WindowParams
        {
            int width;
            int height;
        } params;

    private:
        GLFWwindow* window = nullptr;
        GLFWglproc context = nullptr;
    };
}

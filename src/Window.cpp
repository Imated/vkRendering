﻿#include "Window.h"

namespace physim {
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void errorCallback(int error, const char* description);

    Window::Window(int width, int height, const std::string& title, bool fullscreen) {
        INFO("Initializing...");

        glfwSetErrorCallback(errorCallback);

        if (glfwInit() == GLFW_FALSE) {
            ASSERT("Failed to initialize GLFW! Aborting...");
            return;
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        width = fullscreen ? mode->width : width;
        height = fullscreen ? mode->height : height;
        window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? monitor : nullptr, nullptr);
        if (!window) {
            ASSERT("Failed to create window! Aborting...");
            glfwTerminate();
            return;
        }

        DEBUG("Successfully Created Window.");

        glfwSetFramebufferSizeCallback(window, windowSizeCallback);
        glfwSetKeyCallback(window, keyCallback);
        DEBUG("Successfully Initialized GLFW.");

        //glfwMakeContextCurrent(window);

        params.width = width;
        params.height = height;
        DEBUG("Successfully Fetched Window Size of: %d, %d", width, height);

        INFO("Initialized Successfully.");
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    static void windowSizeCallback(GLFWwindow* window, int width, int height) {
        //glViewport(0, 0, width, height);
        Window::params.width = width;
        Window::params.height = height;
    }

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    static void errorCallback(int error, const char* description) {
        ERR("GLFW Error %d: %s", error, description);
    }

    GLFWwindow* Window::getWindow() const {
        return window;
    }
}
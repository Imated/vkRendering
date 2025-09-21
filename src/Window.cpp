#include "Window.h"

static void windowSizeCallback(GLFWwindow *window, int width, int height);

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

static void errorCallback(int error, const char *description);

Window::Window(int width, int height, const std::string &title, bool fullscreen): width(width), height(height) {
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() == GLFW_FALSE) {
        ASSERT("[Window]: Failed to initialize GLFW! Aborting...");
        return;
    }
    DEBUG("[Window]: Successfully Initialized GLFW.");

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

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
        ASSERT("[Window]: Failed to create window! Aborting...");
        glfwTerminate();
        return;
    }
    DEBUG("[Window]: Successfully Created Window.");

    glfwSetFramebufferSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::shutdown() {
    surface = {};
}

void Window::createWindowSurface(vk::raii::Instance& instance) {
    VkSurfaceKHR raw = VK_NULL_HANDLE;
    glfwCreateWindowSurface(*instance, window, nullptr, &raw);

    surface = std::make_unique<vk::raii::SurfaceKHR>(instance, raw);
    DEBUG("[Window]: Successfully create window surface.");
}

static void windowSizeCallback(GLFWwindow* window, int width, int height) {
    //glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void errorCallback(int error, const char* description) {
    ERR("GLFW Error %d: %s", error, description);
}

GLFWwindow& Window::getWindow() const {
    return *window;
}

vk::raii::SurfaceKHR& Window::getSurface() const {
    return *surface;
}

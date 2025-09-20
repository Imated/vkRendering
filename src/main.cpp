#include <chrono>

#include "Renderer.h"
#include "Window.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

double deltaTime = 0.0f;
std::chrono::time_point<std::chrono::system_clock> startFrame;

auto renderer = Renderer();

int main() {
    physim::Window window(800, 600);

    renderer.init();

    while (!glfwWindowShouldClose(window.getWindow())) {
        startFrame = std::chrono::high_resolution_clock::now();



        //glfwSwapBuffers(window.getWindow());
        glfwPollEvents();

        deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();
    }
}
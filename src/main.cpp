#include <chrono>

#include "Renderer.h"
#include "Window.h"
#include "vulkan/vulkan.hpp"

double deltaTime = 0.0f;
std::chrono::time_point<std::chrono::system_clock> startFrame;

int main() {
    Window window(800, 600);

    auto rendererInfo = RendererInfo {
        true,
        { "VK_LAYER_KHRONOS_validation" },
        window
    };

    auto renderer = Renderer(rendererInfo);

    renderer.init();

    while (!glfwWindowShouldClose(&window.getWindow())) {
        startFrame = std::chrono::high_resolution_clock::now();



        //glfwSwapBuffers(window.getWindow());
        glfwPollEvents();

        deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();
    }
}
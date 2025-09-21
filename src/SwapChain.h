#pragma once
#include <chrono>

#include "Window.h"
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class SwapChain {
public:
    SwapChain(vk::raii::PhysicalDevice& device, Window& window);

    static SwapChainSupportDetails querySwapChainSupport(const vk::raii::PhysicalDevice &device, const vk::raii::SurfaceKHR &surface);

    static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    static vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);
private:
};
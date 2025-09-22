#pragma once
#include <chrono>

#include "Device.h"
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
    SwapChain(Device& device, Window& window);
private:
    static SwapChainSupportDetails querySwapChainSupport(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface);

    static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    static vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);

    std::unique_ptr<vk::raii::SwapchainKHR> swapChain;
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imageViews;
};
#include "SwapChain.h"

#include "GLFW/glfw3.h"

SwapChain::SwapChain(vk::raii::PhysicalDevice& device, Window& window) {
    auto [capabilities, formats, presentModes] = querySwapChainSupport(device, window.getSurface());

    auto surfaceFormat = chooseSurfaceFormat(formats);
    auto presentMode = choosePresentMode(presentModes);
    auto extent = chooseExtent(capabilities, window.getWindow());
}

SwapChainSupportDetails SwapChain::querySwapChainSupport(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface) {
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}

vk::SurfaceFormatKHR SwapChain::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eR8G8B8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            return availableFormat;
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            return availableFormat;
    }

    return availableFormats[0];
}

vk::PresentModeKHR SwapChain::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
            return availablePresentMode;
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D SwapChain::chooseExtent(const vk::SurfaceCapabilitiesKHR &capabilities, GLFWwindow& window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;
    int width, height;
    glfwGetFramebufferSize(&window, &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

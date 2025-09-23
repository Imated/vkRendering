#include "SwapChain.h"

#include "Device.h"
#include "GLFW/glfw3.h"

SwapChain::SwapChain(Device& device, Window& window) {
    auto [capabilities, formats, presentModes] = querySwapChainSupport(device.getPhysicalDevice(), window.getSurface());

    auto surfaceFormat = chooseSurfaceFormat(formats);
    auto presentMode = choosePresentMode(presentModes);
    auto extent = chooseExtent(capabilities, window.getWindow());

    this->extent = extent;
    this->format = surfaceFormat;

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        imageCount = capabilities.maxImageCount;

    auto queueFamilies = device.getQueueFamilies();
    auto graphicsPresentSimilar = queueFamilies.graphicsFamily != queueFamilies.presentFamily;
    vk::SwapchainCreateInfoKHR createInfo {
        {},
        window.getSurface(),
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        graphicsPresentSimilar ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
        graphicsPresentSimilar ? static_cast<uint32_t>(queueFamilies.getIndices().size()) : 0u,
        graphicsPresentSimilar ? std::vector(queueFamilies.getIndices().begin(), queueFamilies.getIndices().end()).data(): nullptr,
        capabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        presentMode,
        vk::True,
        VK_NULL_HANDLE
    };
    return;
    swapChain = std::make_unique<vk::raii::SwapchainKHR>(device.getDevice(), createInfo);

    images.resize(imageCount);
    images = swapChain->getImages();

    imageViews.reserve(imageCount);
    for (auto i = 0; i < images.size(); i++) {
        vk::ImageViewCreateInfo viewCreateInfo {
            {},
            images[i],
            vk::ImageViewType::e2D,
            surfaceFormat.format,
            vk::ComponentSwizzle { },
            vk::ImageSubresourceRange {
                vk::ImageAspectFlagBits::eColor,
                0, 1, 0, 1
            }
        };

        imageViews[i] = device.getDevice().createImageView(viewCreateInfo);
    }
}

SwapChainSupportDetails SwapChain::querySwapChainSupport(const vk::raii::PhysicalDevice& device, const vk::raii::SurfaceKHR& surface) {
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}

vk::Extent2D & SwapChain::getExtent() {
    return extent;
}

vk::SurfaceFormatKHR & SwapChain::getFormat() {
    return format;
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

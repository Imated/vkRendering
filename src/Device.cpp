#include "Device.h"

#include <set>

#include "SwapChain.h"
#include "misc/Logger.h"

Device::Device(const vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface): surface(surface) {
    auto physicalDevices = vk::raii::PhysicalDevices(instance);
    if (physicalDevices.empty())
        ERR("Failed to get a physical device with Vulkan support!");

    const std::vector deviceExtensions = {
        vk::KHRSwapchainExtensionName
    };

    for (auto device: physicalDevices) {
        if (isDeviceSuitable(device, deviceExtensions)) {
            physicalDevice = std::make_unique<vk::raii::PhysicalDevice>(device);
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
        ERR("Failed to find a suitable GPU!");

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set uniqueQueueFamilies = { queueFamilies.graphicsFamily.value(), queueFamilies.presentFamily.value() };

    auto queuePriority = 1.f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo {
            {},
            queueFamily,
            1,
            &queuePriority
        };
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures {};

    vk::DeviceCreateInfo deviceCreateInfo {
        {},
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        0, nullptr,
        static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data()
    };

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    logicalDevice = std::make_unique<vk::raii::Device>(*physicalDevice, deviceCreateInfo);

    graphicsQueue = std::make_unique<vk::raii::Queue>(*logicalDevice, queueFamilies.graphicsFamily.value(), 0);
    presentQueue = std::make_unique<vk::raii::Queue>(*logicalDevice, queueFamilies.presentFamily.value(), 0);
}

bool Device::isDeviceSuitable(const vk::raii::PhysicalDevice& device, const std::vector<const char*>& deviceExtensions) {
    const auto properties = device.getProperties();
    const auto extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);
    findQueueFamilyIndices(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        const auto swapChainSupport = SwapChain::querySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return properties.deviceType ==
           vk::PhysicalDeviceType::eDiscreteGpu
           && queueFamilies.isComplete()
           && extensionsSupported
           && swapChainAdequate;
    // discrete gpus are good and cool, geom shaders are needed to function, queue families needed to render shit and stuff
}

void Device::findQueueFamilyIndices(const vk::raii::PhysicalDevice& device) {
    queueFamilies = QueueFamilyIndices();
    const auto families = device.getQueueFamilyProperties();
    for (int i = 0; i < static_cast<int>(families.size()); ++i) {
        if (auto family = families[i]; family.queueFlags & vk::QueueFlagBits::eGraphics)
            queueFamilies.graphicsFamily = i;
        if (device.getSurfaceSupportKHR(i, surface))
            queueFamilies.presentFamily = i;
        if (queueFamilies.isComplete())
            break;
    }
}

bool Device::checkDeviceExtensionSupport(const vk::raii::PhysicalDevice &device, std::vector<const char*> deviceExtensions) {
    auto extensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : extensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

vk::raii::PhysicalDevice& Device::getPhysicalDevice() const {
    return *physicalDevice;
}

vk::raii::Device& Device::getDevice() const {
    return *logicalDevice;
}

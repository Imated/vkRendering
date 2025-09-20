#include "Device.h"

#include <set>

#include "misc/Logger.h"

Device::Device(const vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface): surface(surface) {
    auto physicalDevices = vk::raii::PhysicalDevices(instance);
    if (physicalDevices.empty())
        ERR("Failed to get a physical device with Vulkan support!");
    for (auto device: physicalDevices) {
        if (isDeviceSuitable(device)) {
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

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vk::DeviceCreateInfo deviceCreateInfo {
        {},
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
    };

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    logicalDevice = std::make_unique<vk::raii::Device>(*physicalDevice, deviceCreateInfo);

    graphicsQueue = std::make_unique<vk::raii::Queue>(*logicalDevice, queueFamilies.graphicsFamily.value(), 0);
    presentQueue = std::make_unique<vk::raii::Queue>(*logicalDevice, queueFamilies.presentFamily.value(), 0);
}

bool Device::isDeviceSuitable(const vk::raii::PhysicalDevice& device) {
    const auto properties = device.getProperties();
    const auto features = device.getFeatures();
    findQueueFamilyIndices(device);

    return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && features.geometryShader && queueFamilies.isComplete();
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

vk::raii::PhysicalDevice& Device::getPhysicalDevice() const {
    return *physicalDevice;
}

vk::raii::Device& Device::getDevice() const {
    return *logicalDevice;
}

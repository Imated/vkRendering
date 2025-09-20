#include "Device.h"

#include "misc/Logger.h"

Device::Device(const vk::raii::Instance& instance) {
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

    vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
        {},
        queueFamilies.graphicsFamily.value(),
        1
    };

    vk::PhysicalDeviceFeatures deviceFeatures { };

    vk::DeviceCreateInfo deviceCreateInfo {
        {},
        1,
        &deviceQueueCreateInfo
    };

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    logicalDevice = std::make_unique<vk::raii::Device>(*physicalDevice, deviceCreateInfo);

    graphicsQueue = std::make_unique<vk::raii::Queue>(*logicalDevice, queueFamilies.graphicsFamily.value(), 0);
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

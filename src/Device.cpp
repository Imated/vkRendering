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
}

bool Device::isDeviceSuitable(const vk::raii::PhysicalDevice& device) {
    const auto properties = device.getProperties();
    const auto features = device.getFeatures();
    findQueueFamilyIndices(device);

    return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && features.geometryShader && queueFamilies.isComplete();
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

vk::raii::PhysicalDevice Device::getPhysicalDevice() const {
    return *physicalDevice;
}

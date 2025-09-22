#pragma once
#include <memory>
#include <set>

#include "vulkan/vulkan_raii.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    std::set<uint32_t> getIndices() {
        return { graphicsFamily.value(), presentFamily.value() };
    }

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Device {
public:
    Device(const vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface);

    vk::raii::PhysicalDevice& getPhysicalDevice() const;
    vk::raii::Device& getDevice() const;

    QueueFamilyIndices getQueueFamilies() const;

private:
    bool isDeviceSuitable(const vk::raii::PhysicalDevice &device, const std::vector<const char*> &deviceExtensions);
    void findQueueFamilyIndices(const vk::raii::PhysicalDevice &device);

    bool checkDeviceExtensionSupport(const vk::raii::PhysicalDevice& device, std::vector<const char*> deviceExtensions);

    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
    std::unique_ptr<vk::raii::Device> logicalDevice;
    vk::raii::SurfaceKHR& surface;

    QueueFamilyIndices queueFamilies;

     std::unique_ptr<vk::raii::Queue> graphicsQueue;
     std::unique_ptr<vk::raii::Queue> presentQueue;
};

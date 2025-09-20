#pragma once
#include <memory>

#include "vulkan/vulkan_raii.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value();
    }
};

class Device {
public:
    Device(const vk::raii::Instance& instance);

    vk::raii::PhysicalDevice& getPhysicalDevice() const;
    vk::raii::Device& getDevice() const;

private:
    bool isDeviceSuitable(const vk::raii::PhysicalDevice &device);
    void findQueueFamilyIndices(const vk::raii::PhysicalDevice &device);

    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
    std::unique_ptr<vk::raii::Device> logicalDevice;
    QueueFamilyIndices queueFamilies;

     std::unique_ptr<vk::raii::Queue> graphicsQueue;
};

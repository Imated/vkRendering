#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "misc/Logger.h"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

Renderer::Renderer() {
    const vk::detail::DynamicLoader loader;
    auto getProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(getProcAddr);

    context = std::make_unique<vk::raii::Context>();

    vk::ApplicationInfo appInfo("Test", 1, "Custom", 1, VK_API_VERSION_1_3);

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    vk::InstanceCreateFlags flags { vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR };
    vk::InstanceCreateInfo createInfo {
        flags, &appInfo,
        0, { },
        static_cast<uint32_t>(requiredExtensions.size()),
        requiredExtensions.data()
    };

    instance = std::make_unique<vk::raii::Instance>(*context, createInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(**instance);


    auto extensions = vk::enumerateInstanceExtensionProperties();
    INFO("Available extensions:");
    for (const auto& extension : extensions) {
        INFO("\t %s", extension.extensionName);
    }
}

Renderer::~Renderer() {
}

void Renderer::init() {

}

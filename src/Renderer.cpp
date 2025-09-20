#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "misc/Logger.h"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

Renderer::Renderer(RendererInfo& info): info(info) {
    const vk::detail::DynamicLoader loader;
    auto getProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(getProcAddr);

    auto extensions = getExtensions();
    if (info.enableValidationLayers && !checkValidationLayerSupport())
        ERR("Failed to enable validation layer extensions!");

    vk::ValidationFeaturesEXT validationFeatures { };
    std::array features {
        vk::ValidationFeatureEnableEXT::eBestPractices
      };
    validationFeatures.setEnabledValidationFeatures(features);

    vk::ApplicationInfo appInfo("Test", 1, "Custom", 1, vk::HeaderVersionComplete);

    vk::InstanceCreateInfo createInfo {
        { }, &appInfo,
        info.enableValidationLayers ? static_cast<uint32_t>(info.validationLayers.size()) : 0,
        info.enableValidationLayers ? info.validationLayers.data() : nullptr,
        static_cast<uint32_t>(extensions.size()),
        extensions.data()
    };

    auto debugMessengerCreateInfo = getDebugMessengerInfo();
    vk::StructureChain chained { createInfo, validationFeatures, debugMessengerCreateInfo };

    context = std::make_unique<vk::raii::Context>();
    instance = std::make_unique<vk::raii::Instance>(*context, chained.get<vk::InstanceCreateInfo>());
    debugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(*instance, chained.get<vk::DebugUtilsMessengerCreateInfoEXT>());

    VULKAN_HPP_DEFAULT_DISPATCHER.init(**instance);

    auto availableExtensions = vk::enumerateInstanceExtensionProperties();
    INFO("Available extensions:");
    for (const auto& extension: availableExtensions) {
        INFO("\t %s", extension);
    }
}

vk::DebugUtilsMessengerCreateInfoEXT Renderer::getDebugMessengerInfo() {
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags =
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    vk::DebugUtilsMessageTypeFlagsEXT typeFlags =
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo {
            { },
            severityFlags,
            typeFlags,
            &debugCallback
        };
    return createInfo;
}

Renderer::~Renderer() {
}

void Renderer::init() {

}

vk::Bool32 Renderer::debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData) {
    switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            DEBUG("[Validation Layer]: %s", callbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            INFO("[Validation Layer]: %s", callbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            WARN("[Validation Layer]: %s", callbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            ERR("[Validation Layer]: %s", callbackData->pMessage);
            break;

        default:
            ERR("How did you get here... Just how...");
            break;
    }

    return vk::False;
}

bool Renderer::checkValidationLayerSupport() const {
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const auto& layer : info.validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layer, layerProperties.layerName) == 0) {
                layerFound = true;
                INFO("Found layer %s", layer);
                break;
            }
        }

        if (!layerFound) {
            WARN("Layer %s not found", layer);
            return false;
        }
    }

    return true;
}

std::vector<const char*> Renderer::getExtensions() const {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (info.enableValidationLayers)
        extensions.emplace_back(vk::EXTDebugUtilsExtensionName);
    return extensions;
}
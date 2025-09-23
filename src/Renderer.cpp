#include "Renderer.h"

#include "RenderPass.h"
#include "misc/Logger.h"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

Renderer::Renderer(RendererInfo& info): info(info) {
    const vk::detail::DynamicLoader loader;
    auto getProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(getProcAddr);

    auto extensions = getExtensions();
    if (info.enableValidationLayers && !checkValidationLayerSupport())
        ERR("Failed to enable validation layer extensions!");

    vk::ApplicationInfo appInfo("Test", 1, "Custom", 1, VK_API_VERSION_1_3);
    vk::InstanceCreateInfo createInfo {
        { }, &appInfo,
        info.enableValidationLayers ? static_cast<uint32_t>(info.validationLayers.size()) : 0,
        info.enableValidationLayers ? info.validationLayers.data() : nullptr,
        static_cast<uint32_t>(extensions.size()),
        extensions.data()
    };
    auto debugMessengerCreateInfo = getDebugMessengerInfo();
    vk::StructureChain chained { createInfo, debugMessengerCreateInfo };

    context = std::make_unique<vk::raii::Context>();
    instance = std::make_unique<vk::raii::Instance>(*context, chained.get<vk::InstanceCreateInfo>());
    debugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(*instance, chained.get<vk::DebugUtilsMessengerCreateInfoEXT>());

    VULKAN_HPP_DEFAULT_DISPATCHER.init(**instance);

    info.window.createWindowSurface(*instance);
    device = std::make_unique<Device>(*instance, info.window.getSurface());

    VULKAN_HPP_DEFAULT_DISPATCHER.init(**instance, device->getDevice(), loader);

    swapChain = std::make_unique<SwapChain>(*device, info.window);

    vk::PipelineLayoutCreateInfo layoutCreateInfo {
            {},
            0, nullptr,
            0, nullptr
        };

    layout = std::make_unique<vk::raii::PipelineLayout>(device->getDevice(), layoutCreateInfo);

    renderPass = std::make_unique<RenderPass>(*device, *swapChain);
    auto defaultShaderNames = Shader("resources/shaders/vert.spv", "resources/shaders/frag.spv");
    defaultShader = std::make_unique<Pipeline>(*device, *swapChain, defaultShaderNames, *layout, *renderPass);
}

vk::DebugUtilsMessengerCreateInfoEXT Renderer::getDebugMessengerInfo() {
    constexpr vk::DebugUtilsMessageSeverityFlagsEXT severityFlags =
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    constexpr vk::DebugUtilsMessageTypeFlagsEXT typeFlags =
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    constexpr vk::DebugUtilsMessengerCreateInfoEXT createInfo {
            { },
            severityFlags,
            typeFlags,
            &debugCallback
        };

    return createInfo;
}

Renderer::~Renderer() {
    info.window.shutdown();
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

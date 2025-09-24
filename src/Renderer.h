#pragma once

#include "Device.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"
#include <GLFW/glfw3.h>

#include "Pipeline.h"
#include "SwapChain.h"
#include "Window.h"

struct RendererInfo {
    bool enableValidationLayers;
    const std::vector<const char*>& validationLayers;
    Window& window;
};

class Renderer {
public:
    Renderer(RendererInfo& info);
    ~Renderer();
    void init();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT *callbackData,
        void *userData
    );
private:
    static vk::DebugUtilsMessengerCreateInfoEXT getDebugMessengerInfo();

    bool checkValidationLayerSupport() const;
    std::vector<const char*> getExtensions() const;


    RendererInfo& info;

    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::Context> context;
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugMessenger;
    std::unique_ptr<Device> device;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<vk::raii::PipelineLayout> layout;
    std::unique_ptr<RenderPass> renderPass;
    std::unique_ptr<Pipeline> defaultShader;
};

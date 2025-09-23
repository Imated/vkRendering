#include "Pipeline.h"
#include <vector>
#include "RenderPass.h"
#include "SwapChain.h"
#include "misc/FileHelper.h"
#include "vulkan/vulkan.hpp"

Pipeline::Pipeline(Device& device, SwapChain& swapChain, Shader& shader, vk::raii::PipelineLayout& layout, RenderPass& renderPass) {

    const auto vertShaderCode = readFile(shader.vertexFilename);
    const auto fragShaderCode = readFile(shader.fragmentFilename);
    auto vertexShaderModule = createShaderModule(device.getDevice(), vertShaderCode);
    auto fragmentShaderModule = createShaderModule(device.getDevice(), fragShaderCode);

    std::vector shaderStages = {
        vk::PipelineShaderStageCreateInfo{
                {},
                vk::ShaderStageFlagBits::eVertex,
                *vertexShaderModule,
                "main"
            },
            vk::PipelineShaderStageCreateInfo{
                {},
                vk::ShaderStageFlagBits::eFragment,
                *fragmentShaderModule,
                "main"
            }
    };

    std::vector dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo {
        {},
        0, nullptr,
        0, nullptr,
    };

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {
        {},
        vk::PrimitiveTopology::eTriangleList,
        vk::False
    };

    vk::Extent2D extent = swapChain.getExtent();

    vk::Viewport viewport {
        0.f, 0.f,
        static_cast<float>(extent.width), static_cast<float>(extent.height),
        0.f, 1.f
    };

    vk::Rect2D scissor {
        { 0, 0 },
        extent
    };

    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo({}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data());

    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {
        {},
        1, &viewport,
        1, &scissor
    };

    vk::PipelineRasterizationStateCreateInfo rasterizerStateCreateInfo {
        {},
        vk::False,
        vk::False,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        vk::False,
        0.f,
        0.f,
        0.f,
        1.f
    };

    vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo {
        {},
        vk::SampleCountFlagBits::e1,
        vk::False,
        1.f,
        nullptr,
        vk::False,
        vk::False
    };

    vk::PipelineColorBlendAttachmentState colorBlendAttachmentState {
        vk::True,
        vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusSrcAlpha,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    };

    vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
        {},
        vk::False,
        vk::LogicOp::eCopy,
        1,
        &colorBlendAttachmentState,
        { 0.f, 0.f, 0.f, 0.f }
    };

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo {
        {},
        2, shaderStages.data(),
        &vertexInputStateCreateInfo,
        &inputAssemblyStateCreateInfo,
        nullptr,
        &viewportStateCreateInfo,
        &rasterizerStateCreateInfo,
        &multisampleStateCreateInfo,
        nullptr,
        &colorBlendStateCreateInfo,
        &dynamicStateCreateInfo,
        *layout,
        *renderPass.getPass(),
        0,
        VK_NULL_HANDLE,
        -1
    };

    pipeline = std::make_unique<vk::raii::Pipeline>(device.getDevice(), VK_NULL_HANDLE, graphicsPipelineCreateInfo);
}

vk::raii::ShaderModule Pipeline::createShaderModule(const vk::raii::Device &device, const std::vector<char> &code) {
    vk::ShaderModuleCreateInfo createInfo{
        {},
        code.size(),
        reinterpret_cast<const uint32_t *>(code.data()),
    };

    return vk::raii::ShaderModule(device, createInfo);
}

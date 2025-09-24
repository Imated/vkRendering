#include "Pipeline.h"
#include <vector>
#include "RenderPass.h"
#include "SwapChain.h"
#include "misc/FileHelper.h"
#include "vulkan/vulkan.hpp"

Pipeline::Pipeline(PipelineConfig& config, Device& device, vk::raii::PipelineLayout& layout, RenderPass& renderPass) {

    const auto vertShaderCode = readFile(config.shader.vertexFilename);
    const auto fragShaderCode = readFile(config.shader.fragmentFilename);
    const auto vertexShaderModule = createShaderModule(device.getDevice(), vertShaderCode);
    const auto fragmentShaderModule = createShaderModule(device.getDevice(), fragShaderCode);

    std::vector shaderStages {
        vk::PipelineShaderStageCreateInfo{
            {},
            vk::ShaderStageFlagBits::eVertex,
            *vertexShaderModule,
            "main"
        },
        vk::PipelineShaderStageCreateInfo {
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

    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo {
        {},
        static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data()
    };

    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {
        {},
        1, &config.viewport,
        1, &config.scissor
    };

    vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
        {},
        vk::False,
        vk::LogicOp::eCopy,
        1,
        &config.colorBlendAttachmentState,
        { 0.f, 0.f, 0.f, 0.f }
    };

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo {
        {},
        2, shaderStages.data(),
        &vertexInputStateCreateInfo,
        &config.inputAssemblyInfo,
        nullptr,
        &viewportStateCreateInfo,
        &config.rasterizerInfo,
        &config.multisampleInfo,
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

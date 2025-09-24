#pragma once
#include "SwapChain.h"

class RenderPass;

struct Shader {
    const char *vertexFilename;
    const char *fragmentFilename;
};

struct PipelineConfig {
    Shader shader;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    vk::PipelineRasterizationStateCreateInfo rasterizerInfo;
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    vk::PipelineColorBlendAttachmentState colorBlendAttachmentState;
    vk::Viewport viewport;
    vk::Rect2D scissor;

    static constexpr  vk::PipelineInputAssemblyStateCreateInfo defaultInputAssemblyInfo {
        {},
        vk::PrimitiveTopology::eTriangleList,
        vk::False
    };

    static constexpr vk::PipelineRasterizationStateCreateInfo defaultRasterizerInfo {
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

    static constexpr vk::PipelineMultisampleStateCreateInfo defaultMultisampleInfo {
        {},
        vk::SampleCountFlagBits::e1,
        vk::False,
        1.f,
        nullptr,
        vk::False,
        vk::False
    };

    static constexpr vk::PipelineColorBlendAttachmentState defaultColorBlendAttachmentState {
        vk::True,
        vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusSrcAlpha,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA
    };
};

class Pipeline {
public:
    explicit Pipeline(PipelineConfig& config, Device& device, vk::raii::PipelineLayout& layout, RenderPass& renderPass);
private:
    static vk::raii::ShaderModule createShaderModule(const vk::raii::Device& device, const std::vector<char>& code);

    std::unique_ptr<vk::raii::Pipeline> pipeline;
};

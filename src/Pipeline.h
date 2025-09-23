#pragma once
#include "SwapChain.h"

class RenderPass;
class Shader;

struct Shader {
    const char* vertexFilename;
    const char* fragmentFilename;
};

class Pipeline {
public:
    explicit Pipeline(Device& device, SwapChain& swapChain, Shader& shader, vk::raii::PipelineLayout& layout, RenderPass& renderPass);
private:
    static vk::raii::ShaderModule createShaderModule(const vk::raii::Device& device, const std::vector<char>& code);

    std::unique_ptr<vk::raii::Pipeline> pipeline;
};

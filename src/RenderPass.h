#pragma once
#include "SwapChain.h"

class RenderPass {
public:
    RenderPass(Device& device, SwapChain& swapChain);

    vk::raii::RenderPass& getPass() const;
private:
    std::unique_ptr<vk::raii::RenderPass> renderPass;
};

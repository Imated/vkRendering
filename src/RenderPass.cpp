#include "RenderPass.h"

#include "SwapChain.h"
#include "vulkan/vulkan.hpp"

RenderPass::RenderPass(Device& device, SwapChain& swapChain) {
    vk::AttachmentDescription colorAttachment {
        {},
        swapChain.getFormat().format,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference colorAttachmentReference {
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::SubpassDescription subpass {
        {},
        vk::PipelineBindPoint::eGraphics,
        0, nullptr,
        1, &colorAttachmentReference
    };

    vk::RenderPassCreateInfo renderPassCreateInfo {
        {},
        1, &colorAttachment,
        1, &subpass
    };

    renderPass = std::make_unique<vk::raii::RenderPass>(device.getDevice(), renderPassCreateInfo);
}

vk::raii::RenderPass& RenderPass::getPass() const {
    return *renderPass;
}

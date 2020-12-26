#include "willow/rendering/Vulkan/vulkan_render_pass.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"


namespace wlo{

	VulkanRenderPass::VulkanRenderPass(VkDevice device, VkFormat surfaceFormat)
        {
            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

            renderPassInfo.subpassCount  = 1;
            VkSubpassDescription subpassDescription{};
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescription.colorAttachmentCount = 1;
            VkAttachmentReference attachmentRef{};
            attachmentRef.attachment = 0;
            attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            subpassDescription.pColorAttachments = &attachmentRef;

            renderPassInfo.pSubpasses = &subpassDescription;


            renderPassInfo.attachmentCount = 1;
            VkAttachmentDescription colorAttachmentDescription{};
            colorAttachmentDescription.format = surfaceFormat;
            colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

            colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

            colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            renderPassInfo.pAttachments = &colorAttachmentDescription;

            renderPassInfo.dependencyCount = 1;
            VkSubpassDependency dep{};

            dep.srcSubpass = VK_SUBPASS_EXTERNAL;
            dep.dstSubpass = 0;
            dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dep.srcAccessMask = 0;
            dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dep.dstStageMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            renderPassInfo.pDependencies = &dep;

            VkResult res = vkCreateRenderPass(device,&renderPassInfo,nullptr, &m_vkRenderPass);
            vk::checkResult(res,"create render pass");
        }

	VkRenderPass VulkanRenderPass::get() const{
            return m_vkRenderPass;
        }

        void VulkanRenderPass::reclaim(VkDevice device){
           vkDestroyRenderPass(device,m_vkRenderPass,nullptr);
        }

        VulkanRenderPass::~VulkanRenderPass(){
        }

}
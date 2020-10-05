#include "willow/rendering/Vulkan/vulkan_render_core.hpp"
#include "willow/rendering/Vulkan/vulkan_context.hpp"
#include "willow/rendering/Vulkan/vulkan_swapchain.hpp"
#include "willow/rendering/Vulkan/vulkan_vertex_buffer.hpp"
#include "willow/rendering/Vulkan/vulkan_graphics_pipeline.hpp"
#include "willow/rendering/Vulkan/vulkan_frame_buffer_array.hpp"
#include "willow/rendering/Vulkan/vulkan_uniform_buffer_array.hpp"
#include "willow/rendering/Vulkan/vulkan_index_buffer.hpp"

namespace wlo{

	class VulkanRenderCoreImpl {

	public:
		VulkanRenderCoreImpl(wlo::SharedPointer<wlo::Window> window, RenderCore::Info info) :
			curFrameIdx(0)
		{
			VulkanRenderingContext::Info contextInfo;
			contextInfo.enableValidationLayers = info.enableDebugging;
			context.initialize(window, contextInfo);
			swapchain.initialize(&context, window->getInfo().m_width, window->getInfo().m_height);

			VulkanRenderPass renderPass(context.getDevice(), swapchain.getSwapSurfaceFormat());
			renderPasses.push_back(renderPass);

			frameBufferArray.initialize(context.getDevice(), swapchain, renderPasses[0]);

			vertexBuffer.initialize(&context);
			indexBuffer.initialize(&context);

			vertexBuffer.allocate(info.maxVertices);
			indexBuffer.allocate(info.maxVertices);
		}

		void initialize() {

			vk::QueueFamilyIndices queueIndices = vk::findQueueFamilies(context.getPhysicalDevice(), context.getSurface());
			vkGetDeviceQueue(context.getDevice(), queueIndices.graphicsFamily.value(), 0, &graphicsQueue);
			vkGetDeviceQueue(context.getDevice(), queueIndices.presentFamily.value(), 0, &presentQueue);
			createCommandPool();
			allocateDrawCommandBuffers();
			createSynchronizationPrimitives();
			uniformBufferArray.initialize(&context,swapchain.getImageViews().size());
            createDescriptorPool();
            allocateDescriptorSets();
            

		}

		virtual void resizeRenderSurface(uint32_t width, uint32_t height) {
			waitOnCurrentTasks();
			swapchain.reinitialize(width, height);
			frameBufferArray.reinitialize(context.getDevice(), swapchain, renderPasses[0]);
			pipelines[0].reinitialize(context.getDevice(), swapchain.getSwapSurfaceExtent());
			curFrameIdx = 0;
		}


        void setClearColor(VkClearValue color){
            clearColor = color;
        }
        void beginDrawCommand() {
			vkWaitForFences(context.getDevice(), 1, &commandBufferExecutingFences[curFrameIdx], VK_TRUE, UINT64_MAX);

			VkCommandBufferBeginInfo bufferBeginInfo{};
			bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			bufferBeginInfo.flags = 0;
			bufferBeginInfo.pInheritanceInfo = nullptr;
			vkBeginCommandBuffer(m_drawCommandBuffers[curFrameIdx], &bufferBeginInfo);

			VkRenderPassBeginInfo passBeginInfo{};
			passBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			passBeginInfo.renderPass = renderPasses[0].get();
			passBeginInfo.framebuffer = frameBufferArray[curFrameIdx];
			passBeginInfo.renderArea.extent = swapchain.getSwapSurfaceExtent();
			passBeginInfo.clearValueCount = 1;
			passBeginInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_drawCommandBuffers[curFrameIdx], &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		}
		void drawCmdBindPipeline() {

			vkCmdBindPipeline(m_drawCommandBuffers[curFrameIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[0].get());
			//	WILO_CORE_INFO("Bound graphics pipeline zero");

		}

		void completeDrawCommand() {
			VkDeviceSize offsets[] = { 0 };
			VkBuffer buffers[] = { vertexBuffer.get() };
			vkCmdBindVertexBuffers(m_drawCommandBuffers[curFrameIdx], 0, 1, buffers, offsets);

			vkCmdBindIndexBuffer(m_drawCommandBuffers[curFrameIdx], indexBuffer.get(), 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(m_drawCommandBuffers[curFrameIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[0].getLayout(), 0, 1, &descriptorSets[curFrameIdx], 0, nullptr);
			vkCmdDrawIndexed(m_drawCommandBuffers[curFrameIdx], static_cast<uint32_t>(indexBuffer.size()), 1, 0, 0, 0);

			vkCmdEndRenderPass(m_drawCommandBuffers[curFrameIdx]);

			VkResult res = vkEndCommandBuffer(m_drawCommandBuffers[curFrameIdx]);
			vk::checkResult(res, "end command buffer");
		}

		void submitDrawCommand() {

			uint32_t imageIndex;
			vkAcquireNextImageKHR(context.getDevice(), swapchain.get(), UINT64_MAX, imageAvailableSemaphores[curFrameIdx], VK_NULL_HANDLE, &imageIndex);
			WILO_ASSERT(imageIndex == curFrameIdx);

			if (imageInUseFences[imageIndex] != VK_NULL_HANDLE) {
				vkWaitForFences(context.getDevice(), 1, &imageInUseFences[imageIndex], VK_TRUE, UINT64_MAX);
			}
            

			imageInUseFences[imageIndex] = commandBufferExecutingFences[curFrameIdx];

			VkSubmitInfo submitInfo{ };

			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &imageAvailableSemaphores[curFrameIdx];

			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_drawCommandBuffers[imageIndex];

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

			vkResetFences(context.getDevice(), 1, &commandBufferExecutingFences[curFrameIdx]);

			VkResult res1 = vkQueueSubmit(graphicsQueue, 1, &submitInfo, commandBufferExecutingFences[curFrameIdx]);
			vk::checkResult(res1, "submit draw command queue");

			VkPresentInfoKHR presentInfo{};

			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &renderFinishedSemaphores[curFrameIdx];
			VkSwapchainKHR swapchains[] = { swapchain.get() };
			presentInfo.pSwapchains = swapchains;

			presentInfo.swapchainCount = 1;

			presentInfo.pImageIndices = &imageIndex;

			presentInfo.pResults = nullptr;

			vkQueuePresentKHR(presentQueue, &presentInfo);

			curFrameIdx += 1;
			curFrameIdx %= frameBufferArray.size();
		}

		void uploadVertices(const std::vector<Vertex3D>& vertices, size_t count) {
			vertexBuffer.fill(vertices, count);
		}
		void uploadIndices(const std::vector<uint32_t>& indices, size_t count) {
			indexBuffer.fill(indices, count);
		}

		~VulkanRenderCoreImpl() = default;

		void buildPipeline(std::string vertexShaderPath, std::string fragmentShaderPath) {
			VulkanShader vertexShader(context.getDevice(), vertexShaderPath);
			VulkanShader fragmentShader(context.getDevice(), fragmentShaderPath);
			VulkanGraphicsPipeline pipeline(context.getDevice(), swapchain.getSwapSurfaceExtent(), renderPasses[0], vertexShader, fragmentShader);
			pipeline.pushDescriptorSetLayout(uniformBufferArray.getDescriptorSetLayout());
			pipelines.push_back(pipeline);
            pipelines[0].reinitialize(context.getDevice(), swapchain.getSwapSurfaceExtent());
		}


		void createCommandPool() {

			vk::QueueFamilyIndices queueIndices = vk::findQueueFamilies(context.getPhysicalDevice(), context.getSurface());
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueIndices.graphicsFamily.value();
			poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			VkResult res = vkCreateCommandPool(context.getDevice(), &poolInfo, nullptr, &m_vkCommandPool);
			vk::checkResult(res, "create command pool");
		}

		void allocateDrawCommandBuffers() {

			m_drawCommandBuffers.resize(frameBufferArray.size());
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandBufferCount = (uint32_t)m_drawCommandBuffers.size();
			allocInfo.commandPool = m_vkCommandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

			VkResult res = vkAllocateCommandBuffers(context.getDevice(), &allocInfo, m_drawCommandBuffers.data());

			vk::checkResult(res, "allocate draw command buffers");
		}

		void createSynchronizationPrimitives() {
			VkSemaphoreCreateInfo semInfo{};
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			renderFinishedSemaphores.resize(frameBufferArray.size());
			imageAvailableSemaphores.resize(frameBufferArray.size());
			commandBufferExecutingFences.resize(frameBufferArray.size());
			imageInUseFences.resize(frameBufferArray.size(), VK_NULL_HANDLE);

			for (size_t i = 0; i < frameBufferArray.size(); i++) {
				VkResult res1 = vkCreateSemaphore(context.getDevice(), &semInfo, nullptr, &renderFinishedSemaphores[i]);
				VkResult res2 = vkCreateSemaphore(context.getDevice(), &semInfo, nullptr, &imageAvailableSemaphores[i]);
				VkResult res3 = vkCreateFence(context.getDevice(), &fenceInfo, nullptr, &commandBufferExecutingFences[i]);
				//        VkResult res4 = vkCreateFence(p_context->getDevice(),&fenceInfo,nullptr,&imageInUseFences[i]);

				vk::checkResult(res1, "create render finished semaphore");
				vk::checkResult(res2, "create image available semaphore");
				vk::checkResult(res3, "create command buffer executed fence");
				//       vk::checkResult(res4, "create image used fence");
			}
		}


		void createDescriptorPool(){
			VkDescriptorPoolSize poolSize{};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = static_cast<uint32_t> (frameBufferArray.size());

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32_t>(frameBufferArray.size());

			VkResult res = vkCreateDescriptorPool(context.getDevice(), &poolInfo, nullptr, &m_vkDescriptorPool);
			vk::checkResult(res,"create descriptor pool");
		}


		void allocateDescriptorSets(){
			descriptorSets.resize(frameBufferArray.size());

			std::vector<VkDescriptorSetLayout> layouts(frameBufferArray.size(), uniformBufferArray.getDescriptorSetLayout());
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.descriptorPool = m_vkDescriptorPool;
			allocInfo.descriptorSetCount = frameBufferArray.size();
			allocInfo.pSetLayouts = layouts.data();

			VkResult res = vkAllocateDescriptorSets(context.getDevice(), &allocInfo, descriptorSets.data());

			vk::checkResult(res, "allocate descriptor sets");
            updateDescriptorSets();
		}

		void updateDescriptorSets(){
            std::vector<VkDescriptorBufferInfo> bufferInfos = uniformBufferArray.getBufferInfos();
            for (size_t i =0; i<bufferInfos.size(); i++ ){
                VkWriteDescriptorSet descriptorWrite{};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = descriptorSets[i];
                descriptorWrite.dstBinding = 0;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                
                descriptorWrite.pBufferInfo = &bufferInfos[i];
                descriptorWrite.pImageInfo = nullptr; //no image resources yet
                
                vkUpdateDescriptorSets(context.getDevice(), 1, &descriptorWrite, 0, nullptr);
            }
		}


		void waitOnCurrentTasks() {
			vkDeviceWaitIdle(context.getDevice());
		}
		void submitUniforms(glm::mat4x4 model, glm::mat4x4 view, glm::mat4x4 proj) {
			uniformBufferArray.setModel(curFrameIdx,model);
			uniformBufferArray.setView(curFrameIdx,view);
//			proj[1][1]*=-1;
			uniformBufferArray.setProjection(curFrameIdx,proj);
			uniformBufferArray.upload(curFrameIdx);
		}
        
	private:
		std::vector<VulkanRenderPass> renderPasses;
		std::vector<VulkanGraphicsPipeline> pipelines;
		VulkanRenderingContext context;
		VulkanSwapchain swapchain;
		VulkanVertexBuffer vertexBuffer;
		VulkanIndexBuffer indexBuffer;

		VulkanUniformBufferArray uniformBufferArray;
		VulkanFrameBufferArray frameBufferArray;
		VkClearValue clearColor = {0,0,0,1};
		size_t curFrameIdx;
		VkCommandPool m_vkCommandPool;


		VkDescriptorPool m_vkDescriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> commandBufferExecutingFences;
		std::vector<VkFence> imageInUseFences;

		std::vector<VkCommandBuffer> m_drawCommandBuffers;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

	};

	VulkanRenderCore::VulkanRenderCore(wlo::SharedPointer<wlo::Window> window, RenderCore::Info info):
            wlo::RenderCore(window, info)
	{
		p_Impl = wlo::CreateUniquePointer<VulkanRenderCoreImpl>(window, info);
	}

	VulkanRenderCore::~VulkanRenderCore() {

	}

	void VulkanRenderCore::initialize()
	{
		p_Impl->initialize();
		WILO_CORE_INFO("Render Core Initilaized");
	}

	void VulkanRenderCore::waitForLastFrame()
	{
		p_Impl->waitOnCurrentTasks();
	}

	void VulkanRenderCore::beginDrawCall()
	{
//		WILO_CORE_INFO("started Draw Call");
		p_Impl->beginDrawCommand();
        p_Impl->drawCmdBindPipeline();
	}

	void VulkanRenderCore::resizeRenderSurface(uint32_t width, uint32_t height)
	{
		p_Impl->resizeRenderSurface(width, height);
	}

	void VulkanRenderCore::buildPipeline(std::string vertexShader, std::string fragmentShader)
	{
		p_Impl->buildPipeline(vertexShader, fragmentShader);
	}


	void VulkanRenderCore::submitVertexBuffer(const std::vector<Vertex3D>& vertices, size_t count)
	{
		p_Impl->uploadVertices(vertices, count);

	}


	void VulkanRenderCore::submitIndexBuffer(const std::vector<uint32_t>& indices, size_t count)
	{
		p_Impl->uploadIndices(indices, count);
	}

	void VulkanRenderCore::submitUniforms(glm::mat4x4 model,glm::mat4x4 view, glm::mat4x4 projection)
	{
        p_Impl->submitUniforms(model, view, projection);
		
	}

	void VulkanRenderCore::submitDrawCall()
	{
		p_Impl->completeDrawCommand();
		p_Impl->submitDrawCommand();
	}

    void VulkanRenderCore::setClearColor(glm::vec4 color) {
	   VkClearValue value = {color[0],color[1],color[2],color[3]};
	   p_Impl->setClearColor(value);
	}

}

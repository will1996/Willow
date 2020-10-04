#pragma once 
#include "willow/rendering/wilo_render_core.hpp"


namespace wlo {
	class VulkanRenderCoreImpl;

	class VulkanRenderCore : public wlo::RenderCore {
	public:
		VulkanRenderCore(wlo::SharedPointer<wlo::Window>, wlo::RenderCore::Info);
		~VulkanRenderCore();



		virtual void initialize()override;
		virtual void waitForLastFrame() override;
		virtual void beginDrawCall() override;
		virtual void resizeRenderSurface(uint32_t width, uint32_t height) override;
		virtual void buildPipeline(std::string vertexShader, std::string fragmentShader) override;
        virtual void setClearColor(glm::vec4 color) override;
		virtual void submitVertexBuffer(const std::vector<Vertex3D>& vertices, size_t count) override;
		virtual void submitIndexBuffer(const std::vector<uint32_t>& indices, size_t count)  override;
		virtual void submitUniforms(glm::mat4x4 model,glm::mat4x4 view, glm::mat4x4 projection)  override;
		virtual void submitDrawCall() override;

	private:

		wlo::UniquePointer<VulkanRenderCoreImpl> p_Impl;


	};

}

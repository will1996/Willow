#pragma once 
#include "willow/rendering/wilo_render_core.hpp"


namespace wlo {
	class VulkanRenderCoreImpl;

	class VulkanRenderCore : public wlo::RenderCore {
	public:
		VulkanRenderCore(wlo::SharedPointer<wlo::Window>, wlo::RenderCore::Info);

		~VulkanRenderCore() override;
		 void initialize()override;
		 void waitForLastFrame() override;
		 void beginDrawCall() override;
		 void resizeRenderSurface(uint32_t width, uint32_t height) override;
		 void buildPipeline(std::string vertexShader, std::string fragmentShader) override;
         void setClearColor(glm::vec4 color) override;
		 void submitVertexBuffer(const std::vector<Vertex3D>& vertices, size_t count) override;
		 void submitIndexBuffer(const std::vector<uint32_t>& indices, size_t count)  override;
		 void submitUniforms(glm::mat4x4 model,glm::mat4x4 view, glm::mat4x4 projection)  override;
		 void submitDrawCall() override;
		 void clearScreen() override;
         void allocateVertexBuffer(size_t)  override;
         void allocateIndexBuffer(size_t) override;
         void reclaim() override;
	private:

		wlo::UniquePointer<VulkanRenderCoreImpl> p_Impl;


	};

}

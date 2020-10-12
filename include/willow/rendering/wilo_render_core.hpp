#pragma once

#include "wilo_vertex_types.hpp"
#include "willow/window/wilo_window.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include <glm/glm.hpp>
namespace wlo{
	class RenderCore {
	public:
		struct Info {
			uint32_t maxVertices;
			bool enableDebugging;
			bool enableProfiling;
		};
		RenderCore(wlo::SharedPointer<wlo::Window> window, RenderCore::Info) {};

		virtual void initialize()= 0;
		virtual void waitForLastFrame() = 0;
		virtual void beginDrawCall() = 0;
		virtual void resizeRenderSurface(uint32_t width, uint32_t height) = 0;
		virtual void buildPipeline(std::string vertexShaderPath, std::string FragmentShaderPath) = 0;
		virtual void setClearColor(glm::vec4 color) =0;
		virtual void allocateVertexBuffer(size_t) = 0;
		virtual void submitVertexBuffer(const std::vector<Vertex3D>& vertices, size_t count) = 0;
        virtual void allocateIndexBuffer(size_t) = 0;
		virtual void submitIndexBuffer(const std::vector<uint32_t>& indices, size_t count) = 0;
		virtual void submitUniforms(glm::mat4x4 model,glm::mat4x4 view, glm::mat4x4 projection) = 0;
		virtual void submitDrawCall() = 0;
		virtual void clearScreen()  = 0;
		virtual void reclaim() = 0;

		
		virtual ~RenderCore() {};

	
	};
	


}


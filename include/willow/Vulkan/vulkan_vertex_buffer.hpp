#pragma once
#include "vulkan_context.hpp"
#include "VulkanBuffer.hpp"
#include "willow/rendering/wilo_vertex_types.hpp"
#include "willow/dev-utils/Vulkan/wilovk.hpp"

namespace wlo{
    class VulkanVertexBuffer {
        public:
            VulkanVertexBuffer();
            void initialize(VulkanRenderingContext* contextRef);
             void allocate(size_t nElements) ;
             void reallocate(size_t nElements);
             void fill(const std::vector<Vertex3D>& data, size_t nElements, size_t offset = 0) ;
             void reclaim() ;
             size_t size() ;//must return the number of elements
             size_t memSizeBytes() ;//must return the memory footprint in bytes
            VkBuffer get();

        private:

            VulkanBuffer<Vertex3D> m_data;
    };

    }
#pragma once
#include "lve_engine_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

//std
#include <vector>

namespace lve
{
	class LveModel
	{
	public:
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color; // interleaving 
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};
		LveModel(LveEngineDevice& device, const std::vector<Vertex>& vertices);
		~LveModel();
		LveModel(const LveModel&) = delete;
		LveModel& operator = (const LveModel&) = delete;
		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices );
		LveEngineDevice& _lveDevice;
		VkBuffer _vertexBuffer;
		VkDeviceMemory  _vertexBufferMemory;
		uint32_t _vertexCount;

	};
}
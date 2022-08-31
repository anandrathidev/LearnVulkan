#include "lve_model.h"
#include <cassert>

namespace lve
{

	LveModel::LveModel(LveEngineDevice& device, const std::vector<Vertex>& vertices):_lveDevice(device)
	{
		createVertexBuffers(vertices);
	}

	void LveModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		_vertexCount = static_cast<uint32_t>(vertices.size());
		assert(_vertexCount >= 3 && "Vertex count must atleast be 3");
		VkDeviceSize bufferSize = sizeof(vertices[0])* _vertexCount;
		_lveDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_vertexBuffer,
			_vertexBufferMemory
			);
		void* data;
		vkMapMemory(_lveDevice.device(), _vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(_lveDevice.device(), _vertexBufferMemory);
	}

	LveModel::~LveModel()
	{
		vkDestroyBuffer(_lveDevice.device(), _vertexBuffer, nullptr);
		vkFreeMemory(_lveDevice.device(), _vertexBufferMemory, nullptr);
	}

	void LveModel::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
	}

	void LveModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { _vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions{1};
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		// 
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescription{ 2 };
		attributeDescription[0].binding = 0;
		attributeDescription[0].location = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription[0].offset = offsetof(Vertex, position);
		attributeDescription[1].binding = 0;
		attributeDescription[1].location = 1;
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].offset = offsetof(Vertex, color);
		return attributeDescription;
		/*
		* return {
				{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)},
				{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}
				};
		*
		*/

	}


}
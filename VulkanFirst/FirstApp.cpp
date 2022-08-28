#include "FirstApp.h"
#include <stdexcept>
#include <array>

namespace lve
{

	void FirstApp::run()
	{
		while (!_lveWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(_lveEngineDevice.device());
	}

	FirstApp::FirstApp()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(_lveEngineDevice.device(), _pipelineLayout, nullptr);
	}

	void FirstApp::createPipeline()
	{
		_lvePipeLine = std::make_unique<LvePipeLine>( _lveEngineDevice, 
			"C:/Users/a_rathi/LocalDocuments/LearnVulkan/VulkanFirst/x64/Debug/simple_shader.vert.spv", 
			"C:/Users/a_rathi/LocalDocuments/LearnVulkan/VulkanFirst/x64/Debug/simple_shader.frag.spv", 
			_pipelineLayout, _lveSwapChain.getRenderPass(), 
			WIDTH, HEIGHT);
		PipeLineConfigInfo& pipelineConfig = _lvePipeLine->getdefaultPipeLineConfigInfo();
}

	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		//pipelineLayoutInfo.flags;
		//pipelineLayoutInfo.pNext;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(_lveEngineDevice.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("faile to create pipeline layput");
		}
	}

	void FirstApp::createCommandBuffers()
	{
		_commandBuffer.resize(_lveSwapChain.imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _lveEngineDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffer.size());
		if (vkAllocateCommandBuffers(_lveEngineDevice.device(), &allocInfo, _commandBuffer.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create Commad Buffer FirstApp::createCommandBuffers");
		}
		for (int i = 0; i < _commandBuffer.size(); ++i)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if ( vkBeginCommandBuffer(_commandBuffer[i], &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to Commad Buffer Begin recording FirstApp::createCommandBuffers");
			}
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = _lveSwapChain.getRenderPass();
			renderPassInfo.framebuffer = _lveSwapChain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = _lveSwapChain.getSwapChainExtent();

			//VkClearValue clearValues;
			std::array<VkClearValue, 2> clearValues;
			clearValues[0].color = { 1.0f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0};
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(_commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			_lvePipeLine->bind(_commandBuffer[i]);
			vkCmdDraw(_commandBuffer[i], 3, 1, 0 , 0);
			vkCmdEndRenderPass(_commandBuffer[i]);
			if (vkEndCommandBuffer(_commandBuffer[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("FirstApp::createCommandBuffers failed to record Commad Buffer Begin recording ");
			}
		} // for
	}

	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = _lveSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("FirstApp::drawFrame failed to acquire SwapChain Image ");
		}
		result = _lveSwapChain.submitCommandBuffers(&_commandBuffer[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("FirstApp::drawFrame failed to present submit  SwapChain Image Command Buffers ");
		}

	}

}
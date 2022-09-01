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
		loadModels();
		createPipelineLayout();
		//createPipeline();
		recreateSwapChain();
		createCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(_lveEngineDevice.device(), _pipelineLayout, nullptr);
	}

	void FirstApp::createPipeline()
	{
		assert(_lveSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
		_lvePipeLine = std::make_unique<LvePipeLine>( _lveEngineDevice, 
			"C:/Users/a_rathi/LocalDocuments/LearnVulkan/VulkanFirst/x64/Debug/simple_shader.vert.spv", 
			"C:/Users/a_rathi/LocalDocuments/LearnVulkan/VulkanFirst/x64/Debug/simple_shader.frag.spv", 
			_pipelineLayout, _lveSwapChain->getRenderPass(), 
			WIDTH, HEIGHT);
		PipeLineConfigInfo& pipelineConfig = _lvePipeLine->getdefaultPipeLineConfigInfo();
	}

	void FirstApp::loadModels()
	{
		std::vector<LveModel::Vertex> vertices {
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
			{{0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} },
			{{-0.0f, 0.5f}, {0.0f, 0.0f, 1.0f} }
		};
		_lveModel = std::make_unique<LveModel>(_lveEngineDevice, vertices);
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

	/*
	void FirstApp::recreateSwapChain()
	{
		auto extent = _lveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = _lveWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(_lveEngineDevice.device());
		_lveSwapChain = nullptr;
		_lveSwapChain = std::make_unique<LveSwapChain>(_lveEngineDevice, extent);
		createPipeline();

	}*/

	void FirstApp::recreateSwapChain() {
		auto extent = _lveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = _lveWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(_lveEngineDevice.device());

		if (_lveSwapChain == nullptr) {
			_lveSwapChain = std::make_unique<LveSwapChain>(_lveEngineDevice, extent);
		}
		else {
			_lveSwapChain = std::make_unique<LveSwapChain>(_lveEngineDevice, extent, std::move(_lveSwapChain));
			if (_lveSwapChain->imageCount() != _commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}


	void FirstApp::freeCommandBuffers() 
	{
		vkFreeCommandBuffers(
			_lveEngineDevice.device(),
			_lveEngineDevice.getCommandPool(),
			static_cast<uint32_t>(_commandBuffers.size()),
			_commandBuffers.data());
		_commandBuffers.clear();
	}

	void FirstApp::createCommandBuffers()
	{
		_commandBuffers.resize(_lveSwapChain->imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _lveEngineDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());
		if (vkAllocateCommandBuffers(_lveEngineDevice.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create Commad Buffer FirstApp::createCommandBuffers");
		}
		/*
			for (int i = 0; i < _commandBuffer.size(); ++i)
			{
			} // for
		*/
	}

	void FirstApp::recordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if ( vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to Commad Buffer Begin recording FirstApp::createCommandBuffers");
		}
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _lveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = _lveSwapChain->getFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = _lveSwapChain->getSwapChainExtent();

		//VkClearValue clearValues;
		std::array<VkClearValue, 2> clearValues;
		clearValues[0].color = { 1.0f, 1.0f, 1.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(_lveSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(_lveSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, _lveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);
		_lvePipeLine->bind(_commandBuffers[imageIndex]);
		_lveModel->bind(_commandBuffers[imageIndex]);
		_lveModel->draw(_commandBuffers[imageIndex]);

		vkCmdEndRenderPass(_commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("FirstApp::createCommandBuffers failed to record Commad Buffer Begin recording ");
		}
	}

	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = _lveSwapChain->acquireNextImage(&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("FirstApp::drawFrame failed to acquire SwapChain Image ");
		}
		
		recordCommandBuffer(imageIndex);
		result = _lveSwapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _lveWindow.wasWindowResized())
		{
			_lveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("FirstApp::drawFrame failed to present submit  SwapChain Image Command Buffers ");
		}

	}

}
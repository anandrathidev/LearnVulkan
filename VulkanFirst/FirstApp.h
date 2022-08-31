#pragma once
#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_engine_device.hpp"
#include "lve_engine_swap_chain.hpp"
#include <memory>
#include <vector>
#include "lve_model.h"

namespace lve {
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
		FirstApp();
		~FirstApp();
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator = (const FirstApp&) = delete;


	private:
		void loadModels();
		void createPipeline();
		void createPipelineLayout();
		void createCommandBuffers();
		void drawFrame();

		LveWindow _lveWindow{ WIDTH, HEIGHT, "Hello Vulkan" };
		LveEngineDevice _lveEngineDevice{ _lveWindow };
		LveSwapChain _lveSwapChain{ _lveEngineDevice, _lveWindow.getExtent() };
		std::unique_ptr<LvePipeLine> _lvePipeLine;
		VkPipelineLayout _pipelineLayout;
		std::vector<VkCommandBuffer> _commandBuffer;
		std::unique_ptr<LveModel> _lveModel;

	};

} // lve 

#pragma once
#include <string>
#include <vector>

namespace lve {
	struct PipeLineConfigInfo {
		PipeLineConfigInfo(const PipeLineConfigInfo&) = delete;
		PipeLineConfigInfo& operator=(const PipeLineConfigInfo&) = delete;
		PipeLineConfigInfo() = default;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;

		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LveEngineDevice;
	class LvePipeLine
	{
	public:
		LvePipeLine(LveEngineDevice& device,
			const std::string& vertFilepath, 
			const std::string& fragFilepath,
			VkPipelineLayout pipelineLayout,
			VkRenderPass renderPass,
			uint32_t width, uint32_t height
			);
		~LvePipeLine();
		LvePipeLine(const LvePipeLine&) = delete;
		LvePipeLine& operator =(const LvePipeLine&) = delete;
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shadermodule);

		PipeLineConfigInfo& createdefaultPipeLineConfigInfo(uint32_t width, uint32_t height, VkPipelineLayout pipelineLayout, VkRenderPass renderPass);
		PipeLineConfigInfo& getdefaultPipeLineConfigInfo() { return _pipeLineConfigInfo; };

		void bind(VkCommandBuffer commandBuffer);
	private:
		PipeLineConfigInfo _pipeLineConfigInfo;
		LveEngineDevice& _lveEngineDevice;
		VkPipeline _graphicsPipeline;
		VkShaderModule _vertVkShaderModule;
		VkShaderModule _fragVkShaderModule;
		VkPipelineLayout _pipelineLayout;
		static std::vector<char> readFile(const std::string& filepath);
		void createGraphicsPipeLine(const std::string& vertFilepath, const std::string& fragFilepath);
	};
} // namespace lve 
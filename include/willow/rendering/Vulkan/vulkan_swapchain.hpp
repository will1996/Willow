#pragma once
#include"vulkan_context.hpp"
#include"willow/dev-utils/Vulkan/wilovk.hpp"
namespace wlo{
class VulkanSwapchain {
	public:	
		VulkanSwapchain();
		void initialize(const VulkanRenderingContext* pcontext, uint32_t width, uint32_t height) ;

		void reinitialize(uint32_t width, uint32_t height) ;

		VkSwapchainKHR get();
		VkExtent2D getSwapSurfaceExtent()const ;
		VkFormat getSwapSurfaceFormat() const;
		const std::vector<VkImageView>& getImageViews()const ;
		void reclaim() ;
private:
	//member variables
		//content used by this class to populate and build itself
	const VulkanRenderingContext* p_context;
	vk::SwapChainSupportDetails m_supportDetails;
		
	VkSwapchainKHR m_swapchain;

		//details about the swapchain images, stored for future use
	VkExtent2D m_swapSurfaceExtent;
	VkSurfaceFormatKHR m_swapSurfaceFormat;
	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_imageViews;

	//member functions
	void createVkSwapchain();
	void getSwapchainImages();
	void createSwapchainImageViews();
		//set the member variables for image format, and extent	
	void chooseSwapSurfaceFormat();
	void 		   chooseSwapChainExtent();
	//helper functions
	VkPresentModeKHR   choosePresentMode();

};

}

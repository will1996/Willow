#pragma once
#include<vulkan/vulkan.hpp>
#include "willow/window/Window.hpp"
#include "willow/Vulkan/VulkanRoot.hpp"
#include "willow/messaging/MessageSystem.hpp"
namespace wlo::wk{
class VulkanSwapchain: public wlo::MessageSystem::Observer {
   /*
    * Faciliates swapping between the frame that is being presented and the frame(s) that are being written to
    *
    * To do this, it manages the memory for all of the frames, and also tracks the current frame to be displayed,
    * as well as the indices of the frames that can be written to.
    *
    * This serves as a synchronization point between reads and writes.
    * the swapchain currently only supports doublebuffering, since this is the most common mode.
    * */


	public:	
		VulkanSwapchain( wk::VulkanRoot &,wlo::Window&);
        ~VulkanSwapchain();
         void resize() ;
         const std::vector<vk::ImageView>& getSwapSurfaceViews() ;//gets the image view that can be written to
         const vk::ImageView& getDepthImageView();
         vk::Semaphore& getImageAvailableSemaphore();
         vk::Semaphore* getRenderFinishedSemaphore();
         vk::SurfaceKHR Surface();

		vk::SwapchainKHR get();
		vk::Extent2D getSwapSurfaceExtent()const ;
		vk::Format getSwapSurfaceFormat() const;
		void reclaim() ;
private:
	//member variables
	unsigned  short m_curFrameIndex;
	 VulkanRoot & m_root;
	vk::SurfaceKHR m_surface;
	vk::SwapchainKHR m_vkSwapchain;

	vk::Extent2D m_swapSurfaceExtent;
	vk::SurfaceFormatKHR m_swapSurfaceFormat;
	//all objcets required to access frames
	std::vector<vk::Image> m_images;
	std::vector<vk::ImageView> m_imageViews;

	vk::Image m_depthImage;
	vk::ImageView m_depthView;
    vk::DeviceMemory m_depthBufferMemory;
    Window& m_window;
    void initialize();
	//member functions
	void createVkSwapchain();
	void createImageViews();
	void createDepthBuffers();
		//set the member variables for image format, and extent	
	void chooseSwapSurfaceFormat();
	void 		   chooseSwapChainExtent();
	//helper functions
	vk::PresentModeKHR   choosePresentMode();

};

}

#pragma once
#include<vulkan/vulkan.hpp>
#include "willow/window/window.hpp"
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
		VulkanSwapchain( wk::VulkanRoot &,wlo::SharedPointer<Window>);
        ~VulkanSwapchain();
        const vk::ImageView& getCurView()const ;//gets the image view that can be written to

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
	std::vector<vk::Framebuffer> m_frameBuffers;

	//member functions
	void createVkSwapchain();
	void getSwapchainImages();
	void createImageViews();
	void createFrameBuffers();
	void createDepthBuffers();
		//set the member variables for image format, and extent	
	void chooseSwapSurfaceFormat();
	void 		   chooseSwapChainExtent();
	//helper functions
	vk::PresentModeKHR   choosePresentMode();
	//message handlers
    void resize(const wlo::WindowResized &) ;

};

}

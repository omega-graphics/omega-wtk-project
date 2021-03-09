#include "../BDCompositionDevice.h"

#define VK_USE_PLATFORM_XLIB_KHR

#include <vulkan/vulkan.hpp>


#ifndef OMEGAWTK_COMPOSITION_VK_VKBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_VK_VKBDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {
    class VKBDCompositionDevice : public BDCompositionDevice {
        vk::UniqueHandle<vk::Instance,vk::DispatchLoaderStatic> vulkanInstance;
    public:
        VKBDCompositionDevice();
        static Core::SharedPtr<BDCompositionDevice> Create();
    };
};

#endif
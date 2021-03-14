#include "../BDCompositionDevice.h"
#include <bits/stdint-uintn.h>

#define VK_USE_PLATFORM_XLIB_KHR

#include <vulkan/vulkan.hpp>


#ifndef OMEGAWTK_COMPOSITION_VK_VKBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_VK_VKBDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {
    template<class _Ty>
    using VKUniqueHandle = vk::UniqueHandle<_Ty,vk::DispatchLoaderStatic>;
    class VKBDCompositionDevice : public BDCompositionDevice {
        vk::PhysicalDevice physicalDevice;
        uint32_t queueFamilyIndex;
    public:
        VKUniqueHandle<vk::Instance> inst;
        VKUniqueHandle<vk::Device> device;
        VKBDCompositionDevice();
        ~VKBDCompositionDevice();
        static Core::SharedPtr<BDCompositionDevice> Create();
        Core::SharedPtr<BDCompositionDeviceContext> createContext();
        Core::SharedPtr<BDCompositionFontFactory> createFontFactory();
    };

    class VKBDCompositionDeviceContext : public BDCompositionDeviceContext {
        VKBDCompositionDevice *device;
        VKUniqueHandle<vk::CommandPool> command_pool;
    public:
        VKBDCompositionDeviceContext(VKBDCompositionDevice *device,VKUniqueHandle<vk::CommandPool> & command_pool);
        static Core::SharedPtr<BDCompositionDeviceContext> Create(VKBDCompositionDevice *device,VKUniqueHandle<vk::CommandPool> & command_pool);
        Core::SharedPtr<BDCompositionVisualTree> createVisualTree();
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect &size);
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img);
        void destroyTarget(Layer *layer, Core::SharedPtr<BDCompositionRenderTarget> &target);
    };
};

#endif
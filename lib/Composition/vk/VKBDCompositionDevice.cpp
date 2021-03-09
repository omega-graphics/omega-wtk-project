#include "VKBDCompositionDevice.h"
#include <vulkan/vulkan.hpp>

namespace OmegaWTK::Composition {
    VKBDCompositionDevice::VKBDCompositionDevice(){
        vk::InstanceCreateInfo info;
        vulkanInstance = vk::createInstanceUnique(info);
    };
    Core::SharedPtr<BDCompositionDevice> VKBDCompositionDevice::Create(){
        return std::make_shared<VKBDCompositionDevice>();
    };
}
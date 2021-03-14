#include "VKBDCompositionDevice.h"

#include <iostream>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace OmegaWTK::Composition {
    VKBDCompositionDevice::VKBDCompositionDevice():physicalDevice(nullptr){
        vk::InstanceCreateInfo c_info;
        Core::Vector<const char *> extNames;
        Core::Vector<const char *> layerNames;
        auto extensionsProps = vk::enumerateInstanceExtensionProperties();
        auto layerProps = vk::enumerateInstanceLayerProperties();
        for(auto & extprop : extensionsProps){
            extNames.push_back(extprop.extensionName.data());
        };
        for(auto & layerprop : layerProps){
            extNames.push_back(layerprop.layerName.data());
        };

        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = "OmegaWTK App";
        appInfo.pEngineName = "Omega UI Graphics Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = vk::enumerateInstanceVersion();
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);


        c_info.enabledExtensionCount = extensionsProps.size();
        c_info.enabledLayerCount = layerProps.size();
        c_info.ppEnabledExtensionNames = extNames.data();
        c_info.ppEnabledLayerNames = layerNames.data();
        c_info.pApplicationInfo = &appInfo;

        inst = vk::createInstanceUnique(c_info);
        auto physicalDevices = inst->enumeratePhysicalDevices();
        if(physicalDevices.size() == 0){
            std::cerr << "Failed to enumerate GPU devices.. exiting" << std::endl;
            exit(1);
        };


        /// Find Suitable Physical Device
        for(auto & physicalDevice : physicalDevices){
            if(VkPhysicalDevice(physicalDevice) != VK_NULL_HANDLE){
                auto features = physicalDevice.getFeatures();
                auto props = physicalDevice.getProperties();
                /**
                    Before creating the main device, 
                    the current physical device must pass all of the following checks:

                    1 - Is it a Discreate GPU?
                    2 - Does it support Geometric Shading.
                    3 - Can it perform Tessalation.
                    4 
                */
                if(props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu 
                && features.geometryShader && features.tessellationShader){
                    this->physicalDevice = physicalDevice;
                    break;
                };
                
            };
        };

        if(!physicalDevice){
            std::cerr << "Failed to acquire GPU that supports Vulkan... Exiting.." << std::endl;
            exit(1);
        };

        queueFamilyIndex = 0;
        auto queueFamilyProps = physicalDevice.getQueueFamilyProperties();
        for(auto &q_family_prop : queueFamilyProps){
            if((q_family_prop.queueFlags & vk::QueueFlagBits::eGraphics) && (q_family_prop.queueFlags & vk::QueueFlagBits::eCompute)){
                ++queueFamilyIndex;
            };
        };

        
        // vk::DeviceCreateInfo dev_info;
        // vk::DeviceQueueCreateInfo queue_info;
        // queue_info.flags = {};
        // dev_info.pQueueCreateInfos = &queue_info;
        // dev_info.queueCreateInfoCount = 1;

        device = physicalDevice.createDeviceUnique({});
        if(!device){
            std::cerr << "Failed to create Device.. Exiting.." << std::endl;
            exit(1);
        };


    };
    Core::SharedPtr<BDCompositionDevice> VKBDCompositionDevice::Create(){
        return std::make_shared<VKBDCompositionDevice>();
    };
    Core::SharedPtr<BDCompositionDeviceContext> VKBDCompositionDevice::createContext(){
        vk::CommandPoolCreateInfo commandPoolDesc;
        commandPoolDesc.flags = {};
        commandPoolDesc.queueFamilyIndex = queueFamilyIndex;
        auto commandPool = device->createCommandPoolUnique(commandPoolDesc);
        if(commandPool){
            return VKBDCompositionDeviceContext::Create(this,commandPool);
        }
        else {
            return nullptr;
        };
    };
    VKBDCompositionDevice::~VKBDCompositionDevice(){
        inst->destroy();
    };

    VKBDCompositionDeviceContext::VKBDCompositionDeviceContext(VKBDCompositionDevice *device, VKUniqueHandle<vk::CommandPool> &command_pool):device(device),command_pool(std::move(command_pool)){};

    Core::SharedPtr<BDCompositionImageRenderTarget> VKBDCompositionDeviceContext::makeImageRenderTarget(Core::Rect & rect){
        
    };

    Core::SharedPtr<BDCompositionDeviceContext> VKBDCompositionDeviceContext::Create(VKBDCompositionDevice *device, VKUniqueHandle<vk::CommandPool> &command_pool){
        return std::make_shared<VKBDCompositionDeviceContext>(device,command_pool);
    };
}
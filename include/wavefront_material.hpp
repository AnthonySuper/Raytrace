#pragma once
#include <globals.hpp>
#include <material.hpp>
#define cimg_display 0
#define cimg_use_png 1
#include <CImg.h>


namespace NM {
    class WavefrontMaterial {
        
        
    public:
        Material originalMaterial;
        
        using ExtImage = cimg_library::CImg<>;
        
        std::unique_ptr<ExtImage> m_kd = nullptr;
        
        
    };
    
    using WavefrontMaterialLibrary = std::map<
    std::string,
    std::shared_ptr<WavefrontMaterial>
    >;
}

#pragma once
#include <globals.hpp>
#include <unordered_map>
#include <vec4.hpp>
#include <errors.hpp>
#include <istream>
#include <string>
#include <memory>
#include <sstream>

namespace NM {
    struct Material {
        enum class IlluminationMode {
            ColorNoAmbient = 0,
            ColorAmbient = 1,
            Highlight = 2,
            ReflectionRayTrace = 3,
            TransparencyGlassRayTrace = 4,
            FresenelReflectionRayTrace = 5,
            RefractionNoFresnelRayTrace = 6,
            RefractionFresnelRayTrace = 7,
            ReflectionNoRayTrace = 8,
            GlassOffNoRayTrace = 9,
            InvisibleShadows = 10
        };
        Vec4 ambient;
        Vec4 diffuse;
        Vec4 specular;
        FloatType specularExpon;
        FloatType dissolve;
        IlluminationMode illum;
    };

    using MaterialLibrary = std::unordered_map<std::string, Material>;
    
    
}

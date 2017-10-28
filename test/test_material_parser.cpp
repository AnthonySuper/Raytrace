#include <catch.hpp>
#include <wavefront_material_parser.hpp>

TEST_CASE("Reading one material") {
    std::istringstream is("newmtl None\n"
                          "Ns 0\n"
                          "Ka 0 0 0\n"
                          "Kd 0.8 0.8 0.8\n"
                          "Ks 0.8 0.8 0.8\n"
                          "d 1\n"
                          "illum 2\n");
    NM::WavefrontMaterialParser parser(is, "test.mtl");
    SECTION("It doesn't throw") {
        REQUIRE_NOTHROW(parser.parse());
    }
    
    SECTION("After parsing") {
        auto lib = parser.parse();
        
        SECTION("It has one material") {
            REQUIRE(lib->size() == 1);
        }
        
        SECTION("Material name is None") {
            REQUIRE(lib->find("None") != lib->end());
        }
        
        SECTION("It has the right properties") {
            auto ptr = lib->at("None");
            REQUIRE(ptr != nullptr);
            auto f = *ptr;
            NM::Vec4 zero{0, 0, 0};
            NM::Vec4 others{0.8, 0.8, 0.8};
            REQUIRE(f.ambient == zero);
            REQUIRE(f.diffuse == others);
            REQUIRE(f.specular == others);
            REQUIRE(f.dissolve == 1.0);
            REQUIRE(f.illum == NM::Material::IlluminationMode::Highlight);
        }
    }
}

TEST_CASE("Reading two materials") {
    std::istringstream is("newmtl None\n"
                          "Ns 0\n"
                          "Ka 0 0 0\n"
                          "Kd 0.8 0.8 0.8\n"
                          "Ks 0.8 0.8 0.8\n"
                          "d 1\n"
                          "illum 2\n\n\n"
                          "newmtl Some\n"
                          "Ns 0\n"
                          "Ka 0 0 0\n"
                          "Kd 0.2 0.2 0.2\n"
                          "Ks 0.0 0.0 0.0\n"
                          "d 1\n"
                          "illum 3\n");
    NM::WavefrontMaterialParser parser(is, "test.mtl");
    
    SECTION("It parses without throwing") {
        REQUIRE_NOTHROW(parser.parse());
    }
    
    SECTION("It parses correctly") {
        auto lib = parser.parse();
        
        SECTION("It has two materials") {
            REQUIRE(lib->size() == 2);
        }
    }
}

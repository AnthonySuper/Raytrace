#include <model.hpp>

namespace NM {
    
    Model Model::WavefrontParser::fromStream(std::istream &stream) {
        WavefrontParser toRet;
        std::string line;
        std::string header;
        std::string lastMaterial;
        while(std::getline(stream, line)) {
            std::istringstream lineStream(line);
            header.clear();
            lineStream >> header;
            if(header == "v" || header == "vn") {
                Vec4 vec{0, 0, 0};
                lineStream >> vec;
                if(header == "v") {
                    toRet.points.push_back(vec);
                }
                else {
                    toRet.normals.push_back(vec);
                }
            }
            else if(header == "f") {
                FaceDescriptor fe;
                lineStream >> fe;
                fe.materialName = lastMaterial;
                toRet.faces.emplace_back(fe);
            }
            else if(header == "usemtl") {
                lineStream >> lastMaterial;
            }
            else if(header == "mtllib") {
                std::string fname;
                lineStream >> fname;
                std::ifstream ifs(fname);
                WavefrontMaterialParser parser(ifs);
                toRet.materials = *(parser.parse());
            }
            else {
                continue;
            }
        }
        return toRet.toModel();
    }
    
    Model Model::WavefrontParser::toModel() {
        Model toReturn;
        for(auto &fd : faces) {
            Triangle points{
                coordinateToPoint(fd[0].coordIdx),
                coordinateToPoint(fd[1].coordIdx),
                coordinateToPoint(fd[2].coordIdx)
            };
            auto mtl = getMaterial(fd.materialName);
            /**
             TODO:
             FIX THE FACK THE NORMALS CAN BE FUCKED
             CLEAN UP THIS CODE IN GENERLA YOU GODDAMN TROGLEDYTE
             */
            if(fd[0].normIdx != 0) {
                Triangle normals {
                    coordinateToNormal(fd[0].normIdx),
                    coordinateToNormal(fd[1].normIdx),
                    coordinateToNormal(fd[2].normIdx)
                };
                
                toReturn.faces.emplace_back(points, normals, mtl);
            }
            else {
                toReturn.faces.emplace_back(points,
                                            Triangle{{}, {}, {}},
                                            mtl);
            }
        }
        toReturn.materials = materials;
        return toReturn;
    }
    
    
    ssize_t Model::WavefrontParser::wfToC(FaceElement::ElmType e) {
        return (e - 1);
    }
    
    Vec4 Model::WavefrontParser::coordinateToPoint(FaceElement::ElmType e) {
        return points.at(wfToC(e));
    }
    
    Vec4 Model::WavefrontParser::coordinateToNormal(FaceElement::ElmType e) {
        return normals.at(wfToC(e));
    }
    
    Model::WavefrontParser::MtlPtr
    Model::WavefrontParser::getMaterial(const std::string& key) {
        auto f = materials.find(key);
        auto itr = f != materials.end() ? f : materials.begin();
        if(itr == materials.end()) {
            return std::make_shared<Material>();
        }
        else {
            return (*itr).second;
        }
    }
    
    std::string Model::print() {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
    
    std::istream& operator>>(std::istream& is, Model::WavefrontParser::FaceElement& fe) {
        Model::WavefrontParser::FaceElement::ElmType c = 0, t = 0, n = 0;
        is >> c;
        if(is.bad() || c == 0) {
            throw ParseError("Couldn't parse texture coordinate");
        }
        char ch = is.peek();
        if(ch == '/') {
            is.get();
            ch = is.peek();
            if(ch == '/') {
                is.get();
                is >> n;
            }
            else {
                is >> t;
                ch = is.peek();
                if(ch == '/') {
                    is.get();
                    is >> n;
                }
            }
        }
        fe.coordIdx = c;
        fe.texIdx = t;
        fe.normIdx = n;
        return is;
    }
    
    Model Model::fromStream(std::istream &is) {
        return WavefrontParser::fromStream(is);
    }
    
    std::istream& operator>>(std::istream& is, Model::WavefrontParser::FaceDescriptor& fd) {
        is >> fd[0];
        is >> fd[1];
        is >> fd[2];
        return is;
    }
    
    std::ostream& operator<<(std::ostream& os, const Model::WavefrontParser::FaceElement& fe) {
        return os << "[" <<
            fe.coordIdx << ","
            << fe.texIdx << ","
            << fe.normIdx << "]";
    }
    
    std::ostream& operator<<(std::ostream& os, const Model::WavefrontParser::FaceDescriptor& fd) {
        os << "{";
        for(int i = 0; i < 3; ++i) {
            os << fd[i];
            if(i != 2) os << ",";
        }
        return os << "}";
    }
    
    std::ostream& operator<<(std::ostream& os, const Model& m) {
        os << "{Model ";
        os << m.faces.size() << " faces, ";
        os << m.materials.size() << " materials: ";
        for(const auto& p : m.materials) {
            os << "['" << p.first << ":\t" << *p.second << "]";
            os << std::endl;
        }
        os << "}";
        return os;
    }
    
    RayIntersection Model::checkIntersection(const NM::Ray & r) const {
        Material useMaterial;
        RayIntersection toRet;

        ssize_t intersectedIdx = -1;
        for(size_t i = 0; i < faces.size(); ++i) {
            auto& face = faces[i];
            const auto& tr = face.tri;
            const auto res = tr.checkIntersection(r);
            if(toRet.compareExchange(res)) {
                intersectedIdx = i;
            }
        }
        if(! toRet) return toRet;
        auto& intersectedFace = faces.at(intersectedIdx);
        toRet.material = intersectedFace.material.get();
        toRet.assignNormal(intersectedFace.calcNormal(toRet));
        return toRet;
    }
    
    Vec4 Model::Face::calcNormal(const NM::RayIntersection &ri) const {
        Vec4 norm = tri.normal;
        if(ri.originalRay().direction.cross(norm) < 0) {
            norm = -norm;
        }
        return norm;
        /*
        Vec4 baycentric;
        // Checking again is pretty cheap
        if(! tri.checkIntersection(ri.originalRay(), &baycentric)) {
            throw std::runtime_error("Why am I calculating the normal for a fake intersection?");
        }
        Vec4 norm = normals.a * baycentric[0] + normals.b * baycentric[1] + normals.c * baycentric[2];
        if(norm.dot(ri.point()) < 0) {
            return -norm.toUnit();
        }
        return norm.toUnit();
         */
    }
    
}

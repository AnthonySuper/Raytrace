#include <model.hpp>

namespace NM {
    
    Model Model::fromStream(std::istream &stream) {
        Model toRet;
        std::string line;
        std::shared_ptr<std::string> lastMaterial = nullptr;
        while(std::getline(stream, line)) {
            std::istringstream lineStream(line);
            std::string header;
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
                Model::FaceDescriptor fe;
                if(lastMaterial) {
                    fe.materialPtr = std::move(lastMaterial);
                    lastMaterial = nullptr;
                }
                lineStream >> fe;
                toRet.faces.emplace_back(fe);
            }
            else if(header == "usemtl") {
                lastMaterial = std::make_shared<std::string>();
                lineStream >> *lastMaterial;
            }
            else if(header == "mtllib") {
                std::string fname;
                lineStream >> fname;
                std::ifstream ifs(fname);
                WavefrontMaterialParser parser(ifs);
                toRet.materials = *parser.parse();
            }
            else {
                continue;
            }
        }
        return toRet;
    }
    
    void Model::writeObj(std::ostream &stream) const {
        for(const auto &vec: points) {
            stream << "v ";
            for(int i = 0; i < 4; ++i) {
                stream << vec[i] << " ";
            }
            stream << "\n";
        }
        for(const auto &face: faces) {
            stream << "f ";
            if(face.materialPtr) {
                stream << "usemtl " << *face.materialPtr << std::endl;
            }
            for(const auto& desc: face.elements) {
                stream << desc.coordIdx;
                if(desc.texIdx || desc.normIdx) {
                    stream << "/";
                }
                if(desc.texIdx) stream << desc.texIdx;
                if(desc.normIdx) stream << "/" << desc.normIdx;
                stream << " ";
            }
            stream << "\n";
        }
    }
    
    std::istream& operator>>(std::istream& is, Model::FaceElement& fe) {
        Model::FaceElement::ElmType c = 0, t = 0, n = 0;
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
    
    std::istream& operator>>(std::istream& is, Model::FaceDescriptor& fd) {
        is >> fd.elements[0];
        is >> fd.elements[1];
        is >> fd.elements[2];
        return is;
    }
    
    std::ostream& operator<<(std::ostream& os, const Model::FaceElement& fe) {
        return os << "[" <<
            fe.coordIdx << ","
            << fe.texIdx << ","
            << fe.normIdx << "]";
    }
    
    std::ostream& operator<<(std::ostream& os, const Model::FaceDescriptor& fd) {
        os << "{";
        for(int i = 0; i < 3; ++i) {
            os << fd.elements[i];
            if(i != 2) os << ",";
        }
        return os << "}";
    }
    
    std::ostream& operator<<(std::ostream& os, const Model& m) {
        os << "{\nModel:\n  Verticies (" << m.faces.size() << "):\n";
        for(const auto &v: m.points) {
            os << "\t" << v << "\n";
        }
        os << "  FaceDescriptors (" << m.faces.size() << "):\n";
        for(const auto&v : m.faces) {
            os << "\t" << v << "\n";
        }
        return os << "}";
    }
    
    Model operator*(const Mat4& mat, const Model& model) {
        Model toRet = model;
        for(auto & point: toRet.points) {
            point = (mat * point);
        }
        return toRet;
    }

    void Model::debugCompare(const Model& other, std::ostream& os) {
        if(other.points.size() != points.size()) {
            os << "Size mismatch, BAD BAD BAD";
            throw std::runtime_error("Not allowed");
        }
        for(size_t i = 0; i < other.points.size(); ++i) {
            auto& op = points.at(i);
            auto& tp = other.points.at(i);
            if(! tp.fuzzyEquals(op)) {
                os << "ERROR, LINE " << i << ": Exp " << op << ", got " << tp << std::endl;
            }
        }
    }
    
    RayIntersection Model::checkIntersection(const NM::Ray & r) const {
        Material useMaterial;
        RayIntersection toRet;
        for(size_t i = 0; i < faces.size(); ++i) {
            const auto& face = faces.at(i);
            swapMaterial(face, useMaterial);
            Triangle tri = faceToTriangle(face);
            if(toRet.compareExchange(tri.checkIntersection(r))) {
                toRet.material = useMaterial;
            }
        }
        return toRet;
    }
    
    void Model::swapMaterial(const FaceDescriptor & fd, Material &mat) const {
        if(! fd.materialPtr) return;
        try {
            mat = materials.at(*fd.materialPtr);
        }
        catch(std::out_of_range& r) {
            return;
        }
    }
    
    Triangle Model::faceAt(size_t idx) const {
        const FaceDescriptor &fd = faces.at(idx);
        return faceToTriangle(fd);
    }
    
    Triangle Model::faceToTriangle(const NM::Model::FaceDescriptor &fd) const {
        return {
            pointAtObjCoord(fd.elements[0].coordIdx),
            pointAtObjCoord(fd.elements[1].coordIdx),
            pointAtObjCoord(fd.elements[2].coordIdx)
        };
    }
    
    Vec4 Model::pointAtObjCoord(ssize_t idx) const {
        return points[idx - 1];
        /*
        if(idx == 0) {
            throw std::range_error("Index 0 not allowed!");
        }
        if(idx < 0) {
            idx = faces.size() + idx;
            if(idx < 0) {
                throw std::range_error("Value is too negative");
            }
            return points[idx];
        }
        else {
            return points[idx - 1];
        }
         */
    }
}

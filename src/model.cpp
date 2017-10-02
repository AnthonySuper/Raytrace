#include <model.hpp>

namespace NM {
    
    Model Model::fromStream(std::istream &stream) {
        Model toRet;
        std::string line;
        bool topComment = true;
        while(std::getline(stream, line)) {
            std::istringstream lineStream(line);
            std::string header;
            lineStream >> header;
            if(header == "#" && topComment) {
                toRet.topCommentBlock.push_back(line);
                continue;
            }
            else {
                topComment = false;
            }
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
                lineStream >> fe;
                toRet.faces.push_back(fe);
            }
            else {
                continue;
            }
        }
        toRet.topCommentBlock.emplace_back("# Modified and transformed");
        return toRet;
    }
    
    void Model::writeObj(std::ostream &stream) const {
        for(const auto &comment: topCommentBlock) {
            stream << comment << std::endl;
        }
        for(const auto &vec: points) {
            stream << "v ";
            for(int i = 0; i < 4; ++i) {
                stream << vec[i] << " ";
            }
            stream << "\n";
        }
        for(const auto &face: faces) {
            stream << "f ";
            for(const auto& desc: face) {
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
        if(is.fail() || c == 0) {
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
        is >> fd[0];
        is >> fd[1];
        is >> fd[2];
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
            os << fd[i];
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
}

#include "MeshLoader.hpp"

std::vector<std::string>* jpl::_graphics::_mesh::_split(std::string l, char del){
    std::vector<std::string>* v = new std::vector<std::string>();
    while(!l.empty()){
        std::string s = l.substr(0, l.find(del));
        v->push_back(s);
        l = l.erase(0, s.size()+1);
    }
    return v;
}

jpl::_graphics::_mesh::Mesh* jpl::_graphics::_mesh::parse(std::istream* f){
    if(!f){
        throw jpl::_exception::IllegalArgumentException("File stream is not valid");
    }
    std::vector<std::array<float,3>> vertices;
    std::vector<std::array<float,2>> textures;
    std::vector<std::array<float,3>> normals;
    bool normals_p = false; //Flag for normals presence
    bool textures_p = false;//Flag for textures presence
    unsigned int size = 3;  //Size per point (vertices+textures = 3+2)
    std::vector<float> points;
    unsigned int k = 0;
    for(std::string l; std::getline(*f, l);){
        if (l.empty()){
            continue;
        }
        std::string type = l.substr(0, l.find(' '));    //Get type
        if(     strcmp(type.c_str(), "#") == 0 || 
                strcmp(type.c_str(), "o") == 0 || 
                strcmp(type.c_str(), "mtllib") == 0 || 
                strcmp(type.c_str(), "usemtl") == 0 ){
            continue;   //Skipping useless lines
        }
        std::vector<std::string>* values = _split(l.substr(type.size()+1), ' ');    //Splitting values
        if(strcmp(type.c_str(), "v") == 0){  //Vertex definition
            vertices.push_back({std::stof(values->at(0)), std::stof(values->at(1)), std::stof(values->at(2))});
        }else if(strcmp(type.c_str(), "vt") == 0){   //Texture definition
            if(!textures_p){
                textures_p = true;
                size += 2;
            }
            textures.push_back({std::stof(values->at(0)), std::stof(values->at(1))});
        }else if(strcmp(type.c_str(), "vn") == 0){
            if(!normals_p){
                normals_p = true;
                //size += 3; this line will be decommented once normals will be implemented
            }
            vertices.push_back({std::stof(values->at(0)), std::stof(values->at(1)), std::stof(values->at(2))});
        }else if(strcmp(type.c_str(), "f") == 0){    //Index definition
            for(size_t i = 0; i < values->size(); i++){
                k++;
                std::vector<std::string>* vtn_s = _split(values->at(i), '/'); //Vertex Texture Normal
                int v = std::stoi(vtn_s->at(0));
                int t, n, j = 1;
                if(textures_p){   //Check presence of textures
                    std::string t_str = vtn_s->at(j++);
                    if(!t_str.empty()){
                        t = std::stoi(t_str);
                    }
                }
                if(normals_p){
                    n = std::stoi(vtn_s->at(j));
                }
                //Load vertices into tn
                points.push_back(vertices.at(v-1)[0]);
                points.push_back(vertices.at(v-1)[1]);
                points.push_back(vertices.at(v-1)[2]);
                if(textures_p){
                    points.push_back(textures.at(t-1)[0]);
                    points.push_back(textures.at(t-1)[1]);
                }
                delete vtn_s;
            }
        }
        delete values;
    }
    return new jpl::_graphics::_mesh::Mesh(
        points.data(), k*size, 3, nullptr, 0, size, true
    );
}
#include <Icosahedron/assets/modeling/OBJLoader.h>

ic::Mesh3D *ic::OBJLoader::get_mesh(const std::string &objectFileName) {
    std::ifstream objRead(objectFileName);
    ic::Mesh3D *mesh = new ic::Mesh3D();

    int posSize = 0, 
        tCoordSize = 0, 
        normalSize = 0, 
        faceSize = 0;

    std::vector<ic::Vec3f> vertices;
    std::vector<ic::Vec2f> textureCoords;
    std::vector<ic::Vec3f> normals;

    std::vector<unsigned int> indices;

    if (!objRead.is_open() || objRead.fail()) {
        printf("Couldn't open the .obj file, returning an empty mesh instead.\n");
        return mesh;
    }

    
    // Calculate size
    std::string line;
    std::string referenceToken;
    
    while (std::getline(objRead, line)) {
        if (!line.compare("") || !line.compare(" ")) {
            continue;
        }
    
        std::istringstream stream(line);
        std::string key;
        stream >> key;
        
        if (!key.compare("v")) posSize++;
        else if (!key.compare("vt")) tCoordSize++;
        else if (!key.compare("vn")) normalSize++;
        else if (!key.compare("f")) faceSize++;
    }
    std::cout << faceSize;

    objRead.close();
    
    std::vector<ic::Vec3f> posReference(posSize);
    std::vector<ic::Vec2f> tCoordReference(tCoordSize);
    std::vector<ic::Vec3f> normalReference(normalSize);

    std::vector<std::string> points(faceSize * 3);

    int posAmount = 0, 
        tCoordAmount = 0, 
        normalAmount = 0, 
        faceAmount = 0,
        referenceAmount = 0;
    

    // Get reference lists
    objRead.open(objectFileName);
    while (std::getline(objRead, line)) {
        if (!line.compare("") || !line.compare(" ")) {
            continue;
        }
    
        std::istringstream stream(line);
        std::string key;
        stream >> key;
        
        if (!key.compare("v")) {
            float x, y, z;
            stream >> x >> y >> z;

            ic::Vec3f position = { x, y, z };
            posReference[posAmount++] = position;
        }
        else if (!key.compare("vt")) {
            float u, v;
            stream >> u >> v;
            
            ic::Vec2f textureCoords = { u, v };
            tCoordReference[tCoordAmount++] = textureCoords;
        }
        else if (!key.compare("vn")) {
            float x, y, z;
            stream >> x >> y >> z;

            ic::Vec3f normal = { x, y, z };
            normalReference[normalAmount++] = normal;
        }
        else if (!key.compare("f")) {
            line.erase(0, 2);
            std::istringstream faceStream(line);
            while (std::getline(faceStream, referenceToken, ' ')) {
                points[faceAmount++] = referenceToken;
            }
        }
    }
    objRead.close();

    std::vector<std::string> pointReference;
    pointReference = points;
    std::sort(pointReference.begin(), pointReference.end());
    std::vector<std::string>::iterator pointReferenceIterator = std::unique(pointReference.begin(), pointReference.end());
    pointReference.erase(pointReferenceIterator, pointReference.end());

    indices.resize(points.size());

    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < pointReference.size(); j++) {
            if (pointReference[j] == points[i]) indices[i] = j;
        }
    }

    std::vector<int> indexReference(pointReference.size() * 3);

    for (int i = 0; i < pointReference.size(); i++) {
        std::istringstream stream(pointReference[i]);
        while (std::getline(stream, referenceToken, '/')) {
          indexReference[referenceAmount++] = std::stoi(referenceToken);
        }
        stream.clear();
    }

    vertices.resize(indexReference.size() / 3);
    textureCoords.resize(indexReference.size() / 3);
    normals.resize(indexReference.size() / 3);

    referenceAmount = 0;
    
    for (int i = 0; i < indexReference.size() / 3; i++) {
        vertices[i] = posReference[indexReference[referenceAmount++] - 1];
        textureCoords[i] = tCoordReference[indexReference[referenceAmount++] - 1];
        normals[i] = normalReference[indexReference[referenceAmount++] - 1];
    }

    mesh->add_attribute("position", vertices);
    mesh->jump_attribute();
    mesh->add_attribute("textureCoords", textureCoords);
    mesh->add_attribute("normal", normals);

    mesh->set_index_buffer(indices);

    mesh->unuse_attribute_definitions();

    return mesh;
}


std::map<std::string, ic::OBJMaterialInfo> ic::OBJLoader::get_materials(const std::string &materialFileName) {
    std::ifstream mtlRead(materialFileName);
    std::map<std::string, ic::OBJMaterialInfo> materials;

    if (!mtlRead.is_open() || mtlRead.fail()) {
        printf("Couldn't open the .mtl file.\n");
        return materials;
    }   
    
    std::string line; 
    while (std::getline(mtlRead, line)) {
        if (!line.compare("") || !line.compare(" ")) {
            continue;
        }
    
        std::istringstream stream(line);
        std::string key;
        std::string materialName;
        stream >> key;
        
        // Define a new material
        if (!key.compare("newmtl")) {
            ic::OBJMaterialInfo material;
            stream >> materialName;

            materials[materialName] = material;
        }
        // Ambient color
        else if (!key.compare("Ka")) {
            ic::Vec3f ambient;
            stream >> ambient[0] >> ambient[1] >> ambient[2];

            materials[materialName].ambient = ambient;
        }
        // Diffuse color
        else if (!key.compare("Kd")) {
            ic::Vec3f diffuse;
            stream >> diffuse[0] >> diffuse[1] >> diffuse[2];

            materials[materialName].diffuse = diffuse;
        }
        // Specular color
        else if (!key.compare("Ks")) {
            ic::Vec3f specular;
            stream >> specular[0] >> specular[1] >> specular[2];

            materials[materialName].specular = specular;
        }
        // Shininess
        else if (!key.compare("Ns")) {
            float shininess;
            stream >> shininess;

            materials[materialName].shininess = shininess;
        }
    }

    return materials;
}
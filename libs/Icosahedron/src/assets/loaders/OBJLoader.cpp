#include <Icosahedron/assets/loaders/OBJLoader.h>

///////////////////////////
///// Object geometry /////
///////////////////////////

ic::Mesh3D ic::OBJLoader::load(const std::string &objectFileName) {

    std::ifstream objRead(objectFileName);

    if (!objRead.is_open() || objRead.fail()) {
        printf("Couldn't open the .obj file, returning an empty mesh instead.\n");
        return ic::Mesh3D();
    }

    
    // Calculate size
    ic::OBJSizes sizes = calculate_size(objRead);
    objRead.close();

    // Attribute references
    objRead.open(objectFileName);
    ic::OBJAttributeReferences references = get_attribute_references(objRead, sizes);
    objRead.close();

    // Now try to convert to indices and geometric data
    ic::OBJGeometricData data = get_geometric_data(references);

    ic::Mesh3D mesh;
    mesh.add_attribute(0, data.vertices);
    mesh.add_attribute(2, data.textureCoords);
    mesh.add_attribute(3, data.normals);
    mesh.set_index_buffer(data.indices);

    mesh.unuse_attribute_definitions();

    return mesh;
}


namespace ic::OBJLoader { namespace {
ic::OBJSizes calculate_size(std::ifstream &objRead) {
    int posSize = 0, 
        tCoordSize = 0, 
        normalSize = 0, 
        faceSize = 0;
    
    std::string line;
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

    ic::OBJSizes result;
    result.posSize = posSize;
    result.tCoordSize = tCoordSize;
    result.normalSize = normalSize;
    result.faceSize = faceSize;

    return result;
}

ic::OBJAttributeReferences get_attribute_references(std::ifstream &objRead, const ic::OBJSizes &sizes) {
    ic::OBJAttributeReferences result;
    
    result.posReference.resize(sizes.posSize);
    result.tCoordReference.resize(sizes.tCoordSize);
    result.normalReference.resize(sizes.normalSize);
    result.points.resize(sizes.faceSize * 3);
    

    int posAmount = 0, 
        tCoordAmount = 0, 
        normalAmount = 0, 
        faceAmount = 0;
    
    // Get reference lists
    std::string line, referenceToken;
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
            result.posReference[posAmount++] = position;
        }
        else if (!key.compare("vt")) {
            float u, v;
            stream >> u >> v;
            
            ic::Vec2f textureCoords = { u, v };
            result.tCoordReference[tCoordAmount++] = textureCoords;
        }
        else if (!key.compare("vn")) {
            float x, y, z;
            stream >> x >> y >> z;

            ic::Vec3f normal = { x, y, z };
            result.normalReference[normalAmount++] = normal;
        }
        else if (!key.compare("f")) {
            line.erase(0, 2);
            std::istringstream faceStream(line);

            while (std::getline(faceStream, referenceToken, ' ')) {
                result.points[faceAmount++] = referenceToken;
            }
        }
    }
    

    return result;
}

ic::OBJGeometricData get_geometric_data(const ic::OBJAttributeReferences &reference) {
    ic::OBJGeometricData data;

    std::vector<std::string> pointReference = reference.points;
    std::sort(pointReference.begin(), pointReference.end());

    std::vector<std::string>::iterator pointReferenceIterator = std::unique(pointReference.begin(), pointReference.end());
    pointReference.erase(pointReferenceIterator, pointReference.end());

    data.indices.resize(reference.points.size());
    for (int i = 0; i < reference.points.size(); i++) {
        for (int j = 0; j < pointReference.size(); j++) {
            if (pointReference[j] == reference.points[i]) data.indices[i] = j;
        }
    }



    std::vector<int> indexReference(pointReference.size() * 3);

    int referenceAmount = 0;
    std::string referenceToken;

    // Convert from string to indices
    for (int i = 0; i < pointReference.size(); i++) {
        std::istringstream stream(pointReference[i]);
        while (std::getline(stream, referenceToken, '/')) {
            indexReference[referenceAmount++] = std::stoi(referenceToken);
        }
        stream.clear();
    }


    // Now actually obtain attributes out of references
    std::cout << "Index reference size: " << indexReference.size() / 3 << "\n";
    
    data.vertices.resize(indexReference.size() / 3);
    data.textureCoords.resize(indexReference.size() / 3);
    data.normals.resize(indexReference.size() / 3);

    
    referenceAmount = 0;
    for (int i = 0; i < indexReference.size() / 3; i++) {
        data.vertices[i] = 
        reference.posReference[indexReference[referenceAmount++] - 1];

        data.textureCoords[i] = 
        reference.tCoordReference[indexReference[referenceAmount++] - 1];
        
        data.normals[i] = 
        reference.normalReference[indexReference[referenceAmount++] - 1];
    }

    
    return data;
}



/////////////////////
///// Materials /////
/////////////////////

std::map<std::string, ic::OBJMaterialInfo> get_materials(const std::string &materialFileName) {
    std::ifstream mtlRead(materialFileName);
    std::map<std::string, ic::OBJMaterialInfo> materials;

    if (!mtlRead.is_open() || mtlRead.fail()) {
        printf("Couldn't open the .mtl file.\n");
        return materials;
    }   
    
    ic::OBJMaterialInfo material;
    std::string materialName;
               
    std::string line; 
    while (std::getline(mtlRead, line)) {
        if (!line.compare("") || !line.compare(" ")) {
            continue;
        }
    
        std::istringstream stream(line);
        std::string key;
        
        stream >> key;
        
        // Define a new material
        if (!key.compare("newmtl")) {
            stream >> materialName;
        }
        // Ambient color
        else if (!key.compare("Ka")) {
            ic::Vec3f ambient;
            stream >> ambient[0] >> ambient[1] >> ambient[2];

            material.ambient = ambient;
        }
        // Diffuse color
        else if (!key.compare("Kd")) {
            ic::Vec3f diffuse;
            stream >> diffuse[0] >> diffuse[1] >> diffuse[2];
            
            material.diffuse = diffuse;
        }
        // Specular color
        else if (!key.compare("Ks")) {
            ic::Vec3f specular;
            stream >> specular[0] >> specular[1] >> specular[2];

            material.specular = specular;
        }
        // Shininess
        else if (!key.compare("Ns")) {
            float shininess;
            stream >> shininess;

            material.shininess = shininess;
        }
    }
    materials[materialName] = material;

    return materials;
}
}}
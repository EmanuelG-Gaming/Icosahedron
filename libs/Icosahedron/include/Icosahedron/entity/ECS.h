#ifndef IC_ECS_H
#define IC_ECS_H

#include <array>
#include <bitset>
#include <iostream>


namespace ic {
    using ComponentTypeIndex = std::size_t;
    using EntityTypeIndex = std::size_t;

    const std::size_t MAX_COMPONENTS = 16;
    EntityTypeIndex lastEntityIndex = 0;

    class Entity;
    class Component;


    ComponentTypeIndex get_component_index() {
        static ComponentTypeIndex lastIndex = 0;

        lastIndex++;

        return lastIndex;
    }

    template <typename T>
    ComponentTypeIndex get_component_type_index() {
        static_assert(std::is_base_of<Component, T>::value, "Component error.");
        static const ComponentTypeIndex type = get_component_index();
        return type;
    }

    std::size_t add_entity() {
        static EntityTypeIndex entities = 0;

        entities++;
        lastEntityIndex = entities;
    
        return entities;
    }

}

#endif
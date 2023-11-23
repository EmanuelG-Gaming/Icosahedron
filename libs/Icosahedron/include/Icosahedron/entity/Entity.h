#ifndef IC_ENTITY_H
#define IC_ENTITY_H

#include <vector>
#include <memory>

#include <Icosahedron/entity/ECS.h>
#include <Icosahedron/entity/Component.h>


namespace ic {
    class Entity {
        public:
            ic::EntityTypeIndex index;

            Entity() {
                this->index = 0;
            }

            template <typename T, typename... TemplateArguments>
            T& add(TemplateArguments&&... arguments) {
                T *component = new T(std::forward<TemplateArguments>(arguments)...);
                component->entity = this;

                std::unique_ptr<ic::Component> pointer{component};

                std::size_t index = get_component_type_index<T>();
                componentArray[index] = component;
                componentSet[index] = 0x01;

                components.emplace_back(std::move(pointer));

                return *component;
            }

            template <typename T>
            T& get() {
                ic::Component *comp = componentArray[get_component_type_index<T>()];
                return *(static_cast<T*>(comp));
            }

            template <typename T>
            bool has() {
                return componentSet[get_component_type_index<T>()];
            }

        private:
            std::bitset<ic::MAX_COMPONENTS> componentSet;
            std::array<ic::Component*, ic::MAX_COMPONENTS> componentArray;
            std::vector<std::unique_ptr<ic::Component>> components;
    };
}
#endif
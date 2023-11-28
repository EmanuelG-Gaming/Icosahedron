#ifndef IC_ENTITIES_H
#define IC_ENTITIES_H

#include <vector>
#include <Icosahedron/entity/Entity.h>
#include <Icosahedron/entity/ECS.h>


namespace ic {
    class Entities {
        public:
            Entities() {
                this->startEntityIndex = 0;
                this->addedFirstEntity = false;
            }

            ic::Entity *add_entity() {
                std::size_t index = ic::add_entity();
                if (!this->addedFirstEntity) {
                    this->startEntityIndex = index;
                    this->addedFirstEntity = true;
                }

                ic::Entity *entity = new ic::Entity();
                entity->index = index;
                entities.emplace_back(std::move(entity));

                return entity;
            }

            ic::Entity *get_entity(int index) {
                return entities[index];
            }

            /** @return The global index of the first element of the entity list. */
            std::size_t get_starting_index() {
                return startEntityIndex;
            }
            
        private:
            std::vector<ic::Entity*> entities;
            ic::EntityTypeIndex startEntityIndex;
            bool addedFirstEntity;
    };
}
#endif
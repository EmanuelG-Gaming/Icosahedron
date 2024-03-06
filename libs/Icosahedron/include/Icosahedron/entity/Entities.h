#ifndef IC_ENTITIES_H
#define IC_ENTITIES_H

#include <vector>
#include <Icosahedron/entity/ECS.h>
#include <Icosahedron/entity/Entity.h>
#include <Icosahedron/entity/ECS.h>
#include <algorithm>


namespace ic {
    class Entities {
        public:
            std::vector<ic::Entity*> entities;
            
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

            void remove_entity(ic::Entity *entity) {
                if (!entity) return;

                auto iterator = std::find(entities.begin(), entities.end(), entity);
                if (iterator == entities.end()) return;
            
                entities.erase(iterator);
            }

            ic::Entity *get_entity(int index) {
                return entities[index];
            }

            /** @return The global index of the first element of the entity list. */
            std::size_t get_starting_index() {
                return startEntityIndex;
            }
            
        private:
            ic::EntityTypeIndex startEntityIndex;
            bool addedFirstEntity;
    };
}
#endif
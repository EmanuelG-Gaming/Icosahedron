#ifndef IC_SCENE_GRAPH_H
#define IC_SCENE_GRAPH_H

#include <list>
#include <memory>
#include <algorithm>


namespace ic {
    /** @brief A scene graph is a data structure that allows objects to display inheritance, in a tree-like hierarchy. */
    class SceneGraphNode {
        public:
            std::list<SceneGraphNode*> nodes;
            SceneGraphNode *previous = nullptr;

            virtual ~SceneGraphNode() {
                for (auto &node : nodes) {
                    delete node;
                }
            }
            

            void add_node(ic::SceneGraphNode *node) {
                nodes.emplace_back(node);
                nodes.back()->previous = this;
            }

            void remove_all() {
                for (auto &node : nodes) {
                    node->remove_all();
                }

                nodes.clear();
            }


            void remove_node(ic::SceneGraphNode *node) {
                node->remove_all();
                nodes.remove(node);
            }
    };
}

#endif
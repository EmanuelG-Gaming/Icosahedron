#ifndef IC_SCENE_GRAPH_H
#define IC_SCENE_GRAPH_H

#include <list>
#include <memory>


namespace ic {
    /** @brief A scene graph is a data structure that allows objects to display inheritance, in a tree-like hierarchy. */
    class SceneGraphNode {
        public:
            std::list<SceneGraphNode*> nodes;
            SceneGraphNode *previous = nullptr;

            virtual ~SceneGraphNode() {}
            

            void add_node(ic::SceneGraphNode *node) {
                nodes.emplace_back(node);
                nodes.back()->previous = this;
            }
    };
}

#endif
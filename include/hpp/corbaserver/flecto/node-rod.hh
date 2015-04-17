#ifndef ROD_HH
#define ROD_HH

#include <gepetto/viewer/group-node.h>
#include <gepetto/viewer/leaf-node-capsule.h>


namespace flecto {

    DEF_CLASS_SMART_PTR(NodeRod)

    class NodeRod : public graphics::GroupNode
    {
        private :
            std::list<graphics::LeafNodeCapsulePtr_t> list_of_objects_;

            NodeRodWeakPtr weak_ptr_;
            void initWeakPtr(NodeRodWeakPtr other_weak_ptr);
        protected:
            NodeRod(const std::string& name);
        public:
            static NodeRodPtr_t create(const std::string& name);

    };
} //namespace
#endif // ROD_HH

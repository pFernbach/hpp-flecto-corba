
#include <hpp/corbaserver/flecto/node-rod.hh>

namespace flecto {


    NodeRod::NodeRod (const std::string& name):
        graphics::GroupNode (name), list_of_objects_()
    {
    }

    void NodeRod::initWeakPtr(NodeRodWeakPtr other_weak_ptr)
    {
        weak_ptr_ = other_weak_ptr;
    }


    //------- public :-------


    NodeRodPtr_t NodeRod::create(const std::string& name)
    {
        NodeRodPtr_t shared_ptr(new NodeRod(name));

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }




} // namespace

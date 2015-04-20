
#include <hpp/corbaserver/flecto/node-rod.hh>
namespace hpp{
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


    bool NodeRod::addChild (graphics::LeafNodeCapsulePtr_t child_ptr)
    {
        list_of_objects_.push_back(child_ptr);
        this->asQueue()->addChild(child_ptr->asGroup());
        return true;
    }

    bool NodeRod::removeChild (graphics::LeafNodeCapsulePtr_t child_ptr)
    {
        list_of_objects_.remove(child_ptr);
        return this->asQueue()->removeChild(this->asQueue()->getChildIndex(child_ptr->asGroup()));
    }


    graphics::LeafNodeCapsulePtr_t NodeRod::getChild(size_t i)
    {
       std::list<graphics::LeafNodeCapsulePtr_t>::const_iterator it = list_of_objects_.begin();
       if (list_of_objects_.size() > i)
       {
           std::advance(it, i);
       }
       return *it;
    }

} // namespace flecto
}//namespace flecto

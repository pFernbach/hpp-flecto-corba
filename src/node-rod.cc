
#include <hpp/corbaserver/flecto/node-rod.hh>
namespace hpp{
namespace flecto {


    NodeRod::NodeRod (const std::string& name, osgVector4 color, float radius, float totalLength,int maxCapsule): list_of_capsule_()
    {
        name_ = name;
        radius_ = radius;
        totalLength_ = totalLength;
        color_ = osgVector4(color);
        maxCapsule_=maxCapsule;
        for (int i = 0; i< maxCapsule ; i++){
            std::stringstream nameCap;
            nameCap << name << "/cap"<<i;
            list_of_capsule_.push_back(nameCap.str());
            //std::cout<<"add : "<< nameCap.str();
        }
    }

    void NodeRod::initWeakPtr(NodeRodWeakPtr other_weak_ptr)
    {
        weak_ptr_ = other_weak_ptr;
    }


    //------- public :-------


    NodeRodPtr_t NodeRod::create(const std::string& name,osgVector4 color, float radius, float totalLength,short maxCapsule)
    {
        NodeRodPtr_t shared_ptr(new NodeRod(name,color,radius,totalLength,maxCapsule));

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }



    char* NodeRod::getCapsule(int i)
    {

        const std::string& str = list_of_capsule_.at(i);
        char* name = new char[str.length ()+1];
        strcpy (name, str.c_str ());

        return name;
    }

} // namespace flecto
}//namespace flecto

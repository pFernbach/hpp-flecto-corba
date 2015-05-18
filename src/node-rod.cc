
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

        // Define rod parameters
        qserl::rod3d::Parameters rodParameters;
        rodParameters.radius = 0.01;                            // radius of the rod
        rodParameters.rodModel = qserl::rod3d::Parameters::RM_INEXTENSIBLE;   // in 3D, could be inextensible rod model or extensible and shearable one.
        rodParameters.numNodes = 100;                          // The number of discretized rod nodes N

        rodParameters.stiffnessCoefficients = Eigen::Matrix<double, 6, 1>::Ones();  // stiffness coefficients each deformation axis.
                                                                // if inextensible model is used only the three first are relevant.
                                                                // see documentation for more details.

        // Create the rod
       rod_ = qserl::rod3d::Rod::create(rodParameters);

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

    hpp::floatSeqSeq* NodeRod::convertAtoQ (const hpp::floatSeq& a) {
        hpp::floatSeqSeq* q;
        q = new hpp::floatSeqSeq;
        q->length(maxCapsule_);


        // Define coordinates in A-space for the rod (its parameterization)...
        Eigen::Wrenchd baseWrench;
        baseWrench.tx() = a[0];
        baseWrench.ty() = a[1];
        baseWrench.tz() = a[2];
        baseWrench.fx() = a[3];
        baseWrench.fy() = a[4];
        baseWrench.fz() = a[5];

        std::cout<<"test convert A to Q : base wrench = "<<baseWrench.tx()<<" , "<<baseWrench.ty()<<" , "<<baseWrench.tz()<<" , "<<baseWrench.fx()<<" , "<<baseWrench.fy()<<" , "<<baseWrench.fz()<<std::endl;


        // ... the rod base position (here identity element of SE(3) ) ...
        const Eigen::Displacementd rodBasePosition(Eigen::Displacementd::Identity());

        // ... some integration options if needed ...
        // for example, here we want in addition to the rod geometry its internal wrenches
        // and Jacobian matrices (dq / da).
        qserl::rod3d::WorkspaceIntegratedState::IntegrationOptions integrationOptions;
        integrationOptions.keepMuValues = false;
        integrationOptions.keepJMatrices = false;

        // ... and compute corresponding configuration
        // note that the given coordinates must not be singular (otherwise the returned
        // boolean isNotSingular will be false). See Rod::isConfigurationSingular() for details.
        qserl::rod3d::WorkspaceIntegratedState::IntegrationResultT intResult = rod_->integrateStateFromBaseWrench(baseWrench,      rodBasePosition, integrationOptions);

        if (intResult != qserl::rod3d::WorkspaceIntegratedState::IR_SINGULAR)
        {
          qserl::rod3d::WorkspaceIntegratedStateShPtr integratedState1 = rod_->integratedState();

          // here we can access to rod geometry (and more as wrenches as we defined some specific
          // integration options

          // For example, we can express the rod geometry (node by node) in the global frame
          // instead of rod base frame this way
          // Note this could be also directly given by the method WorkspaceState::nodesAbsolute6DPositions()
          for (std::vector<Eigen::Displacementd>::const_iterator itNode = integratedState1->nodes().begin();
            itNode != integratedState1->nodes().end(); ++itNode)
          {
            const Eigen::Displacementd nodeAbsolutePosition = rodBasePosition * (*itNode);
            // ... do something with the current rod node position ...
/* <<"["<<(1./rodParameters.numNodes)<<","<<nodeAbsolutePosition.x()<<","<<nodeAbsolutePosition.y()<<","<<nodeAbsolutePosition.z()<<","<<nodeAbsolutePosition.qw()<<","<<nodeAbsolutePosition.qx()<<","<<nodeAbsolutePosition.qy()<<","<<nodeAbsolutePosition.qz()<<"],";*/


          }


        }

        return q;
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


#include <hpp/corbaserver/flecto/node-rod.hh>

namespace hpp{
namespace flecto {


  NodeRod::NodeRod (const std::string& name, osgVector4 color, float radius, float totalLength,int maxCapsule): list_of_capsule_(),rodBasePosition(Eigen::Displacementd::Identity())
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
        rodParameters.radius = radius;                            // radius of the rod
        rodParameters.rodModel = qserl::rod3d::Parameters::RM_INEXTENSIBLE;   // in 3D, could be inextensible rod model or extensible and shearable one.
        rodParameters.numNodes = maxCapsule;                          // The number of discretized rod nodes N

        rodParameters.stiffnessCoefficients = Eigen::Matrix<double, 6, 1>::Ones();  // stiffness coefficients each deformation axis.
                                                                // if inextensible model is used only the three first are relevant.
                                                                // see documentation for more details.

        // Create the rod
       rod_ = qserl::rod3d::Rod::create(rodParameters);
       // ... some integration options if needed ...
       // for example, here we want in addition to the rod geometry its internal wrenches
       // and Jacobian matrices (dq / da).
       integrationOptions.keepMuValues = false;
       integrationOptions.keepJMatrices = false;

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

    hpp::floatSeqSeq* NodeRod::wrenchToGeometry (const hpp::floatSeq& a) {
        hpp::floatSeqSeq* q;
        q = new hpp::floatSeqSeq;
        CORBA::ULong size = (CORBA::ULong) 8; // we always have 8 parameter per capsules (size,pos(3),quat(w,x,y,z))


        // Define coordinates in A-space for the rod (its parameterization)...
        Eigen::Wrenchd baseWrench;
        baseWrench.tx() = a[0];
        baseWrench.ty() = a[1];
        baseWrench.tz() = a[2];
        baseWrench.fx() = a[3];
        baseWrench.fy() = a[4];
        baseWrench.fz() = a[5];


        // ... and compute corresponding configuration
        // note that the given coordinates must not be singular (otherwise the returned
        // boolean isNotSingular will be false). See Rod::isConfigurationSingular() for details.
        qserl::rod3d::WorkspaceIntegratedState::IntegrationResultT intResult = rod_->integrateStateFromBaseWrench(baseWrench,      rodBasePosition, integrationOptions);

        if (intResult == qserl::rod3d::WorkspaceIntegratedState::IR_VALID)
        {
          qserl::rod3d::WorkspaceIntegratedStateShPtr integratedState1 = rod_->integratedState();
          q->length(maxCapsule_);
          // here we can access to rod geometry (and more as wrenches as we defined some specific
          // integration options

          // For example, we can express the rod geometry (node by node) in the global frame
          // instead of rod base frame this way
          // Note this could be also directly given by the method WorkspaceState::nodesAbsolute6DPositions()

          std::size_t i=0;
          for (std::vector<Eigen::Displacementd>::const_iterator itNode = integratedState1->nodes().begin();
            itNode != integratedState1->nodes().end(); ++itNode)
          {
            const Eigen::Displacementd nodeAbsolutePosition = rodBasePosition * (*itNode);
            // ... do something with the current rod node position ...

	   // std::cout<<"test : "<<i<<std::endl;
	    float* paramCapsule = hpp::floatSeq::allocbuf(size);
	    hpp::floatSeq floats(size, size, paramCapsule, true);

	    paramCapsule[0] = (totalLength_/(float)maxCapsule_);
	    paramCapsule[1] = nodeAbsolutePosition.x();
	    paramCapsule[2] = nodeAbsolutePosition.y();
	    paramCapsule[3] = nodeAbsolutePosition.z();
	    paramCapsule[4] = nodeAbsolutePosition.qw();
	    paramCapsule[5] = nodeAbsolutePosition.qx();
	    paramCapsule[6] = nodeAbsolutePosition.qy();
	    paramCapsule[7] = nodeAbsolutePosition.qz();
	    (*q) [i] = floats;
	    //std::cout<<"test fin: "<<i<<std::endl;
	    i++;
          }
        }else{  // cas singulier
            q->length(1);
            float* paramCapsule = hpp::floatSeq::allocbuf(size);
            hpp::floatSeq floats (size, size, paramCapsule, true);
            for(std::size_t i = 0; i<8; i++)
              paramCapsule[i]=0;
            (*q)[0] = floats;

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

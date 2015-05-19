#ifndef ROD_HH
#define ROD_HH

#include <gepetto/viewer/group-node.h>
#include <gepetto/viewer/leaf-node-capsule.h>
#include <gepetto/viewer/config-osg.h>
#include <qserl/rod3d/rod.h>
# include "Rod.hh"

namespace hpp{
namespace flecto {

    DEF_CLASS_SMART_PTR(NodeRod)

    class NodeRod
    {
        private :
            std::vector<std::string> list_of_capsule_;
            NodeRodWeakPtr weak_ptr_;
            void initWeakPtr(NodeRodWeakPtr other_weak_ptr);
            osgVector4 color_;
            float radius_;
            float totalLength_;
            short maxCapsule_;
            std::string name_;
            qserl::rod3d::RodShPtr rod_; // object rod from qserl
            qserl::rod3d::WorkspaceIntegratedState::IntegrationOptions integrationOptions;
            const Eigen::Displacementd rodBasePosition;
        protected:
            NodeRod(const std::string& name,osgVector4 color, float radius, float totalLength, int maxCapsule);
        public:
            static NodeRodPtr_t create(const std::string& name,osgVector4 color, float radius, float totalLength,short maxCapsule);

            virtual char *getCapsule(int i);

            virtual hpp::floatSeqSeq* wrenchToGeometry (const hpp::floatSeq& a);


            float radius(){
                return radius_;
            }

            float totalLength(){
                return totalLength_;
            }

            osgVector4 color(){
                return color_;
            }

            short maxCapsule(){
                return maxCapsule_;
            }

    };
} //namespace
} //namespace hpp
#endif // ROD_HH

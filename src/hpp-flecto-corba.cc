

#include <iostream>
#include <hpp/util/debug.hh>
#include <hpp/core/problem-solver.hh>
#include <hpp/corbaserver/server.hh>
#include <hpp/corbaserver/flecto/server.hh>
#include <hpp/model/object-factory.hh>
#include <hpp/model/device.hh>
# include <hpp/fcl/math/matrix_3f.h>

typedef hpp::flecto::Server FlectoServer;
typedef hpp::corbaServer::Server CorbaServer;
typedef hpp::core::ProblemSolverPtr_t ProblemSolverPtr_t;
typedef hpp::core::ProblemSolver ProblemSolver;

int main (int argc, char* argv[])
{
    //Eigen::internal::set_is_malloc_allowed (true);
    hpp::model::ObjectFactory objectFactory;
    ProblemSolverPtr_t problemSolver = new ProblemSolver;
    //create robot :
    hpp::model::DevicePtr_t device (hpp::model::Device::create ("robot_rod"));

    fcl::Transform3f mat;
    mat.setIdentity ();
    // add joints freeflyer
 /*
    hpp::model::JointPtr_t baseJoint, rootJoint; // architecture : baseJoint->So3Joint->RodJoint

    baseJoint = objectFactory.createJointTranslation3 (mat);
    baseJoint->name ("base_joint_xyz");
    baseJoint->lowerBound (0, -std::numeric_limits<double>::infinity());
    baseJoint->upperBound (0, +std::numeric_limits<double>::infinity());
    baseJoint->lowerBound (1, -std::numeric_limits<double>::infinity());
    baseJoint->upperBound (1, +std::numeric_limits<double>::infinity());
    baseJoint->lowerBound (2, -std::numeric_limits<double>::infinity());
    baseJoint->upperBound (2, +std::numeric_limits<double>::infinity());
    device->rootJoint (baseJoint);

    // joint SO3
    rootJoint = objectFactory.createJointSO3 (mat);
    rootJoint->name ("base_joint_SO3");
    baseJoint->addChildJoint (rootJoint);
*/

    // add Joint Anchor :

    hpp::model::JointPtr_t rootJoint;

    rootJoint = objectFactory.createJointAnchor (mat);
    rootJoint->name ("anchorJoint");
    device->rootJoint (rootJoint);


    //add joint rod :
    fcl::Transform3f matRod;
    matRod.setIdentity ();
    hpp::model::JointPtr_t jointRod;
    jointRod = objectFactory.createJointStaticRod(matRod);
    jointRod->name ("Rod_Joint");

    jointRod->isBounded (0, false);
    jointRod->lowerBound(0, -std::numeric_limits <double>::infinity ());
    jointRod->upperBound(0, std::numeric_limits <double>::infinity ());
    rootJoint->addChildJoint(jointRod);



    // Add device to the planner
    problemSolver->robot (device);
    problemSolver->robot ()->controlComputation(hpp::model::Device::JOINT_POSITION);


    CorbaServer corbaServer (problemSolver, argc,const_cast<const char**> (argv), false);
    FlectoServer flectoServer (argc, argv, false);
    //flectoServer.setProblemSolver (problemSolver);

    try {
    corbaServer.startCorbaServer ();
    hppDout (info, "successfully start hpp-corbaserver");
    } catch (const std::exception& exc) {
    hppDout (error, "Failed to start hpp-corbaserver");
    }
    try {
    flectoServer.startCorbaServer ("hpp", "corbaserver",
                "flecto", "robot");

    hppDout (info, "Successfully started corba server for whole body planner");
    } catch (const std::exception& exc) {
    hppDout (error, "failed to start corba server for whole body planner");
    }
    corbaServer.processRequest(true);
}

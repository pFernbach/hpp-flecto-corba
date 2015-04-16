

#include <iostream>
#include <hpp/util/debug.hh>
#include <hpp/core/problem-solver.hh>
#include <hpp/corbaserver/server.hh>
#include <hpp/corbaserver/flecto/server.hh>

typedef hpp::flecto::Server FlectoServer;
typedef hpp::corbaServer::Server CorbaServer;
typedef hpp::core::ProblemSolverPtr_t ProblemSolverPtr_t;
typedef hpp::core::ProblemSolver ProblemSolver;
int
main (int argc, char* argv[])
{
  Eigen::internal::set_is_malloc_allowed (true);
  ProblemSolverPtr_t problemSolver = new ProblemSolver;
  CorbaServer corbaServer (problemSolver, argc,
               const_cast<const char**> (argv), false);
  FlectoServer flectoServer (argc, argv, false);
  flectoServer.setProblemSolver (problemSolver);

  try {
    corbaServer.startCorbaServer ();
    hppDout (info, "successfully start hpp-corbaserver");
  } catch (const std::exception& exc) {
    hppDout (error, "Faile to start hpp-corbaserver");
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

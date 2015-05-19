// Copyright (C) 2011,2012,2013,2014 CNRS-LAAS
// Author: Florent Lamiraux.
//
// This file is part of the hpp-wholebody-step-corba.
//
// hpp-wholebody-step-corba is free software: you can redistribute
// it and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// hpp-wholebody-step-corba is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with hpp-wholebody-step-corba.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef HPP_FLECTO_CORBA_ROD_HH
# define HPP_FLECTO_CORBA_ROD_HH

# include <omniORB4/CORBA.h>
# include <hpp/core/problem-solver.hh>
# include <hpp/corbaserver/flecto/fwd.hh>
# include <hpp/corbaserver/flecto/node-rod.hh>
# include "Rod.hh"

namespace hpp {
  namespace flecto {
    namespace impl {
      typedef osg::Vec3f::value_type value_type;
      class Rod : public virtual POA_hpp::corbaserver::flecto::Rod
      {
          private:
            std::map<std::string,NodeRodPtr_t> rods_; // contient la liste des rod créé et pour chacun la liste des capsules qu'il possède
          public:
            Rod ();
            hpp::floatSeqSeq* wrenchToGeometry (const char *rodNameCorba, const floatSeq &a) throw (hpp::Error);

            virtual bool createRod(const char* rodNameCorba,const value_type *colorCorba,float radius,float totalLength,short maxCapsules)throw (hpp::Error);

            virtual char* getRodCapsule(const char* rodNameCorba,short i) throw (hpp::Error);

            virtual float getRodRadius(const char* rodNameCorba) throw(hpp::Error);

            virtual float getRodTotalLength(const char* rodNameCorba) throw(hpp::Error);

            virtual short getRodMaxCapsule(const char* rodNameCorba) throw(hpp::Error);

            virtual hpp::floatSeq* convertToOSG(const hpp::floatSeq& q, float length) throw(hpp::Error);


           // void setProblemSolver (const ProblemSolverPtr_t& problemSolver);
      }; // class rod
    } // namespace impl
  } // namespace flecto
} // namespace hpp

#endif //HPP_WHOLEBODY_STEP_CORBA_WHOLEBODY_STEP_HH

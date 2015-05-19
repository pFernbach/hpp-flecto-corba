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

#include <cassert>
#include <hpp/util/debug.hh>
#include "rod.impl.hh"
# include <hpp/corbaserver/flecto/node-rod.hh>
#include <gepetto/viewer/corba/windows-manager.h>
#include <Eigen/Geometry>
// include qserl


namespace hpp {
  namespace flecto {
    namespace impl {

    Rod::Rod():rods_()
    {

    }


    //TODO methode avec qSerl :
    hpp::floatSeqSeq* Rod::wrenchToGeometry (const char* rodNameCorba,const hpp::floatSeq& a) throw (hpp::Error)
    {
      try {
          const std::string rodName (rodNameCorba);
          NodeRodPtr_t rod = rods_[rodName];
          return rod->wrenchToGeometry(a);
        // cf problem.impl::nodes() pour utiliser les floatseq
      } catch (const std::exception& exc) {
        throw hpp::Error (exc.what ());
        return 0;
      }
    }

    bool Rod::createRod (const char* rodNameCorba, const value_type *colorCorba, float radius, float totalLength, short maxCapsules)throw (Error)
    {
        try {
            osgVector4 color(colorCorba[0], colorCorba[1], colorCorba[2],colorCorba[3]);
            const std::string rodName (rodNameCorba);
            NodeRodPtr_t rod = NodeRod::create(rodName,color,radius,totalLength,maxCapsules);
            rods_[rodName] = rod;
            return true;
        } catch (const std::exception& exc) {
            throw Error (exc.what ());
            return false;
        }
    }

    /*bool Rod::setDisplayedChild(const char* rodNameCorba, long i) throw (Error){
        const std::string rodName (rodNameCorba);
        if (rods_.find (rodName) == rods_.end ()){
            std::cout << "rod name \"" << rodName <<  "\" doesn't exist." << std::endl;
            return false;
        }
        if ( i > rods_[rodName]->maxCapsule()){
            std::cout  << rodName << "  have only" << rods_[rodName]->maxCapsule() << " capsules "<< std::endl;
            return false;
        }
        rods_[rodName]->setDisplayedChild(i);
        return true;
    }*/


    char* Rod::getRodCapsule(const char* rodNameCorba, short i) throw (hpp::Error){
        const std::string rodName (rodNameCorba);
        if (rods_.find (rodName) == rods_.end ()){
            std::cout << "rod name \"" << rodName <<  "\" doesn't exist." << std::endl;
            return 0;
        }
        return rods_[rodName]->getCapsule(i);
    }

    float Rod::getRodRadius(const char* rodNameCorba) throw(hpp::Error){
        const std::string rodName (rodNameCorba);
        if (rods_.find (rodName) == rods_.end ()){
            std::cout << "rod name \"" << rodName <<  "\" doesn't exist." << std::endl;
            return 0;
        }
        return rods_[rodName]->radius();
    }

    float Rod::getRodTotalLength(const char* rodNameCorba) throw(hpp::Error){
        const std::string rodName (rodNameCorba);
        if (rods_.find (rodName) == rods_.end ()){
            std::cout << "rod name \"" << rodName <<  "\" doesn't exist." << std::endl;
            return 0;
        }
        return rods_[rodName]->totalLength();
    }

    short Rod::getRodMaxCapsule(const char* rodNameCorba) throw(hpp::Error){
        const std::string rodName (rodNameCorba);
        if (rods_.find (rodName) == rods_.end ()){
            std::cout << "rod name \"" << rodName <<  "\" doesn't exist." << std::endl;
            return 0;
        }
        return rods_[rodName]->maxCapsule();
    }

    hpp::floatSeq* Rod::convertToOSG(const hpp::floatSeq& q,float length) throw(hpp::Error){
        std::size_t size = (std::size_t)q.length();
        if (size != 7){
            std::cout<<"You must specify a 7 element vector"<<std::endl;
            return 0;
        }
        Eigen::Quaterniond quat(q[3],q[4],q[5],q[6]);
        Eigen::Matrix< double, 3, 3 > mat = quat.toRotationMatrix() ;
        // on applique une rotation de pi/2 selon l'axe y
        static const double kRotationHalfPiArroundY_data[] = {
        0.0,0.0,-1.0,
        0.0,1.0,0.0,
        1.0,0.0,0.0
        };
        Eigen::Matrix< double, 3, 3 > kRotationHalfPiArroundY (kRotationHalfPiArroundY_data);
        Eigen::Matrix< double, 3, 3 > matosg = mat*kRotationHalfPiArroundY;

        // translation d'une demie longueur car origine OSG = centre de la piece
        static const double transLength[] = {
        0.0,0.0,length/2
        };
        Eigen::Matrix<double,3,1> trans(transLength);
        Eigen::Matrix<double,3,1> transRot = matosg*trans;
        double* transRotD = transRot.data();
        Eigen::Quaterniond quatOsg(matosg);
        hpp::floatSeq* res = new hpp::floatSeq;
        res->length(7);
        for(int i=0;i<3;i++)
            (* res)[i] = q[i] + transRotD[i];
        (*res)[3]=quatOsg.w();
        (*res)[4]=quatOsg.x();
        (*res)[5]=quatOsg.y();
        (*res)[6]=quatOsg.z();
        return res;

    }

    } // namespace impl
  } // namespace flecto
} // namespace hpp

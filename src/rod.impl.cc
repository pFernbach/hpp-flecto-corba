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
// include qserl


namespace hpp {
  namespace flecto {
    namespace impl {
    //todo ?
    Rod::Rod()
    {}


    //TODO methode avec qSerl :
    hpp::floatSeqSeq* Rod::convertAtoQ (const hpp::floatSeq& a) throw (hpp::Error)
    {
      hpp::floatSeqSeq* q;
      try {
          std::cout<<"test interface python"<<std::endl;

        // cf problem.impl::nodes() pour utiliser les floatseq
      } catch (const std::exception& exc) {
        throw hpp::Error (exc.what ());
      }
      return q;
    }

    } // namespace impl
  } // namespace flecto
} // namespace hpp

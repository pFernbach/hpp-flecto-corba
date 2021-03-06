#!/usr/bin/env python
# Copyright (c) 2014 CNRS
# Author: Florent Lamiraux
#
# This file is part of hpp-corbaserver.
# hpp-corbaserver is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-corbaserver is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

from hpp.corbaserver.robot import Robot as Parent
from hpp.corbaserver.flecto.client import Client as flectoClient

##
#  Control of robot "boule" in hpp
#
#  This class implements a client to the corba server implemented in
#  hpp-corbaserver. It derive from class hpp.corbaserver.robot.Robot.
#
#  This class is also used to initialize a client to rviz in order to
#  display configurations of the PR2 robot.
#
#  At creation of an instance, the urdf and srdf files are loaded using
#  idl interface hpp::corbaserver::Robot::loadRobotModel.
class Robot (Parent):
    ##
    #  Information to retrieve urdf and srdf files.
    packageName = "hpp_tutorial"
    ##
    #  Information to retrieve urdf and srdf files.


    def __init__ (self, robotName):
        Parent.__init__ (self, robotName, "anchor", False)
        self.tf_root = 'base_link' # le nom du tf_root est libre
        self.rod = flectoClient().rod
        
        
    # redefinition de la methode, pour prendre en compte le cas d'un rod
    def getLinkPosition (self, jointName):
      if jointName.startswith("Rod") :
        rank = self.rankInConfiguration[jointName]
        q = self.getCurrentConfig ()
        a = q[rank:rank+6] # un joint rod a forcement configSize() = 6
        return self.rod.wrenchToGeometry(self.name,a)
      else : 
        return self.client.robot.getLinkPosition (jointName)
        
        


        
        
        
        

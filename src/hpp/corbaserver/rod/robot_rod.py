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
        
        seq = [[0.0526316,0,0,0,1,0,0,0],[0.0526316,0.0521021,0.00162961,-0.00620368,0.979072,-0.163742,0.115482,0.0362613],[0.0526316,0.101217,0.000423862,-0.0246585,0.914555,-0.325656,0.238637,0.026411],[0.0526316,0.143954,-0.0123251,-0.0521436,0.795413,-0.478732,0.371002,-0.0262116],[0.0526316,0.175719,-0.0425005,-0.0805611,0.607076,-0.604597,0.503456,-0.112781],[0.0526316,0.192138,-0.0888352,-0.0976098,-0.348143,0.675361,-0.613819,0.214351],[0.0526316,0.192955,-0.140463,-0.0923456,0.0470678,-0.668379,0.677552,-0.303116],[0.0526316,0.184438,-0.182258,-0.062753,-0.247632,-0.586202,0.684883,-0.354271],[0.0526316,0.17633,-0.204884,-0.0165788,-0.493769,-0.45658,0.647877,-0.356691],[0.0526316,0.176527,-0.209037,0.0354909,0.677381,0.311278,-0.587918,0.312289],[0.0526316,0.188744,-0.202067,0.0859481,0.803266,0.173677,-0.522674,0.225093],[0.0526316,0.213076,-0.193374,0.131604,0.882507,0.0522691,-0.456652,0.0966938],[0.0526316,0.246657,-0.192086,0.171893,0.918098,-0.0500325,-0.385542,-0.074113],[0.0526316,0.283122,-0.205784,0.206919,0.902773,-0.133699,-0.299761,-0.277074],[0.0526316,0.312963,-0.236659,0.236636,0.821807,-0.19487,-0.193409,-0.498566],[0.0526316,0.324278,-0.281318,0.260721,0.660932,-0.222558,-0.0628366,-0.712962],[0.0526316,0.309126,-0.32735,0.279067,-0.427403,0.212808,-0.0841323,0.873947],[0.0526316,0.27038,-0.358775,0.29315,-0.160579,0.169748,-0.23024,0.943985],[0.0526316,0.220631,-0.366317,0.306352,0.0907054,0.110352,-0.360081,0.921198]]
        return seq
        #return self.rod.convertAtoQ(a)
      else : 
        return self.client.robot.getLinkPosition (jointName)
        
        


        
        
        
        

#!/usr/bin/env python
import os.path

from hpp.corbaserver.flecto.client import Client as flectoClient
from gepetto.corbaserver import Client as GuiClient
from hpp.gepetto import Viewer


class FlectoViewer(Viewer):
    ## Constructor
    #  \param problemSolver object of type ProblemSolver
    #  \param viewerClient if not provided, a new client to
    #         gepetto-viewer-server is created.
    #
    #  The robot loaded in hppcorbaserver is loaded into gepetto-viewer-server.
    def __init__(self,problem, viewerClient = None):
        self.rod = flectoClient().rod
        self.problemSolver = problem
        self.robot = problem.robot
        if not viewerClient:
          viewerClient = GuiClient ()
          self.createWindowAndScene (viewerClient, "hpp_")
        self.client = viewerClient
        self.displayName = self.robot.displayName
        self.buildRobotBodies ()
        self.client.gui.addToGroup (self.displayName, self.sceneName)
        
    ## display rod, in the same way as viewer __call__
    # but here we must compute the geometry of the rod from the a (in R6) parameter
    def __call__(self, args):
      q_list = self.rod.convertAtoQ(args)
      # recup chaque capsule dans la liste et la display avec self.client.gui


    def createRod(self,nameRod,color,radius,totalLength,maxCaspule):
      self.rod.createRod(nameRod,color,radius,totalLength,maxCaspule)
      self.client.gui.createGroup(nameRod)
      totalLength = totalLength +0.0 #cast to float
      for i in range(0,self.rod.getRodMaxCapsule(nameRod)) :
        self.client.gui.addCapsule(self.rod.getRodCapsule(nameRod,i),radius,(totalLength/maxCaspule),color)
        self.client.gui.addToGroup(self.rod.getRodCapsule(nameRod,i),nameRod)   
      self.client.gui.addToGroup(nameRod,self.sceneName)
      return True
        
        
    def applyRodConfiguration(self,nameRod,q_list):
      self.setDisplayedCapsule(nameRod,len(q_list))
      length = (self.rod.getRodTotalLength(nameRod) +0.0)/self.rod.getRodMaxCapsule(nameRod)
      
      for i in range (0,len(q_list)):
        self.client.gui.applyConfiguration(self.rod.getRodCapsule(nameRod,i),self.rod.convertToOSG(q_list[i],length))
      self.client.gui.refresh()
      
      return True
        
        
    # set visibility On for all capsule before index, and off for all capsule after index
    def setDisplayedCapsule(self,nameRod,index):
      if index>self.rod.getRodMaxCapsule(nameRod):
        return False
      for i in range(0,index):
        self.client.gui.setVisibility(self.rod.getRodCapsule(nameRod,i),"ON")
      for i in range (index,self.rod.getRodMaxCapsule(nameRod)):
        self.client.gui.setVisibility(self.rod.getRodCapsule(nameRod,i),"OFF")
      return True
      
      
      
      
      
        

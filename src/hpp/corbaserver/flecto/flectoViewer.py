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
       #creer capsule de taille unitaire pour etre rescale ensuite seulement en longueur
        self.client.gui.addCapsule(self.rod.getRodCapsule(nameRod,i),radius,totalLength/maxCaspule,color)
        self.client.gui.addToGroup(self.rod.getRodCapsule(nameRod,i),nameRod)   
      self.client.gui.addToGroup(nameRod,self.sceneName)
      return True
        
        
    # q_list est un ensemble de vecteur 8 : le premier param est la longueur de la capsule, les autres sont sa configuration (3 pos + 4 quat), les quarternions sont selon la convention mathematique : oriente en X+
    # pretty : boolean pour indiquer si l'on veux un affichage lisse (True) ou exacte mathematiquement (False)
    def applyRodConfiguration(self,nameRod,q_list,pretty):
      self.setDisplayedCapsule(nameRod,len(q_list))
      #length = (self.rod.getRodTotalLength(nameRod) +0.0)/self.rod.getRodMaxCapsule(nameRod)
      for i in range (0,len(q_list)):
        self.client.gui.resizeCapsule(self.rod.getRodCapsule(nameRod,i),q_list[i][0])
        #self.client.gui.setScale(self.rod.getRodCapsule(nameRod,i),[1,1]+[q_list[i][0]])
        if pretty == True :
          self.client.gui.applyConfiguration(self.rod.getRodCapsule(nameRod,i),self.rod.convertToOSG(q_list[i][1:8],0))
        else: 
          self.client.gui.applyConfiguration(self.rod.getRodCapsule(nameRod,i),self.rod.convertToOSG(q_list[i][1:8],q_list[i][0]))
       # apply translation to all the capsule :
      if pretty :
        self.client.gui.applyConfiguration(nameRod,[(q_list[0][0]/2)+q_list[0][1]] + q_list[0][2:8])
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
      
      
      
      
      
        

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


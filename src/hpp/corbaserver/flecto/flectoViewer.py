#!/usr/bin/env python
import os.path

from hpp.corbaserver.flecto.client import Client as flectoClient



class FlectoViewer(object):
    ## Constructor
    #  \param problemSolver object of type ProblemSolver
    #  \param viewerClient if not provided, a new client to
    #         gepetto-viewer-server is created.
    #
    #  The robot loaded in hppcorbaserver is loaded into gepetto-viewer-server.
    def __init__(self, viewer):
        self.viewer = viewer
        self.viewerGUI = viewer.client.gui
        self.client = flectoClient()

    ## display rod, in the same way as viewer __call__
    # but here we must compute the geometry of the rod from the a (in R6) parameter
    def __call__(self, args):
      q_list = self.client.rod.convertAtoQ(args)
      # recup chaque capsule dans la liste et la display avec self.viewerGUI


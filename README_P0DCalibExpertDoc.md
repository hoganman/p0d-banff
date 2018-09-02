P0D Calibration Expert Documentation
====================================

AUTHOR: Matthew Hogan (hoganman@gmail.com)

This document provides the technical details for experts to perform calibration duties for the P0D. Please contribute to this document as necessary.

Table of Contents
=================

 1. Introduction
 2. Time

1. Introduction
===============

     The P0D calibration work is a non-intensive set of tasks performed during a T2K run break or after a run is completed. The required task as P0D expert is perform the time-offset correction constants for each Trip-T Front-End Board (TFB) and their associated read-out ..?.. module (RMM). If you are unfamiliar with these terms, you are recommended to read the Trip-T Detector (TTD) expert documentation on t2k.org.

     The T2K data for the calibration tasks should be available on the Large Hadron Collider (LHC) Computing Grid, or Grid for short. The Grid is a shared network of job computing and data storage ``elements'' across the world for particle physics experiments. As of Aug 2018, the data is hosted at KEK, but there are efforts to replicate it elsewhere like in the UK. TODO: Update this location/lab/university. As the expert you are encouraged to use these resources instead of your own.

     Output data processed on the Grid should be then copied to your institution's local processing resources like a data server. The Grid is not designed for interactive sessions, and therefore you will need to download your data to view it. As this point, most of the computational work has been done and it is up to you to finish and validate the calibration data on your own.

     The final part of each task is to upload the calibration data to the T2K calibration database. From there, the programs that create the real production data (RDP) can calibrate out issues associated with the electronics.

     Non-essential tasks that lack document include the:

 * Bar-to-bar MIP light-yield equalization calibration (see T2K-TN-???)

Any updates to these tasks should be updated by the current expert.

The other purpose of the document is to show how one interacts with the Grid.

2. Time-Offset Constants
========================

The P0D's electronics are not in syncronization with one another and this can cause events to be misreconstructed. To allievate this, the time-offset calibration attempts to correct the internal timing of each TFB and RMM using through-going sand muons. A sand muon is a neutrino-induced, charged current interaction in the sand between the proton target material and ND280. Through going sand muons are muons that deposit enough energy to be seen in at least one scintillating bar in a scintillator layer inside the
P0D.

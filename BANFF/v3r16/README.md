# BANFF

The BANFF is a frequentist fitting framework that uses the ND280 data to constrain the T2K flux and cross-section model parameters.  The complete documentation can be found here: http://nd280.lancs.ac.uk/devel/invariant/BANFF/ 

The official BANFF, T2KReWeight and psyche code can be downloaded from the T2K CVS repository and compiled through CMT.  This GitHub repo holds the development version of the code, to allow multiple developers to work easily in parallel, and will be merged into the CVS repo when the analysis is finalised.

Generating detector covariance matrices:

You have to run `RunSyst_BinCorr.cxx` in psyche on the flat trees, that gives you the detector throws - make sure to change the systematics parameters in the parameter file of psycheSteering (make sure you are saving the NRooTrackerVtx and running with AllSyst at the end of the parameter file).
Then you run `genWeightFromPsycheRunSyst_new.cxx` from T2KReWeight on the output file from `RunSyst_BinCorr.cxx`.  This creates a friend tree with all the NIWG weights and the 1p1h fake data weight.
Finally you run `MakeND280Cov.cxx` with the 2 lists of file - there is a check to make sure that the friends trees are correctly linked.

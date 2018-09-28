---
title: p0d-banff
---

P0D-BANFF
=========

This repo is designed to contain all the necessary ingredients to run P0D samples in BANFF.

Below is a list of the working pckages. All these packages are managed by CMT but not all from the same repo

If a branch exists in this repo not listed here, then it is not used
If a package branch in a container package like highlandIO in nd280Highland2 does not match what is committed in the CVS repo, chose the newer one

Changes in Software
===================

# General Notes

Using head of nd280Psyche while using frozen versions of HL2.

# highlandIO
 * Changed the parameters file to save P0D in the pre-selection (aka save into the FlatTree)

# psycheSelections
 * Replaced p0dNuMuCCSelection with p0dWaterNuMuCCSelection. In addition an water-out (air) completementary selection, there are now numubar bkg in RHC, numubar in RHC, and numu bkg in RHC. The highest momentum postive track replaces the highest momentum negative track for numubar. In RHC specifically, an additional cut requires that the main track be the highest momentum track.

# psycheSteering
 * Added more branches to `RunSyst_New.exe`
 * Added to AnalysisManager the new p0d Selections

# psycheND280AnalysisUtils
 * Added the eight selections to SampleId

    SOFTWARE                      BRANCH       REPO
    _____________________________ ____________ __________________________________________________________
    BANFF                         psyche-HEAD  hoganman@github.com/t2k-software/BANFF
    T2KReWeight                   develop      hoganman@github.com/t2k-software/T2KReWeight
    nd280Psyche                   v3r32        :ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository

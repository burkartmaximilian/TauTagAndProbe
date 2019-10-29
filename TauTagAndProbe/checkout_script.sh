#!/usr/bin/bash

NJOBS=$(grep -c ^processor /proc/cpuinfo)
if [ ! "$1" == "" ]; then
    NJOBS=$1
fi

source /cvmfs/cms.cern.ch/cmsset_default.sh
scramv1 project CMSSW_10_2_17
cd CMSSW_10_2_17/src/
eval `scramv1 runtime -sh`

git cms-init

git cms-merge-topic cms-egamma:EgammaPostRecoTools

git cms-merge-topic -u cms-tau-pog:CMSSW_10_2_X_tau-pog_DeepTau2017v2p1_nanoAOD

scram b -j$NJOBS

git clone git@github.com:mburkart/TauTagAndProbe.git -b master_HLT
git clone git@github.com:mburkart/EGTagAndProbe.git -b HLT_SingleTau
git clone git@github.com:mburkart/QCDTauTagAndProbe.git

scram b -j$NJOBS

git clone git@github.com:KIT-CMS/grid-control.git

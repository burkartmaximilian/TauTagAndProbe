#!/bin/bash

# elementIn () {
#     local e match=$1
#     shift
#     for e; do [[ "$e" == "$match" ]] && return 0; done
#     return 1
# }

USAGE="Usage: $0 era1 era2 era3 ... eraN"

if [ "$#" == "0" ]; then
    echo $USAGE
    exit 1
fi

CONFIGS=()
# Read in passed command line options for era one after another and append subdirectories to be processed.
while (( "$#" ))
do
    ERA=$1
    case $ERA in 
        2016)
            CONFIGS+=($(ls grid_control_configs/2016/*.conf))
            ;;
        2017)
            CONFIGS+=($(ls grid_control_configs/2017/*.conf))
            ;;
        2018)
            CONFIGS+=($(ls grid_control_configs/2018/*.conf))
            ;;
        *)
            echo "[ERROR] Unsupported argument. Supported are only 2016, 2017 and 2018. Exiting now..."
            exit 1
            ;;
    esac
    shift
done

echo "[INFO] Starting grid-control jobs with configurations:"
for conf in ${CONFIGS[@]}
do
    echo "[INFO] $conf"
done

# Setup CMSSW.
echo "[INFO] Setting up scram runtime environment.."
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 runtime -sh`

# Initialize grid-control.
echo "[INFO] Initializing grid-control.."
export PATH=$PATH:${CMSSW_BASE}/src/grid-control:${CMSSW_BASE}/src/grid-control/scripts

echo "[INFO] Setting up proxy.."
export X509_USER_PROXY=~/.globus/x509up
if [ `voms-proxy-info | awk '/timeleft/{print $NF}' | cut -d : -f 1` -lt 24 ]; then
    echo "[INFO] No valid proxy or proxy with short lifetime found. Recreating a new one..."
    voms-proxy-init -rfc --valid 192:00:00 --voms cms:/cms/dcms
fi

touch .lock
while [ -f ".lock" ]
do
    for CONF in ${CONFIGS[@]}
    do
        go.py $CONF -m 1
    done
    echo "rm .lock"
    sleep 2
done

import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
process = cms.Process("TagAndProbe")

#isMC = False
isMC = False
#is2016 = True
is2016 = False

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

#### handling of cms line options for tier3 submission
#### the following are dummy defaults, so that one can normally use the config changing file list by hand etc.

options = VarParsing.VarParsing ('analysis')
options.register ('skipEvents',
                  -1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Number of events to skip")
options.register ('JSONfile',
                  "", # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "JSON file (empty for no JSON)")
if isMC:
    options.outputFile = 'NTuple_MC.root'
else:
    options.outputFile = 'NTuple_Data.root'
options.inputFiles = []

#options.register('numOfThreads',
#                 10,
#                 VarParsing.VarParsing.multiplicity.singleton,
#                 VarParsing.VarParsing.varType.int,
#                 'Number of threads.'
#                )
#
#options.register('numOfStreams',
#                 10,
#                 VarParsing.VarParsing.multiplicity.singleton,
#                 VarParsing.VarParsing.varType.int,
#                 'Number of streams.'
#                )
options.parseArguments()
# START ELECTRON CUT BASED ID SECTION
#
# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream
#

from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
setupEgammaPostRecoSeq(process,
                       era="2017-Nov17ReReco",
                       eleIDModules=[
                           "RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff",

                           "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff",
                           "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff",

                           "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff",
                           "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff",
                       ],
                       phoIDModules=[])

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
        process,
        jetSource = cms.InputTag("slimmedJets"),
        pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
        svSource = cms.InputTag("slimmedSecondaryVertices"),
        jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),
        btagDiscriminators = [
            'pfDeepFlavourJetTags:probb',
            'pfDeepFlavourJetTags:probbb',
            'pfDeepFlavourJetTags:problepb',
            ],
        postfix='NewDFTraining'
)


process.bTaggingSequence = cms.Sequence(
        process.patJetCorrFactorsNewDFTraining +
        process.updatedPatJetsNewDFTraining +
        process.pfImpactParameterTagInfosNewDFTraining +
        process.pfInclusiveSecondaryVertexFinderTagInfosNewDFTraining +
        process.pfDeepCSVTagInfosNewDFTraining +
        process.pfDeepFlavourTagInfosNewDFTraining +
        process.pfDeepFlavourJetTagsNewDFTraining +
        process.patJetCorrFactorsTransientCorrectedNewDFTraining +
        process.updatedPatJetsTransientCorrectedNewDFTraining +
        process.selectedUpdatedPatJetsNewDFTraining
)

#START RERUNNING OF ID TRAINING
#
# set up the rerunning of the latest tau id trainings
import RecoTauTag.RecoTau.tools.runTauIdMVA as idemb
na = idemb.TauIDEmbedder(process, cms,
        debug=True,
        updatedTauName = "NewTauIDsEmbedded",
        toKeep=["2017v2", "newDM2017v2", "deepTau2017v2p1"]
)
na.runTauID()



if not isMC: # will use 80X
    from Configuration.AlCa.autoCond import autoCond
    process.GlobalTag.globaltag = '102X_dataRun2_v8'
    process.load('TauTagAndProbe.TauTagAndProbe.tagAndProbe_2017_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
             "/store/data/Run2017D/SingleMuon/MINIAOD/31Mar2018-v1/00000/2AB0A664-EC39-E811-8A25-0612DC000281.root"
        ),
    )
else:
    process.GlobalTag.globaltag = '102X_mc2017_realistic_v6' #MC 25 ns miniAODv2
    process.load('TauTagAndProbe.TauTagAndProbe.MCanalysis_2017_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
	     "/store/mc/RunIIFall17MiniAODv2/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1/910000/ECC11159-F647-E811-A157-001E67792510.root"
        )
    )

if is2016 and not isMC:
    process.patTriggerUnpacker.patTriggerObjectsStandAlone = cms.InputTag("selectedPatTrigger","","RECO")


#if options.JSONfile:
#    print "Using JSON: ", options.JSONfile
#    process.source.lumisToProcess = LumiList.LumiList(filename = options.JSONfile).getVLuminosityBlockRange()

if options.inputFiles:
    process.source.fileNames = cms.untracked.vstring(options.inputFiles)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

if options.maxEvents >= -1:
    process.maxEvents.input = cms.untracked.int32(options.maxEvents)
if options.skipEvents >= 0:
    process.source.skipEvents = cms.untracked.uint32(options.skipEvents)


print options.JSONfile

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
#    numberOfThreads = cms.untracked.uint32(options.numOfThreads),
#    numberOfStreams = cms.untracked.uint32(options.numOfStreams)
)

process.p = cms.Path(
    process.PreFilterSeq +
    process.egammaPostRecoSeq +
    process.rerunMvaIsolationSequence +
    process.NewTauIDsEmbedded +
    process.bTaggingSequence +
    process.TAndPseq +
    process.NtupleSeq
)

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Adding ntuplizer
process.TFileService=cms.Service('TFileService',fileName=cms.string(options.outputFile))

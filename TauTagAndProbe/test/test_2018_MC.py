import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
process = cms.Process("TagAndProbe")

#isMC = False
isMC = True
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
                       era="2018-Prompt",
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
        updatedTauName="NewTauIDsEmbedded",
        toKeep=["2017v2", "newDM2017v2", "deepTau2017v2p1"]
)
na.runTauID()



if not isMC: # will use 80X
    from Configuration.AlCa.autoCond import autoCond
    process.GlobalTag.globaltag = '102X_dataRun2_Sep2018ABC_v2'
    # process.GlobalTag.globaltag = '102X_dataRun2_Prompt_v13'
    process.load('TauTagAndProbe.TauTagAndProbe.tagAndProbe_2018_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
              "/store/data/Run2018A/SingleMuon/MINIAOD/17Sep2018-v2/270000/F0A38EF6-D656-794D-9C8E-DCF533C98FFA.root"
        ),
    )
else:
    process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v18' #MC 25 ns miniAODv2
    process.load('TauTagAndProbe.TauTagAndProbe.MCanalysis_2018_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
             #"/store/mc/RunIIAutumn18MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/00000/87F96FBB-7EAC-BC44-8CA9-9B5787ECD801.root"
             "file:///ceph/mburkart/trigger_testing_inputs/042C8EE9-9431-5443-88C8-77F1D910B3A5.root",
             "file:///ceph/mburkart/trigger_testing_inputs/08540B6C-AA39-3F49-8FFE-8771AD2A8885.root",
             "file:///ceph/mburkart/trigger_testing_inputs/872CAA08-8946-AA43-A812-F6E5963D917B.root",
             "file:///ceph/mburkart/trigger_testing_inputs/8F103C41-A7BA-754F-923E-B5C102366249.root",
             "file:///ceph/mburkart/trigger_testing_inputs/973AE986-070D-ED40-9A99-393E4E212670.root"
        )
    )

if is2016 and not isMC:
    process.patTriggerUnpacker.patTriggerObjectsStandAlone = cms.InputTag("selectedPatTrigger","","RECO")



if options.JSONfile:
    print "Using JSON: " , options.JSONfile
    process.source.lumisToProcess = LumiList.LumiList(filename = options.JSONfile).getVLuminosityBlockRange()

if options.inputFiles:
    process.source.fileNames = cms.untracked.vstring(options.inputFiles)

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(-1)
            )

# options.maxEvents = -1

if options.maxEvents >= -1:
    process.maxEvents.input = cms.untracked.int32(options.maxEvents)
if options.skipEvents >= 0:
    process.source.skipEvents = cms.untracked.uint32(options.skipEvents)



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

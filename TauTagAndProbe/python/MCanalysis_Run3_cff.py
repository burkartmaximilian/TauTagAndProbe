import FWCore.ParameterSet.Config as cms

print "Running on MC"

# filter HLT paths for T&P
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt


HLTLIST_TAG = cms.VPSet(
    #MuTau SingleL1
    cms.PSet (
        HLT = cms.string("HLT_IsoMu27_v"),
        path1 = cms.vstring ("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07"),
        path2 = cms.vstring (""),
        leg1 = cms.int32(13),
        leg2 = cms.int32(13)
    ),
)


HLTLIST = cms.VPSet(
    #Mu-Tau35 (di-tau monitoring)
    cms.PSet (
        HLT = cms.string("HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1_v"),
        path1 = cms.vstring ("hltL3crIsoL1sBigOrMuXXerIsoTauYYerL1f0L2f10QL3f24QL3trkIsoFiltered0p07", "hltHpsOverlapFilterIsoMu24MediumChargedIsoPFTau35MonitoringReg"),
        path2 = cms.vstring ("hltHpsSelectedPFTau35TrackPt1MediumChargedIsolationL1HLTMatchedReg", "hltHpsOverlapFilterIsoMu24MediumChargedIsoPFTau35MonitoringReg"),
        leg1 = cms.int32(13),
        leg2 = cms.int32(15)
    ),
)




hltFilter = hlt.hltHighLevel.clone(
    # TriggerResultsTag = cms.InputTag("TriggerResults","","MYHLT"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = ['HLT_IsoMu27_v*'],
    andOr = cms.bool(True), # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True) #if True: throws exception if a trigger path is invalid
)

### ----------------------------------------------------------------------
### gen info, only from MC
### ----------------------------------------------------------------------
genInfo = cms.EDProducer("GenFiller",
         src = cms.InputTag("prunedGenParticles"),
         storeLightFlavAndGlu = cms.bool(True) # if True, store also udcs and gluons (first copy)
 )

## only events where slimmedMuons has exactly 1 muon
muonNumberFilter = cms.EDFilter ("muonNumberFilter",
    src = cms.InputTag("slimmedMuons")
)

## good muons for T&P
goodMuons = cms.EDFilter("PATMuonRefSelector",
        src = cms.InputTag("slimmedMuons"),
        cut = cms.string(
                'pt > 24 && abs(eta) < 2.1 ' # kinematics
                '&& ( (pfIsolationR04().sumChargedHadronPt + max(pfIsolationR04().sumNeutralHadronEt + pfIsolationR04().sumPhotonEt - 0.5 * pfIsolationR04().sumPUPt, 0.0)) / pt() ) < 0.1 ' # isolation
                '&& isMediumMuon()' # quality -- medium muon
        ),
        filter = cms.bool(True)
)

## good taus - apply analysis selection
goodTaus = cms.EDFilter("PATTauRefSelector",
        src = cms.InputTag("NewTauIDsEmbedded"),
        cut = cms.string(
        #        'pt > 5 && abs(eta) < 2.1 ' #kinematics
                'pt > 18 && abs(eta) < 2.1 ' #kinematics
                '&& abs(charge) > 0 && abs(charge) < 2 ' #sometimes 2 prongs have charge != 1
                # '&& tauID("decayModeFinding") > 0.5 ' # tau ID
                '&& (tauID("decayModeFinding") > 0.5 || tauID("decayModeFindingNewDMs") > 0.5)'
                '&& (tauID("byVVLooseIsolationMVArun2017v2DBoldDMwLT2017") > 0.5 || tauID("byVVVLooseDeepTau2017v2p1VSjet") > 0.5)' # tau iso - NOTE: can as well use boolean discriminators with WP
                '&& (tauID("againstMuonLoose3") > 0.5 || tauID("byVLooseDeepTau2017v2p1VSmu") > 0.5)' # anti Muon tight
                '&& (tauID("againstElectronVLooseMVA6") > 0.5 || tauID("byVVVLooseDeepTau2017v2p1VSe") > 0.5)' # anti-Ele loose
        ),
        filter = cms.bool(True)
)


genMatchedTaus = cms.EDFilter("genMatchTauFilter",
        taus = cms.InputTag("goodTaus")
    )

## b jet veto : no additional b jets in the event (reject tt) -- use in sequence with
bjets = cms.EDFilter("PATJetRefSelector",
        src = cms.InputTag("selectedUpdatedPatJetsNewDFTraining"),
        cut = cms.string(
                'pt > 20 && abs(eta) < 2.4 ' #kinematics
                '&& (bDiscriminator("pfDeepFlavourJetTags:probb") + bDiscriminator("pfDeepFlavourJetTags:probbb") + bDiscriminator("pfDeepFlavourJetTags:problepb")) > 0.2770' # b tag with medium WP
        ),
        #filter = cms.bool(True)
)

TagAndProbe = cms.EDFilter("TauTagAndProbeFilter",
                           taus  = cms.InputTag("goodTaus"),
                           muons = cms.InputTag("goodMuons"),
                           met   = cms.InputTag("slimmedMETs"),
                           useMassCuts = cms.bool(False),
                           electrons = cms.InputTag("slimmedElectrons"),
                           electronId = cms.string("mvaEleID-Fall17-iso-V2-wpLoose"),
                           eleVeto = cms.bool(True),
                           bjets = cms.InputTag("bjets"),
                           tauId = cms.string("byDeepTau2017v2p1VSjetraw")
)



patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
                                    # patTriggerObjectsStandAlone = cms.InputTag("selectedPatTriggerCustom"),
                                    # triggerResults = cms.InputTag('TriggerResults', '', "MYHLT"),
                                    patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
                                    triggerResults = cms.InputTag('TriggerResults', '', "HLT"),
                                    unpackFilterLabels = cms.bool(True)
                                    )

# Ntuplizer.taus = cms.InputTag("genMatchedTaus")
Ntuplizer = cms.EDAnalyzer("NtuplizerTau",
    treeName = cms.string("TagAndProbe"),
    isMC = cms.bool(True),
    genCollection = cms.InputTag("generator"),
    genPartCollection = cms.InputTag("genInfo"),
    muons = cms.InputTag("TagAndProbe"),
    taus = cms.InputTag("TagAndProbe"),
    puInfo = cms.InputTag("slimmedAddPileupInfo"),
    met   = cms.InputTag("slimmedMETs"),
    triggerSet = cms.InputTag("patTriggerUnpacker"),
    # triggerResultsLabel = cms.InputTag("TriggerResults", "", "MYHLT"),
    triggerResultsLabel = cms.InputTag("TriggerResults", "", "HLT"),
    L1Tau = cms.InputTag("caloStage2Digis", "Tau", "RECO"),
    L1EmuTau = cms.InputTag("simCaloStage2Digis", "MP"),
    Vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
    triggerList = HLTLIST,
    triggerList_tag = HLTLIST_TAG,
    # L2CaloJet_ForIsoPix_Collection = cms.InputTag("hltL2TausForPixelIsolationL1TauSeeded", "", "MYHLT"),
    # L2CaloJet_ForIsoPix_IsoCollection = cms.InputTag("hltL2TauPixelIsoTagProducerL1TauSeeded", "", "MYHLT"),
    # L2CaloJet_ForIsoPix_Patatrack_IsoCollection = cms.InputTag("hltL2TauPixelIsoTagProducer", "", "MYHLT"),
    L2CaloJet_ForIsoPix_Collection = cms.InputTag("hltL2TausForPixelIsolationL1TauSeeded", "", "HLT"),
    L2CaloJet_ForIsoPix_IsoCollection = cms.InputTag("hltL2TauPixelIsoTagProducerL1TauSeeded", "", "HLT"),
    L2CaloJet_ForIsoPix_Patatrack_IsoCollection = cms.InputTag("hltL2TauPixelIsoTagProducer", "", "HLT"),
    filterPath = cms.string("HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1_v"),
    stageL1Trigger = cms.uint32(2)
)

genMatchSeq = cms.Sequence(
    genMatchedTaus
)

PreFilterSeq = cms.Sequence(
    hltFilter +
    muonNumberFilter +
    goodMuons
)

TAndPseq = cms.Sequence(
    goodTaus       +
    bjets          +
    TagAndProbe    +
    genInfo        #+
#    genMatchedTaus
)

NtupleSeq = cms.Sequence(
    patTriggerUnpacker +
    Ntuplizer
)

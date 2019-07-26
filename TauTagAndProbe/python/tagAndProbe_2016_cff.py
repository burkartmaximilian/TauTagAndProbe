import FWCore.ParameterSet.Config as cms

print "Running on data"

# filter HLT paths for T&P
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt


HLTLIST_TAG = cms.VPSet(
    #MuTau SingleL1
    cms.PSet (
        HLT = cms.string("HLT_IsoMu27_v"),
        path1 = cms.vstring ("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09"),
        path2 = cms.vstring (""),
        leg1 = cms.int32(13),
        leg2 = cms.int32(13)
    ),
)


HLTLIST = cms.VPSet(
    #Mu-Tau32 (di-tau monitoring)
    cms.PSet (
        HLT = cms.string("HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v"),
        path1 = cms.vstring ("hltL3crIsoL1sMu18erIsoTau26erL1f0L2f10QL3f19QL3trkIsoFiltered0p09", "hltOverlapFilterIsoMu19MediumIsoPFTau32Reg"),
        path2 = cms.vstring ("hltPFTau32TrackPt1MediumIsolationL1HLTMatchedReg", "hltOverlapFilterIsoMu19MediumIsoPFTau32Reg"),
        leg1 = cms.int32(13),
        leg2 = cms.int32(15)
    ),
    # TODO:
    cms.PSet (
        HLT = cms.string("HLT_IsoMu24_eta2p1_MediumChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_CrossL1_v"),
        path1 = cms.vstring ("hltL3crIsoL1sBigOrMuXXerIsoTauYYerL1f0L2f10QL3f24QL3trkIsoFiltered0p07", "hltOverlapFilterIsoMu24MediumChargedIsoAndTightOOSCPhotonsPFTau35MonitoringReg"),
        path2 = cms.vstring ("hltSelectedPFTau35TrackPt1MediumChargedIsolationAndTightOOSCPhotonsL1HLTMatchedReg", "hltOverlapFilterIsoMu24MediumChargedIsoAndTightOOSCPhotonsPFTau35MonitoringReg"),
        leg1 = cms.int32(13),
        leg2 = cms.int32(15)
    ),

    #Mu-Tau20 (signal path)
    cms.PSet (
        HLT = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"),
        path1 = cms.vstring ("hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09", "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20"),
        path2 = cms.vstring ("hltPFTau20TrackLooseIsoAgainstMuon", "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20"),
        leg1 = cms.int32(13),
        leg2 = cms.int32(15)
    ),
    cms.PSet (
        HLT = cms.string("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v"),
        path1 = cms.vstring ("hltL3crIsoL1sMu18erTauJet20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09", "hltOverlapFilterIsoMu19LooseIsoPFTau20"),
        path2 = cms.vstring ("hltPFTau20TrackLooseIsoAgainstMuon", "hltOverlapFilterIsoMu19LooseIsoPFTau20"),
        leg1 = cms.int32(13),
        leg2 = cms.int32(15)
    ),

    #SingleTau
    cms.PSet (
        HLT = cms.string("HLT_VLooseIsoPFTau140_Trk50_eta2p1_v"),
        path1 = cms.vstring ("hltPFTau140TrackPt50LooseAbsOrRelVLooseIso"),
        path2 = cms.vstring (""),
        leg1 = cms.int32(15),
        leg2 = cms.int32(999)
    ),
)




hltFilter = hlt.hltHighLevel.clone(
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = ['HLT_IsoMu27_v*'],
    andOr = cms.bool(True), # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True) #if True: throws exception if a trigger path is invalid
)

## only events where slimmedMuons has exactly 1 muon
muonNumberFilter = cms.EDFilter ("muonNumberFilter",
    src = cms.InputTag("slimmedMuons")
)

## good muons for T&P
goodMuons = cms.EDFilter("PATMuonRefSelector",
        src = cms.InputTag("slimmedMuons"),
        cut = cms.string(
         #       'pt > 5 && abs(eta) < 2.1 ' # kinematics
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
                '&& (tauID("decayModeFinding") > 0.5 || tauID("decayModeFindingNewDMs") > 0.5)' # tau ID
                '&& (tauID("byVVLooseIsolationMVArun2017v2DBoldDMwLT2017") > 0.5 || tauID("byVVVLooseDeepTau2017v2VSjet") > 0.5)'
                '&& (tauID("againstMuonLoose3") > 0.5 || tauID("byVLooseDeepTau2017v2VSmu") > 0.5)' # anti Muon tight
                '&& (tauID("againstElectronVLooseMVA6") > 0.5 || tauID("byVVVLooseDeepTau2017v2VSe") > 0.5)' # anti-Ele loose
        ),
        filter = cms.bool(True)
)

## b jet veto : no additional b jets in the event (reject tt) -- use in sequence with
bjets = cms.EDFilter("PATJetRefSelector",
        src = cms.InputTag("slimmedJets"),
        cut = cms.string(
                'pt > 20 && abs(eta) < 2.4 ' #kinematics
                '&& bDiscriminator("pfDeepCSVJetTags:probb + pfDeepCSVJetTags:probbb") > 0.6321' # b tag with medium WP
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
                           bjets = cms.InputTag("bjets")
)



patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
                                    patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
                                    triggerResults = cms.InputTag('TriggerResults', '', "HLT"),
                                    unpackFilterLabels = cms.bool(True)
                                    )

Ntuplizer = cms.EDAnalyzer("NtuplizerTau",
    treeName = cms.string("TagAndProbe"),
    isMC = cms.bool(False),
    genCollection = cms.InputTag(""),
    genPartCollection = cms.InputTag(""),
    muons = cms.InputTag("TagAndProbe"),
    taus = cms.InputTag("TagAndProbe"),
    puInfo = cms.InputTag("slimmedAddPileupInfo"),
    met   = cms.InputTag("slimmedMETs"),
    triggerSet = cms.InputTag("patTriggerUnpacker"),
    triggerResultsLabel = cms.InputTag("TriggerResults", "", "HLT"),
    L1Tau = cms.InputTag("caloStage2Digis", "Tau", "RECO"),
    #L1EmuTau = cms.InputTag("simCaloStage2Digis"),
    L1EmuTau = cms.InputTag("simCaloStage2Digis", "MP"),
    Vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
    triggerList = HLTLIST,
    triggerList_tag = HLTLIST_TAG,
    L2CaloJet_ForIsoPix_Collection = cms.InputTag("hltL2TausForPixelIsolation", "", "TEST"),
    L2CaloJet_ForIsoPix_IsoCollection = cms.InputTag("hltL2TauPixelIsoTagProducer", "", "TEST"),
    filterPath = cms.string("HLT_VLooseIsoPFTau140_Trk50_eta2p1_v")
)

PreFilterSeq = cms.Sequence(
    hltFilter +
    muonNumberFilter +
    goodMuons
)

TAndPseq = cms.Sequence(
    goodTaus         +
    bjets            +
    TagAndProbe
)

NtupleSeq = cms.Sequence(
    patTriggerUnpacker +
    Ntuplizer
)

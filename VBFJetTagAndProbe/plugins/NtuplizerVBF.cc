// -*- C++ -*-
//
// Package:    QCDTagAndProbe/QCDTagAndProbe
// Class:      NtuplizerVBF
// 
/**\class NtuplizerVBF NtuplizerVBF.cc QCDTagAndProbe/QCDTagAndProbe/plugins/NtuplizerVBF.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Maximilian Burkart
//         Created:  Tue, 03 Jul 2018 12:50:23 GMT
//
//

#ifndef NTUPLIZERVBF_H
#define NTUPLIZERVBF_H
// system include files
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>


// user include files
#include <TNtuple.h>
#include <TString.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>

#include <FWCore/Framework/interface/Event.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <FWCore/ServiceRegistry/interface/Service.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include <DataFormats/PatCandidates/interface/Jet.h>
#include <FWCore/Common/interface/TriggerNames.h> 
#include <SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h>
#include <HLTrigger/HLTcore/interface/HLTConfigProvider.h>
#include <DataFormats/VertexReco/interface/Vertex.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#include <DataFormats/PatCandidates/interface/GenericParticle.h>

#include "tParameterSet.h"

#include <CommonTools/UtilAlgos/interface/TFileService.h>



//Set this variable to decide the number of triggers that you want to check simultaneously
#define NUMBER_OF_MAXIMUM_TRIGGERS 64
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class NtuplizerVBF : public edm::EDAnalyzer {
   public:
      explicit NtuplizerVBF(const edm::ParameterSet&);
      virtual ~NtuplizerVBF();

   private:
      virtual void beginJob();
      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();
      virtual void endRun(edm::Run const&, edm::EventSetup const&);

      void Initialize();
      bool hasFilters(const pat::TriggerObjectStandAlone&, const std::vector<std::string>&);
      int GenIndex(const pat::TauRef&, const edm::View<pat::GenericParticle>*);
      // ----------member data ---------------------------
      edm::EDGetTokenT<pat::TauRefVector> tauTag_;
      edm::EDGetTokenT<pat::JetRefVector> leadJet_;
      edm::EDGetTokenT<edm::ValueMap<int>> jetIdTag_;
      edm::EDGetTokenT<edm::View<pat::GenericParticle>> genParticlesTag_;
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      edm::EDGetTokenT<std::vector<reco::Vertex>> VtxTag_;
      edm::EDGetTokenT<std::vector<PileupSummaryInfo>> puTag_;
      edm::InputTag processName_;
      std::string treeName_;
      bool useGenMatch_;
      bool useHLTMatch_;
      bool isMC_;
      std::string filterPath_;

      TTree* tree_;
      TTree* triggerNamesTree_;
      
      // ---------------------------------------
      // variables to be filled in output tree
      ULong64_t indexevents_;
      Int_t runNumber_;
      Int_t lumi_;
      Int_t PS_column_;

      float MC_weight_;

      unsigned long tauTriggerBits_;
      unsigned long tauTriggerBits_woL3_;
      float tauPt_;
      float tauEta_;
      float tauPhi_;
      int tauCharge_;
      int tauDM_;
      int tau_genindex_;
      float tauTrkPt_;

      float leadJetPt_;
      float leadJetEta_;
      float leadJetPhi_;
      float trailJetPt_;
      float trailJetEta_;
      float trailJetPhi_;
      float thirdJetPt_;
      float thirdJetEta_;
      float thirdJetPhi_;
      int nJets_;
      float Mjj_;

      bool tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByLooseIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByMediumIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByTightIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByVTightIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017_;
      bool tauByVVLooseIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByVLooseIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByLooseIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByMediumIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByTightIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByVTightIsolationMVArun2017v2DBnewDMwLT2017_;
      bool tauByVVTightIsolationMVArun2017v2DBnewDMwLT2017_;

      bool tauAgainstMuonLoose3_;
      bool tauAgainstMuonTight3_;
      bool tauAgainstElectronVLooseMVA6_;
      bool tauAgainstElectronLooseMVA6_;
      bool tauAgainstElectronMediumMVA6_;
      bool tauAgainstElectronTightMVA6_;
      bool tauAgainstElectronVTightMVA6_;
      bool isLeadingTau_;

      int Nvtx_;
      float nTruePU_;
      UInt_t lastFilter_;

      bool isProbeHLTmatched_;


      //!Contains the parameters
      tVParameterSet parameters_;
      
      //! Maximum
      std::bitset<NUMBER_OF_MAXIMUM_TRIGGERS> tauTriggerBitSet_;
      std::bitset<NUMBER_OF_MAXIMUM_TRIGGERS> tauTriggerBitSet_woL3_;

      HLTConfigProvider hltConfig_;

      std::vector <std::string> triggerModules_;
      TString filterLabel_;
      TTree* filterLabelsTree_;
      unsigned int lastFilterInd_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
NtuplizerVBF::NtuplizerVBF(const edm::ParameterSet& iConfig) :
    tauTag_ (consumes<pat::TauRefVector>  (iConfig.getParameter<edm::InputTag>("taus"))),
    leadJet_ (consumes<pat::JetRefVector>  (iConfig.getParameter<edm::InputTag>("jets"))),
    jetIdTag_ (consumes<edm::ValueMap<int>> (iConfig.getParameter<edm::InputTag>("jetId"))),
    genParticlesTag_ (consumes<edm::View<pat::GenericParticle>>  (iConfig.getParameter<edm::InputTag>("genParticles"))),
    triggerObjects_ (consumes<pat::TriggerObjectStandAloneCollection> (iConfig.getParameter<edm::InputTag>("triggerSet"))),
    triggerBits_ (consumes<edm::TriggerResults> (iConfig.getParameter<edm::InputTag>("triggerResultsLabel"))),
    VtxTag_ (consumes<std::vector<reco::Vertex>> (iConfig.getParameter<edm::InputTag>("Vertices"))),
    puTag_ (consumes<std::vector<PileupSummaryInfo>> (iConfig.getParameter<edm::InputTag>("puInfo"))),
    processName_ (iConfig.getParameter<edm::InputTag>("triggerResultsLabel")),
    treeName_ (iConfig.getParameter<std::string>("treeName")),
    isMC_ (iConfig.getParameter<bool>("isMC")),
    filterPath_ (iConfig.getParameter<std::string>("filterPath"))
{
   TString triggerName;
   edm::Service<TFileService> fs;
   triggerNamesTree_ = fs->make<TTree>("triggerNames", "triggerNames");
   triggerNamesTree_->Branch("triggerNames", &triggerName);

   filterLabelsTree_ = fs -> make<TTree>("filterLabels", "filterLabels");
   filterLabelsTree_ -> Branch("filterLabels", &filterLabel_);


   //Building the trigger arrays
   const std::vector<edm::ParameterSet>& HLTList = iConfig.getParameter <std::vector<edm::ParameterSet> > ("triggerListProbe");
   for (const edm::ParameterSet& parameterSet : HLTList) {
       tParameterSet pSet;
       pSet.hltPath = parameterSet.getParameter<std::string>("HLT");
       triggerName = pSet.hltPath;
       pSet.hltFilters1 = parameterSet.getParameter<std::vector<std::string> >("path1");
       pSet.hltFilters2 = parameterSet.getParameter<std::vector<std::string> >("path2");
       pSet.leg1 = parameterSet.getParameter<int>("leg1");
       pSet.leg2 = parameterSet.getParameter<int>("leg2");
       parameters_.push_back(pSet);

       triggerNamesTree_->Fill();
   }

   Initialize();
   return;

}


NtuplizerVBF::~NtuplizerVBF()
{
    return;
}


//
// member functions
//

// ------------ method called for each event  ------------
void NtuplizerVBF::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    Initialize();

    indexevents_ = iEvent.id().event();
    runNumber_ = iEvent.id().run();
    lumi_ = iEvent.luminosityBlock();

    edm::Handle<pat::TauRefVector> taus;
    edm::Handle<pat::JetRefVector> jets;
    edm::Handle<edm::ValueMap<int>> jet_id_decisions; 
    edm::Handle<edm::View<pat::GenericParticle> > genParticles;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
    edm::Handle<edm::TriggerResults> triggerBits;
    edm::Handle<std::vector<reco::Vertex> >  vertices;
    edm::Handle<std::vector<PileupSummaryInfo>> puInfo;


    iEvent.getByToken(tauTag_, taus);
    iEvent.getByToken(leadJet_, jets);
    iEvent.getByToken(jetIdTag_, jet_id_decisions);
    iEvent.getByToken(triggerObjects_, triggerObjects);
    iEvent.getByToken(triggerBits_, triggerBits);
    iEvent.getByToken(VtxTag_, vertices);
    iEvent.getByToken(puTag_, puInfo);

    if(isMC_)
      iEvent.getByToken(genParticlesTag_, genParticles);

    const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

    const pat::JetRef leadJet = jets->at(0);
    leadJetPt_ = leadJet->pt();
    leadJetEta_ = leadJet->eta();
    leadJetPhi_ = leadJet->phi();

    const pat::JetRef trailJet = jets->at(1);
    trailJetPt_ = trailJet->pt();
    trailJetEta_ = trailJet->eta();
    trailJetPhi_ = trailJet->phi();

    if (jets->size() > 2)
    {
        const pat::JetRef addJet = jets->at(2);
        thirdJetPt_ = addJet->pt();
        thirdJetEta_ = addJet->eta();
        thirdJetPhi_ = addJet->phi();
    }

    Mjj_ = (leadJet->p4() + trailJet->p4()).M();
    nJets_ = jets->size();

    Nvtx_ = vertices->size();

    nTruePU_ = -99;
    if (isMC_)
    {
        std::vector<PileupSummaryInfo>::const_iterator PVI;
        for(PVI = puInfo->begin(); PVI != puInfo->end(); ++PVI)
        {
            if(PVI->getBunchCrossing() == 0)
            {
                float nTrueInt = PVI->getTrueNumInteractions();
                nTruePU_ = nTrueInt;
                break;
            }
        }
    }

    assert(taus->size() == 2);
    for (unsigned int index = 0; index < taus->size(); index++)
    {
        tauTriggerBitSet_.reset();
        tauTriggerBitSet_woL3_.reset();
        pat::TauRef tau = (*taus)[index];
        if (index == 0)
        {
            isLeadingTau_ = true;
        }
        else
        {
            isLeadingTau_ = false;
        }
        for (pat::TriggerObjectStandAlone obj: *triggerObjects)
        {
            const float dR = deltaR(*tau, obj);
            if (dR < 0.5)
            {
                const edm::TriggerNames::Strings& triggerNames = names.triggerNames();
                // Looking for the path index.
                unsigned int x = 0;
                unsigned int y = 0;
                for (const tParameterSet& parameter : parameters_)
                {
                    if ((parameter.hltPathIndex >= 0) && obj.hasPathName(triggerNames[parameter.hltPathIndex], false, true))
                    {
                        const std::vector<std::string>& filters = parameter.hltFilters1;
                        if (hasFilters(obj, filters))
                        {
                            tauTriggerBitSet_[x] = true;
                        }
                    }
                    x += 1;
                    if ((parameter.hltPathIndex >= 0) && obj.hasPathName(triggerNames[parameter.hltPathIndex], false, false))
                    {
                        const std::vector<std::string>& filters = parameter.hltFilters1;
                        if (hasFilters(obj, filters))
                        {
                            tauTriggerBitSet_woL3_[y] = true;
                        }
                    }
                    y += 1;
                }
            }
            // std::cout << "Kinematics: " << "pT: " << obj.pt() << " eta: " << obj.eta() << " phi: " << obj.phi() << std::endl;
            // bool isBoth = obj.hasPathName( filterPath_ + "*", true, true );
            // bool isL3   = obj.hasPathName( filterPath_ + "*", false, true );
            // bool isLF   = obj.hasPathName( filterPath_ + "*", true, false );
            // bool isNone = obj.hasPathName( filterPath_ + "*", false, false );
            // std::cout << "   " << filterPath_ + "*";
            // if (isBoth) std::cout << "(L,3)";
            // if (isL3 && !isBoth) std::cout << "(*,3)";
            // if (isLF && !isBoth) std::cout << "(L,*)";
            // if (isNone && !isBoth && !isL3 && !isLF) std::cout << "(*,*)";
            //std::cout << std::endl;
            if (obj.hasPathName(filterPath_ + "*", false, true))
            {
                for (std::vector<std::string>::reverse_iterator filterName = triggerModules_.rbegin(); filterName != triggerModules_.rend(); filterName+=1)
                {
                    if (obj.hasFilterLabel(*filterName))
                    {
                        if (triggerModules_.rend() - filterName > lastFilter_)
                        {                                                                                                                                                                                               
                                lastFilter_ = triggerModules_.rend() - filterName;
                        }
                    }
                }
            }
        }   

        tauTriggerBits_ = tauTriggerBitSet_.to_ulong();
        tauTriggerBits_woL3_ = tauTriggerBitSet_woL3_.to_ulong();

        tauPt_ = tau->pt();
        tauEta_ = tau->eta();
        tauPhi_ = tau->phi();
        tauCharge_ = tau->charge();
        tauDM_ = tau->decayMode();
        tauTrkPt_ = tau->leadChargedHadrCand()->pt();
        if (isMC_)
        {
            const edm::View<pat::GenericParticle>* genparts = genParticles.product();
            tau_genindex_ = GenIndex(tau, genparts);
        }
        tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byVVLooseIsolationMVArun2017v2DBoldDMwLT2017");
        tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017");
        tauByLooseIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017");
        tauByMediumIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017");
        tauByTightIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byTightIsolationMVArun2017v2DBoldDMwLT2017");
        tauByVTightIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017");
        tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017_ = tau->tauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017");
        tauAgainstMuonLoose3_ = tau->tauID("againstMuonLoose3");
        tauAgainstMuonTight3_ = tau->tauID("againstMuonTight3");
        tauAgainstElectronVLooseMVA6_ = tau->tauID("againstElectronVLooseMVA6");
        tauAgainstElectronLooseMVA6_ = tau->tauID("againstElectronLooseMVA6");
        tauAgainstElectronMediumMVA6_ = tau->tauID("againstElectronMediumMVA6");
        tauAgainstElectronTightMVA6_ = tau->tauID("againstElectronTightMVA6");
        tauAgainstElectronVTightMVA6_ = tau->tauID("againstElectronVTightMVA6");
    
        //std::cout << "Fill event with: EventNumber " << indexevents_ << " RunNumber " << runNumber_ << " and LumiSection: " << lumi_ << std::endl;
        tree_->Fill();
        // else
        // {
        //     std::cout << "Did not fill event with: EventNumber " << indexevents_ << " RunNumber " << runNumber_ << " and LumiSection: " << lumi_ << std::endl;
        //     std::cout << "reason: " << isTagHLTmatched ? std::cout << " no tau found" : std::cout << " tag not HLT matched";
        //     std::cout << std::endl;
        // }
    }
}


// ------------ method called once each job just before starting event loop  ------------
void NtuplizerVBF::beginJob()
{
    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>(treeName_.c_str(), treeName_.c_str());

    tree_->Branch("EventNumber", &indexevents_, "EventNumber/l");
    tree_->Branch("RunNumber", &runNumber_, "RunNumber/I");
    tree_->Branch("lumi", &lumi_, "lumi/I");

    tree_->Branch("tauPt", &tauPt_, "tauPt/F");
    tree_->Branch("tauEta", &tauEta_, "tauEta/F");
    tree_->Branch("tauPhi", &tauPhi_, "tauPhi/F");
    tree_->Branch("tauCharge", &tauCharge_, "tauCharge/I");
    tree_->Branch("tauDM", &tauDM_, "tauDM/I");
    tree_->Branch("tauTrkPt", &tauTrkPt_, "tauTrkPt/F");
    tree_->Branch("tau_genindex", &tau_genindex_, "tau_genindex/I");

    tree_->Branch("tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017", &tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017_, "tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017", &tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017_, "tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByLooseIsolationMVArun2017v2DBoldDMwLT2017", &tauByLooseIsolationMVArun2017v2DBoldDMwLT2017_, "tauByLooseIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByMediumIsolationMVArun2017v2DBoldDMwLT2017", &tauByMediumIsolationMVArun2017v2DBoldDMwLT2017_, "tauByMediumIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByTightIsolationMVArun2017v2DBoldDMwLT2017", &tauByTightIsolationMVArun2017v2DBoldDMwLT2017_, "tauByTightIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByVTightIsolationMVArun2017v2DBoldDMwLT2017", &tauByVTightIsolationMVArun2017v2DBoldDMwLT2017_, "tauByVTightIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017", &tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017_, "tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017/O");
    tree_->Branch("tauAgainstMuonLoose3", &tauAgainstMuonLoose3_,"tauAgainstMuonLoose3/O");
    tree_->Branch("tauAgainstMuonTight3", &tauAgainstMuonTight3_, "tauAgainstMuonTight3/O");
    tree_->Branch("tauAgainstElectronVLooseMVA6", &tauAgainstElectronVLooseMVA6_, "tauAgainstElectronVLooseMVA6/O");
    tree_->Branch("tauAgainstElectronLooseMVA6", &tauAgainstElectronLooseMVA6_, "tauAgainstElectronLooseMVA6/O");
    tree_->Branch("tauAgainstElectronMediumMVA6", &tauAgainstElectronMediumMVA6_, "tauAgainstElectronMediumMVA6/O");
    tree_->Branch("tauAgainstElectronTightMVA6", &tauAgainstElectronTightMVA6_, "tauAgainstElectronTightMVA6/O");
    tree_->Branch("tauAgainstElectronVTightMVA6", &tauAgainstElectronVTightMVA6_, "tauAgainstElectronVTightMVA6/O");
    tree_->Branch("tauTriggerBits", &tauTriggerBits_, "tauTriggerBits/l");
    tree_->Branch("tauTriggerBits_woL3", &tauTriggerBits_woL3_, "tauTriggerBits_woL3/l");
    tree_->Branch("isLeadingTau", &isLeadingTau_, "isLeadingTau/O");

    tree_->Branch("leadJetPt", &leadJetPt_, "leadJetPt/F");
    tree_->Branch("leadJetEta", &leadJetEta_, "leadJetEta/F");
    tree_->Branch("leadJetPhi", &leadJetPhi_, "leadJetPhi/F");

    tree_->Branch("trailJetPt", &trailJetPt_, "trailJetPt/F");
    tree_->Branch("trailJetEta", &trailJetEta_, "trailJetEta/F");
    tree_->Branch("trailJetPhi", &trailJetPhi_, "trailJetPhi/F");

    tree_->Branch("thirdJetPt", &thirdJetPt_, "thirdJetPt/F");
    tree_->Branch("thirdJetEta", &thirdJetEta_, "thirdJetEta/F");
    tree_->Branch("thirdJetPhi", &thirdJetPhi_, "thirdJetPhi/F");
    
    tree_->Branch("nJets", &nJets_, "nJets/I");
    tree_->Branch("Mjj", &Mjj_, "Mjj/F");

    tree_->Branch("isMatched", &isProbeHLTmatched_, "isMatched/O");

    tree_->Branch("Nvtx", &Nvtx_, "Nvtx/I");
    tree_->Branch("nTruePU", &nTruePU_, "nTruePU/F");

    tree_ -> Branch("lastFilter", &lastFilter_, "lastFilter/I");

    return;
}

// ------------ method called once each job just after ending the event loop  ------------
void NtuplizerVBF::endJob() 
{
    return;
}


void NtuplizerVBF::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
    Bool_t changedConfig = false;

    if (!hltConfig_.init(iRun, iSetup, processName_.process(), changedConfig))
    {
        edm::LogError("HLTMatchingFilter") << "Initialization of HLTConfigProvider failed!!";
        return;
    }

    const edm::TriggerNames::Strings& triggerNames = hltConfig_.triggerNames();
    std::cout << " ===== LOOKING FOR THE PATH INDEXES =====" << std::endl;

    for (tParameterSet& parameter : parameters_){
        const std::string& hltPath = parameter.hltPath;
        bool found = false;
        for(unsigned int j=0; j < triggerNames.size(); j++)
        {
            if (triggerNames[j].find(hltPath) != std::string::npos) {
                found = true;
                parameter.hltPathIndex = j;

                std::cout << "### FOUND AT INDEX #" << j << " --> " << triggerNames[j] << std::endl;
                // Look for the trigger filters running in this configuration.
                if (hltPath==filterPath_)
                {
                    lastFilterInd_ = j;
                }
            }
        }
        if (!found) parameter.hltPathIndex = -1;
    }
    // Get trigger modules which ran with saveTags option, e.g. important EDFilters                                                                                                                                 
     triggerModules_ = hltConfig_.saveTagsModules(lastFilterInd_);
     for (const std::string triggerModule: triggerModules_)
     {
         filterLabel_ = triggerModule;
         filterLabelsTree_->Fill();
     }

    return;
}

void NtuplizerVBF::Initialize()
{
    indexevents_ = 0;
    runNumber_ = 0;
    lumi_ = 0;

    tauPt_ = -1.;
    tauEta_ = -999;
    tauPhi_ = -999;
    tauCharge_ = 0;
    tauDM_ = -1;
    tauTrkPt_ = -1.;
    tau_genindex_ = -1;

    tauByVVLooseIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByVLooseIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByLooseIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByMediumIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByTightIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByVTightIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauByVVTightIsolationMVArun2017v2DBoldDMwLT2017_ = 0;
    tauAgainstMuonLoose3_ = 0;
    tauAgainstMuonTight3_ = 0;
    tauAgainstElectronVLooseMVA6_ = 0;
    tauAgainstElectronLooseMVA6_ = 0;
    tauAgainstElectronMediumMVA6_ = 0;
    tauAgainstElectronTightMVA6_ = 0;
    tauAgainstElectronVTightMVA6_ = 0;
    isLeadingTau_ = true;


    leadJetPt_ = -1.;
    leadJetEta_ = -999;
    leadJetPhi_ = -999;

    trailJetPt_ = -1.;
    trailJetEta_ = -999;
    trailJetPhi_ = -999;

    thirdJetPt_ = -1.;
    thirdJetEta_ = -999;
    thirdJetPhi_ = -999;
    Mjj_ = -1.;
    nJets_ = -1;

    isProbeHLTmatched_ = false;

    Nvtx_ = 0;
    nTruePU_ = 0.;

    tauTriggerBits_ = 999;
    tauTriggerBits_woL3_ = 999;

    lastFilter_ = 0;
    return; 
}

void NtuplizerVBF::endRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
    return;
}

bool NtuplizerVBF::hasFilters(const pat::TriggerObjectStandAlone&  obj , const std::vector<std::string>& filtersToLookFor)
{
    const std::vector<std::string>& eventLabels = obj.filterLabels();
    for (const std::string& filter : filtersToLookFor)
    {
        //Looking for matching filters
        bool found = false;
        for (const std::string& label : eventLabels)
        {
            //if (label == std::string("hltOverlapFilterIsoMu17MediumIsoPFTau40Reg"))
            if (label == filter)
            {
                //std::cout << "#### FOUND FILTER " << label << " == " << filter << " ####" << std::endl;
                found = true;
            }
        }
        if(!found)
        {
            return false;
        }
    }
    return true;
}

int NtuplizerVBF::GenIndex(const pat::TauRef& tau, const edm::View<pat::GenericParticle>* genparts)
{
    float dRmin = 1.0;
    int genMatchInd = -1;

    for(edm::View<pat::GenericParticle>::const_iterator genpart = genparts->begin(); genpart!=genparts->end();++genpart)
    {

        int flags = genpart->userInt ("generalGenFlags");
        int apdg = abs(genpart->pdgId());
        float pT = genpart->p4().pt();

        if( !( apdg==11 || apdg==13 || apdg==66615) ) continue;

        if( apdg==11 || apdg==13)
        {
            if( !(pT>8 && (flags&1 || (flags>>5)&1)) ) continue;
        }
        else if(apdg==66615)
        {
            int tauMothInd = genpart->userInt("TauMothIndex");
            pat::GenericParticle mother = (*genparts)[tauMothInd];
            int flags_tau = mother.userInt ("generalGenFlags");
            if( !(pT>15 && flags_tau&1) ) continue;
        }

        float dR = deltaR(*tau,*genpart);
        if(dR<0.2 && dR<dRmin)
        {
            dRmin = dR;
            if(apdg==11)
            {
                if(flags&1) genMatchInd = 1;
                else if((flags>>5)&1) genMatchInd = 3;
            }
            else if(apdg==13)
            {
                if(flags&1) genMatchInd = 2;
                else if((flags>>5)&1) genMatchInd = 4;
            }
            else if(apdg==66615)
                genMatchInd = 5;
        }

    }

    return genMatchInd;
}

//define this as a plug-in
#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(NtuplizerVBF);
#endif

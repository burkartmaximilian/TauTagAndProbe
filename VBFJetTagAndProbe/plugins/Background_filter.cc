#ifndef BACKGROUNDFILTER_H
#define BACKGROUNDFILTER_H

#include <FWCore/Framework/interface/EDFilter.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/MET.h>
#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/PatCandidates/interface/Electron.h>
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>

#include <iostream>

class Background_filter : public edm::EDFilter
{
    public:
        Background_filter(const edm::ParameterSet&);
        ~Background_filter();

    private:
        bool filter(edm::Event&, edm::EventSetup const&);

        edm::EDGetTokenT<pat::MuonRefVector> muonTag_;
        edm::EDGetTokenT<edm::View<reco::GsfElectron>> eleTag_;
        edm::EDGetTokenT<edm::ValueMap<bool>> eleIdMapTag_;
        edm::EDGetTokenT<pat::JetRefVector> bjetTag_;

        bool decision_;
};

Background_filter::Background_filter(const edm::ParameterSet& iConfig) :
    muonTag_ (consumes<pat::MuonRefVector> (iConfig.getParameter<edm::InputTag>("muons"))),
    eleTag_ (consumes<edm::View<reco::GsfElectron>> (iConfig.getParameter<edm::InputTag>("electrons"))),
    eleIdMapTag_ (consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>("eleIdMap"))),
    bjetTag_ (consumes<pat::JetRefVector> (iConfig.getParameter<edm::InputTag>("bjets"))),
    decision_ (true)
{}

Background_filter::~Background_filter()
{}

bool Background_filter::filter(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
    decision_ = true;
    // Muon veto
    edm::Handle<pat::MuonRefVector> muonHandle;
    iEvent.getByToken(muonTag_, muonHandle);
    if (muonHandle->size() > 0)
    {
        decision_ = false;
    }

    // Electron veto
    edm::Handle<edm::View<reco::GsfElectron>> eleHandle;
    iEvent.getByToken(eleTag_, eleHandle);
    edm::Handle<edm::ValueMap<bool>> ele_id_map;
    iEvent.getByToken(eleIdMapTag_, ele_id_map);
    for (unsigned int i = 0; i < eleHandle->size(); i+=1)
    {
        const auto ele = eleHandle->ptrAt(i);
        int eleId = (*ele_id_map)[ele];
        if (eleId && ele->p4().Pt() > 10 && fabs(ele->p4().Eta()) < 2.5)
        {
            decision_ = false;
            break;
        }
    }

    // bJet veto
    edm::Handle<pat::JetRefVector> bJetHandle;
    iEvent.getByToken(bjetTag_, bJetHandle);
    if (bJetHandle->size() > 0)
    {
        decision_ = false;
    }

    return decision_;    
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(Background_filter);

#endif

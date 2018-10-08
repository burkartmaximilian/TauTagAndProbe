
#ifndef JETPAIRBUILDER_H
#define JETPAIRBUILDER_H

#include <string.h>
#include <vector>
#include <utility>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <typeinfo>

#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>


class Jet_pair_builder : public edm::EDProducer {

    public:
        Jet_pair_builder(const edm::ParameterSet&);
        ~Jet_pair_builder();

    private:
        void produce(edm::Event&, edm::EventSetup const&);

        static bool compareTauPair(const std::pair<pat::TauRef,pat::TauRef>&, const std::pair<pat::TauRef, pat::TauRef>&);
        edm::EDGetTokenT<pat::TauRefVector> tausTag_;
        edm::EDGetTokenT<pat::JetRefVector> jetsTag_;
        edm::EDGetTokenT<edm::ValueMap<int>> jetIdTag_;
        double leadPtCut_;
        double trailPtCut_;
        double massCut_;
};

Jet_pair_builder::Jet_pair_builder(const edm::ParameterSet& iConfig) :
    tausTag_ (consumes<pat::TauRefVector>  (iConfig.getParameter<edm::InputTag>("taus"))),
    jetsTag_ (consumes<pat::JetRefVector> (iConfig.getParameter<edm::InputTag>("jets"))),
    jetIdTag_ (consumes<edm::ValueMap<int>> (iConfig.getParameter<edm::InputTag>("jetId"))),
    leadPtCut_ (iConfig.getParameter<double>("leadPtCut")),
    trailPtCut_ (iConfig.getParameter<double>("trailPtCut")),
    massCut_ (iConfig.getParameter<double>("massCut"))
{
    produces<pat::JetRefVector>();
}

Jet_pair_builder::~Jet_pair_builder()
{
}

void Jet_pair_builder::produce(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
    std::unique_ptr<pat::JetRefVector> resultJets(new pat::JetRefVector);
    resultJets->reserve(3);

    edm::Handle<pat::JetRefVector> jetHandle;
    edm::Handle<edm::ValueMap<int>> jet_id_decisions;
    iEvent.getByToken(jetsTag_, jetHandle);
    iEvent.getByToken(jetIdTag_, jet_id_decisions);
    assert(jetHandle.isValid());
    assert(jet_id_decisions.isValid());

    if (jetHandle->size() >= 2)
    {
        pat::JetRefVector trailJetCandidates;
        trailJetCandidates.reserve(jetHandle->size());
        for (pat::JetRefVector::const_iterator jet = jetHandle->begin(); jet != jetHandle->end(); jet++)
        {
            if ((*jet_id_decisions)[*jet] == 1 && (*jet)->pt() > trailPtCut_)
            {
                trailJetCandidates.push_back(*jet);
            }
        }
        if (trailJetCandidates.size()>1)
        {
            double mjj = 0.;
            unsigned int i1 = 0;
            unsigned int i2 = 0;
            for (pat::JetRefVector::const_iterator jet1 = trailJetCandidates.begin(); jet1 != trailJetCandidates.end()-1; jet1++)
            {
                for (pat::JetRefVector::const_iterator jet2 = jet1+1; jet2 != trailJetCandidates.end(); jet2++)
                {
                    const double mjj_temp = ((*jet1)->p4() + (*jet2)->p4()).M();
                    if (mjj_temp > mjj)
                    {
                        mjj = mjj_temp;
                        i1 = jet1 - trailJetCandidates.begin();
                        i2 = jet2 - trailJetCandidates.begin();
                    }
                }
            }
            pat::JetRef outJet1 = trailJetCandidates.at(i1);
            pat::JetRef outJet2 = trailJetCandidates.at(i2);
            if (outJet1->pt() > leadPtCut_ && outJet2->pt() > trailPtCut_)
            {
                resultJets->push_back(outJet1);
                resultJets->push_back(outJet2);
            }
            if (outJet1->pt() < leadPtCut_ && outJet1->pt() > trailPtCut_ && outJet2->pt() > trailPtCut_)
            {
                pat::JetRef outJet3 = trailJetCandidates.at(0);
                resultJets->push_back(outJet1);
                resultJets->push_back(outJet2);
                resultJets->push_back(outJet3);
            }
        }
    }
    iEvent.put(std::move(resultJets));
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(Jet_pair_builder);

#endif

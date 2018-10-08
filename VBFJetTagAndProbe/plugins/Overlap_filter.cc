#ifndef TAUPAIRFILTER_H
#define TAUPAIRFILTER_H

#include <iostream>
#include <utility>
#include <vector>

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include <DataFormats/PatCandidates/interface/Jet.h>


class Overlap_filter : public edm::EDFilter {

    public:
        Overlap_filter(const edm::ParameterSet &);
        ~Overlap_filter();

    private:
        bool filter(edm::Event &, edm::EventSetup const&);

        edm::EDGetTokenT<pat::TauRefVector> tausTag_;
        edm::EDGetTokenT<pat::JetRefVector> jetsTag_;
};

Overlap_filter::Overlap_filter(const edm::ParameterSet & iConfig) :
    tausTag_  (consumes<pat::TauRefVector>  (iConfig.getParameter<edm::InputTag>("taus"))),
    jetsTag_ (consumes<pat::JetRefVector>  (iConfig.getParameter<edm::InputTag>("jets")))
{
}

Overlap_filter::~Overlap_filter()
{}

bool Overlap_filter::filter(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
    bool filter_decision = true;
    // ------------------- get Taus -------------------------------
    edm::Handle<pat::TauRefVector> tauHandle;
    iEvent.getByToken (tausTag_, tauHandle);
    edm::Handle<pat::JetRefVector> jetHandle;
    iEvent.getByToken (jetsTag_, jetHandle);
    // if (jetHandle.failedToGet())
    // {
    //     std::cout << "Failed to get the Handle to the jets..." << std::endl;
    //     filter_decision = false;
    // }
    // else
    // {
    //    std::cout << "Number of jets in Handle: " << jetHandle->size() << std::endl;
        if (jetHandle->size() < 2 || tauHandle->size() != 2)
        {
            filter_decision = false;
        }
        for (unsigned int iJet = 0; iJet < jetHandle->size(); iJet++)
        {
            bool isMatched = false;
            const pat::JetRef myJet = (*jetHandle)[iJet];
            for (unsigned int iTau = 0; iTau < tauHandle->size(); iTau++)
            {
                if (deltaR(myJet->p4(), (*tauHandle)[iTau]->p4()) < 0.5)
                {
                    isMatched = true;
                    break;
                }
            }
            if (isMatched == true)
            {
                filter_decision = false;
                break;
            }
        }
    //}

    return filter_decision;
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(Overlap_filter);

#endif

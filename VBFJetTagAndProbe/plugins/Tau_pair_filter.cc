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


class Tau_pair_filter : public edm::EDFilter {

    public:
        Tau_pair_filter(const edm::ParameterSet &);
        ~Tau_pair_filter();

    private:
        bool filter(edm::Event &, edm::EventSetup const&);

        edm::EDGetTokenT<pat::TauRefVector> tausTag_;
};

Tau_pair_filter::Tau_pair_filter(const edm::ParameterSet & iConfig) :
    tausTag_  (consumes<pat::TauRefVector>  (iConfig.getParameter<edm::InputTag>("taus")))
{
}

Tau_pair_filter::~Tau_pair_filter()
{}

bool Tau_pair_filter::filter(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
    bool filter_decision = true;
    // ------------------- get Taus -------------------------------
    edm::Handle<pat::TauRefVector> tauHandle;
    iEvent.getByToken (tausTag_, tauHandle);
    if (tauHandle.failedToGet())
    {
        std::cout << "Failed to get the Handle to the taus..." << std::endl;
        filter_decision = false;
    }
    else
    {
        std::cout << "Number of taus in Handle: " << tauHandle->size() << std::endl;
        if (tauHandle->size() != 2)
        {
            filter_decision = false;
        }
        std::cout << "Filter decision: " << filter_decision << std::endl;
    }

    return filter_decision;
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(Tau_pair_filter);

#endif

#ifndef TAUPAIRBUILDER_H
#define TAUPAIRBUILDER_H

#include <string.h>
#include <vector>
#include <utility>
#include <iostream>
#include <assert.h>
#include <algorithm>

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


class Tau_pair_builder : public edm::EDProducer {

    public:
        Tau_pair_builder(const edm::ParameterSet&);
        ~Tau_pair_builder();

    private:
        void produce(edm::Event&, edm::EventSetup const&);

        static bool compareTauPair(const std::pair<pat::TauRef,pat::TauRef>&, const std::pair<pat::TauRef, pat::TauRef>&);
        edm::EDGetTokenT<pat::TauRefVector> tausTag_;
        edm::EDGetTokenT<pat::JetRefVector> jetsTag_;
};

Tau_pair_builder::Tau_pair_builder(const edm::ParameterSet& iConfig) :
    tausTag_ (consumes<pat::TauRefVector>  (iConfig.getParameter<edm::InputTag>("taus")))
{
    produces<pat::TauRefVector>();
}

Tau_pair_builder::~Tau_pair_builder()
{
}

void Tau_pair_builder::produce(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
    std::unique_ptr<pat::TauRefVector> resultTaus(new pat::TauRefVector);
    resultTaus->reserve(2);

    edm::Handle<pat::TauRefVector> tauHandle;
    iEvent.getByToken(tausTag_, tauHandle);
    assert(tauHandle->isNonnull());
    if (tauHandle->size() < 2)
    {
        return;
    }
    
    std::vector<std::pair<pat::TauRef, pat::TauRef>> tauPairs;

    unsigned int numTaus = tauHandle->size();
    for (unsigned int firstTau = 0; firstTau < numTaus; firstTau++)
    {
        for (unsigned int secondTau = 0; secondTau < numTaus; secondTau++)
        {
            if (deltaR(*(tauHandle->at(firstTau)), *(tauHandle->at(secondTau))) > 0.5) // TODO: maybe additionally use charge here
            {
                tauPairs.push_back(std::make_pair(tauHandle->at(firstTau), tauHandle->at(secondTau)));
            }
        }
    }
    if (tauPairs.size() > 0)
    {
        // std::cout << "Print pT and isolation of tau pairs:" << std::endl;
        // for (std::vector<std::pair<pat::TauRef, pat::TauRef>>::const_iterator it = tauPairs.begin(); it != tauPairs.end(); it++)
        // {
        //     std::cout << "Quantities for tau pair number " << it-tauPairs.begin() << std::endl;
        //     std::cout << it->first->pt() << "\t" << it->first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") << std::endl;
        //     std::cout << it->second->pt() << "\t" << it->second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") << std::endl;
        // }
        std::sort(tauPairs.begin(), tauPairs.end(), compareTauPair);
        // for (std::vector<std::pair<pat::TauRef, pat::TauRef>>::const_iterator it = tauPairs.begin(); it != tauPairs.end(); it++)
        // {
        //     std::cout << "Quantities for tau pair number " << it-tauPairs.begin() << std::endl;
        //     std::cout << it->first->pt() << "\t" << it->first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") << std::endl;
        //     std::cout << it->second->pt() << "\t" << it->second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") << std::endl;
        // }
        if (tauPairs.at(0).first->pt() > tauPairs.at(0).second->pt())
        {
            resultTaus->push_back(tauPairs.at(0).first);
            resultTaus->push_back(tauPairs.at(0).second);
        }
        else
        {
            resultTaus->push_back(tauPairs.at(0).second);
            resultTaus->push_back(tauPairs.at(0).first);
        }
        // std::cout << "pT of taus in the chosen pair:" << std::endl;
        // for (pat::TauRefVector::const_iterator it = resultTaus->begin(); it != resultTaus->end(); it++)
        // {
        //     std::cout << it->get()->pt() << std::endl;
        // }
        // std::sort(resultTaus->begin(), resultTaus->end(), [](pat::TauRef a, pat::TauRef b) -> bool
        // {
        //     return a->get()->pt() > b->get()->pt();
        // });
        // std::sort(resultTaus->begin(), resultTaus->end());
    }

    iEvent.put(std::move(resultTaus));

}

bool Tau_pair_builder::compareTauPair(const std::pair<pat::TauRef,pat::TauRef>& firstPair, const std::pair<pat::TauRef, pat::TauRef>& secondPair)
{
    bool firstGreaterSecond = false;
    if (firstPair.first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") > secondPair.first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017"))
    {
       firstGreaterSecond = true;
    }
    else if (firstPair.first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") == secondPair.first->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017")) 
    {
        if (firstPair.first->pt() > secondPair.first->pt())
        {
            firstGreaterSecond = true;
        }
        else if (firstPair.first->pt() == secondPair.first->pt())
        {
            if (firstPair.second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") > secondPair.second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017"))
            {
                firstGreaterSecond = true;
            }
            else if (firstPair.second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017") == secondPair.second->tauID("byIsolationMVArun2017v2DBoldDMwLTraw2017"))
            {
                firstGreaterSecond = firstPair.second->pt() > secondPair.second->pt();
            }
        }
    }
    return firstGreaterSecond;
}

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(Tau_pair_builder);

#endif

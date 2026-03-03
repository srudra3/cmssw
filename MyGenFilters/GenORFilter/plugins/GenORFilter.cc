// -*- C++ -*-
//
// Package:    MyGenFilters/GenORFilter
// Class:      GenORFilter
//
/**\class GenORFilter GenORFilter.cc MyGenFilters/GenORFilter/plugins/GenORFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sahithi Rudrabhatla
//         Created:  Sat, 28 Feb 2026 13:40:35 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

//
// class declaration
//

class GenORFilter : public edm::stream::EDFilter<> {
public:
  explicit GenORFilter(const edm::ParameterSet&);
  ~GenORFilter() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginStream(edm::StreamID) override;
  bool filter(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;
  edm::EDGetTokenT<reco::GenJetCollection> jetsToken_;
  edm::EDGetTokenT<reco::GenMETCollection> metsToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> leptonsAllToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> leptonsHardToken_;
  double jetPtCut_;
  double jetEtaCut_;
  double genHTcut_;
  double genMETcut_;
  unsigned int minLeptons_;
  unsigned int minLeptonsHard_;
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
GenORFilter::GenORFilter(const edm::ParameterSet& iConfig) 
  : jetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
    metsToken_(consumes<reco::GenMETCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
    leptonsAllToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("leptonsAll"))), 
    leptonsHardToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("leptonsHard"))),
    jetPtCut_(iConfig.getParameter<double>("jetPtCut")),
    jetEtaCut_(iConfig.getParameter<double>("jetEtaCut")),
    genHTcut_(iConfig.getParameter<double>("genHTcut")),
    genMETcut_(iConfig.getParameter<double>("genMETcut")),
    minLeptons_(iConfig.getParameter<unsigned int>("minLeptons")),
    minLeptonsHard_(iConfig.getParameter<unsigned int>("minLeptonsHard"))
{
 //   leptonsToken_ = consumes<reco::GenParticleCollection>(leptonsTag_);
//    metsToken_    = consumes<reco::GenMETCollection>(metsTag_);

    //nothing
}

GenORFilter::~GenORFilter() {
}
// ------------ method called on each new Event  ------------
bool GenORFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  edm::Handle<reco::GenJetCollection> jets;
  iEvent.getByToken(jetsToken_, jets);
  double HT = 0;
  for (const auto& j : *jets) {
      if (j.pt() > jetPtCut_ && std::abs(j.eta()) < jetEtaCut_) {
	  HT += j.pt();
        }
    }

  edm::Handle<reco::GenMETCollection> mets;
  iEvent.getByToken(metsToken_, mets);
  double MET = (!mets->empty()) ? mets->front().pt() : 0;

  edm::Handle<reco::GenParticleCollection> leptonsAll;
  iEvent.getByToken(leptonsAllToken_, leptonsAll);

  edm::Handle<reco::GenParticleCollection> leptonsHard;
  iEvent.getByToken(leptonsHardToken_, leptonsHard);

  bool passLeptons = (leptonsAll->size() >= minLeptons_) &&
                     (leptonsHard->size() >= minLeptonsHard_);

  bool passHTMET = (HT > genHTcut_) && (MET > genMETcut_);

  return passHTMET || passLeptons;
}



void GenORFilter::beginStream(edm::StreamID) { }
void GenORFilter::endStream() { }

void GenORFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("jets", edm::InputTag("tmpAk4GenJetsNoNu"));
    desc.add<edm::InputTag>("mets", edm::InputTag("tmpGenMetTrue"));
    desc.add<edm::InputTag>("leptonsAll", edm::InputTag("genLeptonsAll"));
    desc.add<edm::InputTag>("leptonsHard", edm::InputTag("genLeptonsFromHardProcess"));
    desc.add<double>("jetPtCut", 20.0);
    desc.add<double>("jetEtaCut", 2.5);
    desc.add<double>("genHTcut", 50.0);
    desc.add<double>("genMETcut", 80.0);
    desc.add<unsigned int>("minLeptons", 2);
    desc.add<unsigned int>("minLeptonsHard", 2);
    descriptions.add("GenORFilter", desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(GenORFilter);

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

// (local) variables associated with tree branches
Int_t             nsMu_;
vector<float>     smuPt_;
vector<float>     smubesttrackcharge_;

void ggNtuplizer::branchesStandAloneMuons(TTree* tree) {

  tree->Branch("nsMu",           &nsMu_);
  tree->Branch("smuPt",          &smuPt_);
  tree->Branch("smubesttrackcharge", &smubesttrackcharge_);

}

void ggNtuplizer::fillStandAloneMuons(const edm::Event& e, math::XYZPoint& pv, reco::Vertex vtx) {

  // cleanup from previous execution
  smuPt_                  .clear();
  smubesttrackcharge_.clear();
  nsMu_ = 0;

  edm::Handle<edm::View<pat::Muon> > muonHandle;
  e.getByToken(muonCollection_, muonHandle);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  e.getByToken(pckPFCandidateCollection_, pfcands);

  if (!muonHandle.isValid()) {
    edm::LogWarning("ggNtuplizer") << "no pat::Muons in event";
    return;
  }

  for (edm::View<pat::Muon>::const_iterator iMu = muonHandle->begin(); iMu != muonHandle->end(); ++iMu) {

    if (iMu->pt() < 3) continue;
    if (! (iMu->isStandAloneMuon())) continue;

    smuPt_    .push_back(iMu->pt());
    smubesttrackcharge_.push_back(iMu->bestTrack()->charge());

    nsMu_++;
  }

}

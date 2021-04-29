#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

// (local) variables associated with tree branches
Int_t             ntMu_;
vector<float>     tmuPt_;
vector<float>     tmubesttrackcharge_;
vector<float>     tmutrackcharge_;


void ggNtuplizer::branchesTrackerMuons(TTree* tree) {

  tree->Branch("ntMu",           &ntMu_);
  tree->Branch("tmuPt",          &tmuPt_);
  tree->Branch("tmubesttrackcharge", &tmubesttrackcharge_);
  tree->Branch("tmutrackcharge", &tmutrackcharge_);


}

void ggNtuplizer::fillTrackerMuons(const edm::Event& e, math::XYZPoint& pv, reco::Vertex vtx) {

  // cleanup from previous execution
  tmuPt_                  .clear();
  tmubesttrackcharge_.clear();
  tmutrackcharge_.clear();
  ntMu_ = 0;

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
    if (! (iMu->isTrackerMuon())) continue;

    tmuPt_    .push_back(iMu->pt());
    tmubesttrackcharge_.push_back(iMu->bestTrack()->charge());
    tmutrackcharge_.push_back(iMu->track()->charge());     
    

    ntMu_++;
  }

}

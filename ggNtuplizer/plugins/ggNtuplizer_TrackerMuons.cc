#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

// (local) variables associated with tree branches
Int_t             ntMu_;
vector<float>     tmuPt_;
vector<float>     tmueta_;
vector<float>     tmucharge_;
vector<float>     tmubesttrackcharge_;
vector<float>     tmutrackcharge_;
vector<float>     tmutrackpt_;
vector<float>     tmutracketa_;
vector<float>     tmuglobaltrackcharge_;
vector<float>     tmuglobaltrackpt_;
vector<float>     tmuglobaltracketa_;


void ggNtuplizer::branchesTrackerMuons(TTree* tree) {

  tree->Branch("ntMu",           &ntMu_);
  tree->Branch("tmuPt",          &tmuPt_);
  tree->Branch("tmueta",          &tmueta_);
  tree->Branch("tmucharge",          &tmucharge_);
  tree->Branch("tmubesttrackcharge", &tmubesttrackcharge_);
  tree->Branch("tmutrackcharge", &tmutrackcharge_);
  tree->Branch("tmutrackpt", &tmutrackpt_);
  tree->Branch("tmutracketa", &tmutracketa_);
  tree->Branch("tmuglobaltrackcharge", &tmuglobaltrackcharge_);
  tree->Branch("tmuglobaltrackpt", &tmuglobaltrackpt_);
  tree->Branch("tmuglobaltracketa", &tmuglobaltracketa_);


}


void ggNtuplizer::fillTrackerMuons(const edm::Event& e, math::XYZPoint& pv, reco::Vertex vtx) {

  // cleanup from previous execution
  tmuPt_                  .clear();
  tmueta_                  .clear();
  tmucharge_                  .clear();
  tmubesttrackcharge_.clear();
  tmutrackcharge_.clear();
  tmutrackpt_.clear();
  tmutracketa_.clear();
  tmuglobaltrackcharge_.clear();
  tmuglobaltrackpt_.clear();
  tmuglobaltracketa_.clear();
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
    tmueta_    .push_back(iMu->eta());
    tmucharge_    .push_back(iMu->charge());
    tmubesttrackcharge_.push_back(iMu->bestTrack()->charge());
    tmutrackcharge_.push_back(iMu->track()->charge());
    tmutrackpt_.push_back(iMu->track()->pt());
    tmutracketa_.push_back(iMu->track()->eta());
    if(iMu->globalTrack().isNonnull()){
      tmuglobaltrackcharge_.push_back(iMu->globalTrack()->charge());
      tmuglobaltrackpt_.push_back(iMu->globalTrack()->pt());
      tmuglobaltracketa_.push_back(iMu->globalTrack()->eta());
    }
    else{
      tmuglobaltrackcharge_.push_back(-999);
      tmuglobaltrackpt_.push_back(-999);
      tmuglobaltracketa_.push_back(-999);
    }

    ntMu_++;
  }

}

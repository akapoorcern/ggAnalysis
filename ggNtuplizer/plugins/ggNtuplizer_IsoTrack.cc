#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/ParticleFlow/interface/PFPileUpAlgo.h" //kell?

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

Int_t          nIsoTrack_;
vector<float>  isoPt_;
vector<float>  isoEta_;
vector<float>  isoPhi_;
vector<float>  isoD0_;
vector<float>  isoDz_;
vector<float>  isoCharge_;
vector<int>    isoFromPV_;
vector<int>    isoPdgID_;
vector<float>  isoChIso_;
vector<float>  isoChRelIso_;
vector<float>  isoChMiniIso_;
vector<float>  isoChRelMiniIso_;

void ggNtuplizer::branchesIsoTracks(TTree* tree) {
  tree->Branch("nIsoTrack",                       &nIsoTrack_);
  tree->Branch("isoPt",                               &isoPt_);
  tree->Branch("isoEta",                             &isoEta_);
  tree->Branch("isoPhi",                             &isoPhi_);
  tree->Branch("isoD0",                              &isoD0_);
  tree->Branch("isoDz",                               &isoDz_);
  tree->Branch("isoCharge",                       &isoCharge_);
  tree->Branch("isoFromPV",                       &isoFromPV_);
  tree->Branch("isoPdgID",                         &isoPdgID_);
  tree->Branch("isoChIso",                         &isoChIso_);
  tree->Branch("isoChRelIso",                   &isoChRelIso_);
  tree->Branch("isoChMiniIso",                 &isoChMiniIso_);
  tree->Branch("isoChRelMiniIso",           &isoChRelMiniIso_);
}

void ggNtuplizer::fillIsoTracks(const edm::Event& e, math::XYZPoint& pv, reco::Vertex vtx) {
  
  // cleanup from previous execution
  isoPt_               .clear();
  isoEta_              .clear();
  isoPhi_              .clear();
  isoD0_               .clear();
  isoDz_               .clear();
  isoCharge_           .clear();
  isoFromPV_           .clear();
  isoPdgID_            .clear();
  isoChIso_            .clear();
  isoChRelIso_         .clear();
  isoChMiniIso_        .clear();
  isoChRelMiniIso_     .clear();
  
  
  nIsoTrack_ = 0;

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  e.getByToken(pckPFCandidateCollection_, pfcands);

  if (!pfcands.isValid()) {
    edm::LogWarning("ggNtuplizer") << "no pat::PFcandidates in event";
    return;
  }
  for( pat::PackedCandidateCollection::const_iterator pf_it = pfcands->begin(); pf_it != pfcands->end(); pf_it++ ) {
    if (pf_it->charge() == 0) continue;
    bool isPFLep = (fabs(pf_it->pdgId()) == 11 || fabs(pf_it->pdgId()) == 13);
    if (isPFLep && pf_it->pt() < isoPtLeptoncut_) continue;
    if (!isPFLep && pf_it->pt() < isoPtcut_) continue;
    //if (pf_it->eta() > 2.5) continue;
    if (fabs(pf_it->dz()) > isoDZcut_) continue;
    if (fabs(pf_it->dxy()) > isoD0cut_) continue;

    //calculate isolation from other pfcandidates
    float chIso = 0, chMiniIso = 0;
    for( pat::PackedCandidateCollection::const_iterator iso_it = pfcands->begin(); iso_it != pfcands->end(); iso_it++ ) {

      if (iso_it == pf_it) continue;//don't use the pfcand we are calculating isolation for
      if (iso_it->charge() == 0) continue;
      int id = iso_it->pdgId();
      bool fromPV = (iso_it->fromPV()>1 || fabs(iso_it->dz())<0.1);
      float dR = deltaR(iso_it->eta(), iso_it->phi(), pf_it->eta(), pf_it->phi());
      float miniDR = std::max(isoMiniIsoParams_.at(0), std::min(isoMiniIsoParams_.at(1),isoMiniIsoParams_.at(2)/pf_it->pt()));

      if (dR < isoDRcut_ && fromPV) if (id == 211) chIso += iso_it->pt();
      if (dR < miniDR && fromPV) if (id == 211) chMiniIso += iso_it->pt();
    }

    if (pf_it->pt() < isoPtcutnoIso_ && chIso > isoChIsocut_) continue;

    isoChIso_.push_back(chIso);
    isoChRelIso_.push_back(chIso/pf_it->pt());
    isoChMiniIso_.push_back(chMiniIso);
    isoChRelMiniIso_.push_back(chMiniIso/pf_it->pt());
    isoPt_.push_back(pf_it->pt());
    isoEta_.push_back(pf_it->eta());
    isoPhi_.push_back(pf_it->phi());
    isoD0_.push_back(pf_it->dxy());
    isoDz_.push_back(pf_it->dz());
    isoCharge_.push_back(pf_it->charge());
    isoFromPV_.push_back(pf_it->fromPV());
    isoPdgID_.push_back(pf_it->pdgId());

    nIsoTrack_++;
  }
}

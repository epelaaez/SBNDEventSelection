// SBNAna includes.
#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/SRVector3D.h"
#include "sbnana/CAFAna/Core/Var.h"
#include "sbnana/CAFAna/Core/Cut.h"

// std includes.
#include <vector>

namespace ana
{
    // Constants
    const float fFVXMax =  199.15-10.;
    const float fFVXMin = -199.15+10.;
    const float fFVYMax =  200.00-10.;
    const float fFVYMin = -200.00+10.;
    const float fFVZMax =  500.00-50.;
    const float fFVZMin =    0.00+10.;

    const std::map<int, float> PDGToThreshold = {
        {13, 0.175}, // Muon
        {2212, 0.3}, // Proton
        {211, 0.07}, // Pi plus
        {-211, 0.07}, // Pi minus
        {111, 0.0} // Pi zero
    };

    ////////////// 
    // Simple Vars
    //////////////

    // Truth index
    const Var kTruthIndex = SIMPLEVAR(truth.index);

    // The SIMPLEVAR preprocessor macro allows us to shorthand for the following, traditional way of defining a Var:
    /*
    const Var kTruthIndex([](const caf::SRSliceProxy* slc) -> float {
        return slc->truth.index;
    });
    */

    // Primary energy
    const Var kPrimaryEnergy = SIMPLEVAR(truth.E);

    // Longest track in a slice
    const Var kLongestTrkLen([](const caf::SRSliceProxy* slc) -> float {
        float len(-5.f);
        for (auto const& pfp : slc->reco.pfp) {
            if (pfp.trk.len > len) len = pfp.trk.len;
        }
        return len;
    });

    // True energy
    const TruthVar kTrueEnergy = SIMPLETRUTHVAR(E);

    //////////////
    // Functions
    //////////////

    // Check vector is in fiducial volume
    bool bIsInFV(const caf::Proxy<caf::SRVector3D>* data) {
        return (
            (data->x > fFVXMin) && (data->x < fFVXMax) &&
            (data->y > fFVYMin) && (data->y < fFVYMax) &&
            (data->z > fFVZMin) && (data->z < fFVZMax) 
        );
    }

    // Check multiplicity of particles in a given pdg
    int iCountMultParticle(const caf::SRTrueInteractionProxy* nu, int pdg, float threshold) {
        int count = 0;
        for (auto const& prim : nu->prim) {
            float totp = std::sqrt(std::pow(prim.startp.x, 2) + std::pow(prim.startp.y, 2) + std::pow(prim.startp.z, 2));
            if ((prim.pdg == pdg) && (totp > threshold)) {
                count++;
            }
        }
        return count;
    }

    //////////////
    // Simple Cuts
    //////////////

    // Select slices originating from a neutrino
    const Cut kIsNuSlice = ( kTruthIndex >= 0.f );

    // Select only slices that are nu mu CC in origin.
    const Cut kIsNuMuCC([](const caf::SRSliceProxy* slc) {
        return ( kIsNuSlice(slc) && slc->truth.iscc && ( slc->truth.pdg == 14 || slc->truth.pdg == -14 ) );
    });

    // Check fmatch is in beam
    const Cut kIsInBeam([](const caf::SRSliceProxy* slc) {
        return ((slc->fmatch.time > 0.) && (slc->fmatch.time < 1.800));
    });

    //////////////
    // Truth Cuts
    //////////////

    const TruthCut kIsSignal([](const caf::SRTrueInteractionProxy* nu) {
        return (
            bIsInFV(&nu->position) && // check position is in fiducial volume
            nu->iscc && // check it is charged current interaction
            nu->pdg == 14 && // check particle is muon
            iCountMultParticle(nu, 2212, PDGToThreshold.at(2212)) == 2 && // check for two protons
            iCountMultParticle(nu, 211, PDGToThreshold.at(211)) == 0 && // no positively charged pions
            iCountMultParticle(nu, -211, PDGToThreshold.at(-211)) == 0 && // no negatively charged pions
            iCountMultParticle(nu, 111, 0) == 0 // no neutral pions
        );
    });

    ////////////
    // MultiVars
    ////////////

    // All track lengths from each slice
    const MultiVar kAllTrkLen([](const caf::SRSliceProxy* slc) -> std::vector<double> {
        std::vector<double> len;
        for (auto const& pfp : slc->reco.pfp) len.push_back(pfp.trk.len);
        return len;
    });
  
    ////////////
    // SpillCuts
    ////////////
  
    // A simple CRT hit veto.
    const SpillCut kCRTHitVeto([](const caf::SRSpillProxy* sr){
        for (auto const& crtHit: sr->crt_hits) {
            auto thistime = crtHit.time - 1600.; // manually shift to bring beam spill start to zero
            if (thistime > -0.1 && thistime < 1.8 && crtHit.pe > 100) return false;
        }
        return true;
    });
}
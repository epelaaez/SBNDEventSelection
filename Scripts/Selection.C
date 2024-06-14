// SBNAna includes.
#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"

// ROOT includes.
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

// std includes.
#include <vector>

// Definitions for Vars and Cuts.
#include "Definitions.h"

using namespace std;
using namespace ana;

void Selection()
{
    // Some useful variables for later.
    const std::string TargetFile = "/exp/sbnd/data/users/munjung/SBND/2023B/cnnid/cnnid.flat.caf.root";
    const double TargetPOT(6.6e20);

    // The SpectrumLoader object handles the loading of CAFs and the creation of Spectrum.
    SpectrumLoader NuLoader(TargetFile);

    // Side note: I find it to be good practice to add a letter to each object to denote the type.
    // It helps me keep track of the many different objects and makes the code more readable (opinion).

    // Create the binning schemes for the Vars we wish to plot.
    const Binning bPrimaryEnergy = Binning::Simple(20, 0, 3.0);
    const Binning bTrackLength = Binning::Simple(40, 0, 200);

    // Create a simple Spectrum showing the primary neutrino energy of only slices with a nu mu CC interaction.
    Spectrum sNuEnergy( "Neutrino Energy [GeV]",     // A label for the Spectrum.
                bPrimaryEnergy,              // Use 20 bins from 0.0 to 3.0 GeV
                NuLoader,                    // Associate this Spectrum with the NuLoader object (and its target CAF)
                kPrimaryEnergy,              // The Var to plot.
                kNoSpillCut,                 // The SpillCut to use (none in this case).
                kIsNuMuCC );                 // The Cut to use (only true nu mu CC slices).

    // Let's do the same as above, but add a SpillCut to see how many nu mu CC slices survive a simple CRT hit veto.
    Spectrum sNuEnergyNoCRT( "Neutrino Energy [GeV]",     // A label for the Spectrum.
                bPrimaryEnergy,              // Use 20 bins from 0.0 to 3.0 GeV
                NuLoader,                    // Associate this Spectrum with the NuLoader object (and its target CAF)
                kPrimaryEnergy,              // The Var to plot.
                kCRTHitVeto,                 // Use a simple CRT Hit SpillCut.
                kIsNuMuCC );                 // The Cut to use (only true nu mu CC slices).

    // Create a simple Spectrum showing the length of all tracks in all slices with a nu mu CC interaction.
    Spectrum sTrackLen( "Track Length [cm]",         // A label for the Spectrum.
                bTrackLength,                // Use 20 bins from 0.0 to 3.0 GeV
                NuLoader,                    // Associate this Spectrum with the NuLoader object (and its target CAF)
                kAllTrkLen,                  // The Var to plot.
                kNoSpillCut,                 // The SpillCut to use (none in this case).
                kIsNuMuCC );                 // The Cut to use (only true nu mu CC slices).

    // Now that each Spectrum is defined, use the Go() method to populate the Spectrum objects.
    NuLoader.Go();

    // Write out the Spectrum objects to a TCanvas
    std::vector<Spectrum> Spectra{
        sNuEnergy,
        sNuEnergyNoCRT,
        sTrackLen
    };
    std::vector<TString> PlotNames{
        "sNuEnergy",
        "sNuEnergyNoCRT",
        "sTrackLen"
    };
    const int nSpectra = Spectra.size();

    for (int i = 0; i < nSpectra; i++) {
        TCanvas* PlotCanvas = new TCanvas("Selection","Selection",205,34,1124,768);
        TH1D* Histogram = Spectra[i].ToTH1(TargetPOT);

        PlotCanvas->SetTopMargin(0.13);
        PlotCanvas->SetLeftMargin(0.17);
        PlotCanvas->SetRightMargin(0.05);
        PlotCanvas->SetBottomMargin(0.16);

        PlotCanvas->cd();
        Histogram->Draw("hist same");

        PlotCanvas->SaveAs("./Figs/"+PlotNames[i]+".png");
        delete PlotCanvas;
    }
}
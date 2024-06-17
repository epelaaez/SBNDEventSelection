# SBND Event Selection

**Note: this code was merged into the [CC1muAnalysis](https://github.com/epelaaez/CC1muAnalysis) repo.**

CAFAna is used to perform event selection on flat `caf` files. The specific event selection is one muon, two protons, and no pions. 

### Setting up

If it is the first time working in this repository, you have to run

```bash
source setup_cafana.sh
```

to set up the `sbnana` feature branch used in this codebase. Once this is done, you have to run 

```bash
source activate_cafana.sh
```

to activate your local `sbnana` build every time you start a new terminal.

### Running event selection

To perform the event selection, you have to run

```bash
cafe -bq Scripts/Selection.C
```

This will generate the cuts defined in `Definitions.h` and will produce the corresponding figures under `Figs/`.

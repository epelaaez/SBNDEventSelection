# SBND Event Selection

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

to activate your local `sbnana` build.

### Running event selection

To perform the event selection, you have to run

```bash
cafe -bq Scripts/Selection.C
```

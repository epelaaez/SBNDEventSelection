source /cvmfs/sbnd.opensciencegrid.org/products/sbnd/setup_sbnd.sh

dirName=v09_75_03
mkdir ${dirName}; cd ${dirName}
mrb newDev -v v09_75_03 -q e20:prof
source localProducts_larsoft_v09_75_03_e20_prof/setup
cd srcs

mrb g -t v09_75_03 sbnana
cd sbnana
# now pull the feature branch
git pull origin feature/jskim_TrueVar

# setup
mrbsetenv

# compile
mrb i -j4

mrbslp

# go back
cd ..
cd ..
cd ..
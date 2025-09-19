# radiation_simulations
This repo contains the DDG4 plugin(s) necessary to access the GEANT stepping information to calculate things like Total Ionizing Dose, and neutron fluence. 


compling plugin after checking out code.

main source code is locating in ddRadDose.h

It is simple code storing radiation maps in THnSparses, but will eventually be moved to TTree one offline code to calculate dose and fluence is completed.


cmake -B build -S . -DCMAKE_INSTALL_PREFIX=install

cmake --build build -- install

//This is crucial to make the plugin visible to npsim

export LD_LIBRARY_PATH=$PWD/install/lib:${LD_LIBRARY_PATH}

//needed for the high-precision neutron tables

export G4NEUTRONHPDATA=/cvmfs/geant4.cern.ch/share/data/G4NDL4.5/



------> this is the npsim command to make use of the stepping plugin. The syntax is very imporant here, or the parameters will not be passed properly. Note, the range cut and physics list differ from the default used in dd4hep

npsim --action.step '{"name":"ddRadDose","parameter":{"radiationOutputString":"testRads", "fileNumber":1, "numEvents":1}}' --compactFile $DETECTOR_PATH/epic_craterlake_10x275.xml --inputFile /gpfs02/eic/ajentsch/epicBackgrounds/disWithCrossingAngle/10x275/pythia.ep.10x275.10Kevents.RadCor=0.Q2=0.000000001-1.0.kT=1.0_48.ab.hepmc -N 1 --physics.list FTFP_BERT_HPT_EMZ --physics.rangecut "0.01*mm"




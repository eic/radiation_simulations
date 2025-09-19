#!/bin/sh

echo "Running Job ${1} with ${2} Events Starting at ${3} with input file ${4}"

json_string="'{\"name\":\"ddRadDose\",\"parameter\":{\"radiationOutputString\":\"testRads\",\"fileNumber\":${1},\"numEvents\":${2}}}'"

#json_string=hello

#printf -v $json_string '{"name":"ddRadDose","parameter":{"radiationOutputString":"testRads","fileNumber":1}}' 
#######"$VERSION" "$VERSION"

echo ${json_string}

/gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/eic-shell << EOF
#cat << EOF
#exec /gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/eic-shell
source /opt/detector/epic-main/bin/thisepic.sh
#source /gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/epic/install/bin/thisepic.sh

export LD_LIBRARY_PATH=/gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/radiationStepPlugin/install/lib:\${LD_LIBRARY_PATH}
export G4NEUTRONHPDATA=/cvmfs/geant4.cern.ch/share/data/G4NDL4.5/

cd /gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/

echo \'${json_string}\'

npsim --action.step ${json_string} --compactFile "\$DETECTOR_PATH/epic_craterlake_10x275.xml" --inputFile /gpfs02/eic/ajentsch/epicBackgrounds/disWithCrossingAngle/10x275/pythia.ep.10x275.10Kevents.RadCor=0.Q2=0.000000001-1.0.kT=1.0_${4}.ab.hepmc --outputFile /gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/simulationOutput/radiation_simulations_throw_away_${1}.edm4hep.root -N ${2} --skipNEvents ${3} --physics.list QGSP_BERT_HPT_EMZ --physics.rangecut "0.05*mm"

rm /gpfs/mnt/gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/simulationOutput/radiation_simulations_throw_away_${1}.edm4hep.root

exit
EOF

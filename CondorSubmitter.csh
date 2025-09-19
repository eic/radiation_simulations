#! /bin/env csh

set Exec = /gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/radiationStepPlugin/runSimulation.sh

####### Initialize condor file
echo ""  > CondorFileRadDose
echo "Universe     = vanilla" >> CondorFileRadDose
echo "Executable   = ${Exec}" >> CondorFileRadDose
echo "getenv = true" >> CondorFileRadDose
echo "request_memory = 8.0G" >> CondorFileRadDose


# Output Directory
set Output = /gpfs02/eic/ajentsch/ePIC_dd4hep_simulations/logs

#setup for deuteron DIS
set NUMBER = 1
set LIMIT = 100 # number of actual files
set NEVENTS = 100
set EVENTSKIP = 0
set TOTALEVENTS = 10000 #174056
#set NUMFILES = 100
set FILENUMBER = 1


while ( "$NUMBER" <= "$LIMIT" )

	@ OFFSET = ${EVENTSKIP}
	
	while("$OFFSET" <= "$TOTALEVENTS" - "$NEVENTS")

		set OutFile = ${Output}/npsim_ep_10x275_DIS_RADIATION_${NUMBER}_subEvent_${OFFSET}.out
        set ErrFile = ${Output}/npsim_ep_10x275_DIS_RADIATION_${NUMBER}_subEvent_${OFFSET}.err

    	#@ OFFSET = (${NUMBER} - 1) * ${NEVENTS}
    
    	set Args = ( $FILENUMBER $NEVENTS $OFFSET $NUMBER) 
    	echo "" >> CondorFileRadDose
    	echo "Output       = ${OutFile}" >> CondorFileRadDose
    	#echo "Log          = ${LogFile}" >> CondorFile
    	echo "Error        = ${ErrFile}" >> CondorFileRadDose
    	echo "Arguments    = ${Args}" >> CondorFileRadDose
    	echo "Queue" >> CondorFileRadDose

    	echo Submitting:
    	echo $Exec $Args
    	echo "Logging output to " $OutFile
    	#echo "Logging Condor to " $LogFile
    	echo "Logging errors to " $ErrFile
	
		@ OFFSET = ${OFFSET} + ${NEVENTS}
		@ FILENUMBER = ${FILENUMBER} + 1
	end
	@ NUMBER++
end


#condor_submit CondorFile



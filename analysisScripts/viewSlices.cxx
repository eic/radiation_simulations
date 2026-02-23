
//usage -- provide range of x and y to see your detector region, but only choose a single z-value to avoid adding neiboring voxels incorrectly
//
//
//                            x_min.  x_max. y_min.  y_max.  z_location   type (TID, meq_fluence, total_energy)
//
// root -b -q viewSlices.cxx'(-100.0, 100.0, -100.0, 100.0,   -172.5,    "TID")'
//
//


void viewSlices(double x_location_min = -100.0, double x_location_max = 100.0, double y_location_min = -100.0, double y_location_max = 100.0,
                double z_location = -172.5, TString radiationSource = "meq_fluence"){

	// USER ENTRY HERE
				
	TString pdfFile = ".pdf";

	TFile * dose_source_neutron_fluence_file = new TFile("February_2026/summedRadiationMaps/summed_1_MEQ_neutron_Feb_2026.root" );
	TFile * dose_source_proton_fluence_file = new TFile("February_2026/summedRadiationMaps/summed_1_MEQ_proton_Feb_2026.root" );
	TFile * dose_source_TID_file = new TFile("February_2026/summedRadiationMaps/summed_TID_Feb_2026.root");
	TFile * dose_source_total_energy_file = new TFile("February_2026/summedRadiationMaps/summed_total_energy_Feb_2026.root");
	
	
	/////////////////
	
	
	
	cout << "generating slices for radiation source --> " << radiationSource << endl;
	
	TString mainString = "3D_map_full_detector_and_IR_"; 

	
	TString sourceParticle = "all_sources"; //neutron or proton or all_sources

	TString zAxisTitle_full_IR;
	
	if(radiationSource == "meq_fluence"){zAxisTitle_full_IR = "1 MEQ fluence (protons and neutrons) [cm^{-2}]";}
	else if(radiationSource == "TID"){zAxisTitle_full_IR = "Total Ionizing Dose [Rads]";}
	else {zAxisTitle_full_IR = "Total Deposited Energy [GeV]";}
	//zAxisTitle_full_IR.Form("1 MEQ fluence (protons and neutrons) [cm^{-2}]");
	
	TString energy = "10x275";
	double rateScale = 5e5;
	
	TString gevStr = "GeV";
	TString beamSpecies = "";
	if(energy == "18x110") {beamSpecies = " e+Au, "; }
	else if( energy == "275"){ beamSpecies = " hadron beam+gas, "; }
	else if( energy == "10"){ beamSpecies = " electron beam+gas, "; }
	else beamSpecies = " e+p ";
	
	if( (energy == "10x275" && rateScale != 5e5) || 
		(energy == "18x110" && rateScale != 1e5) || 
		(energy == "275" && rateScale != 35e3)   ||
		(energy == "10" && rateScale < 1e6))
	{
			
			cout << "\n\n BEAM ENERGY AND COLLISION RATE DO NOT MATCH --> CHECK AGAIN!!!" << endl;
			return;
			
	}
	
	TString rateString;
	rateString.Form(" @ %.1f kHz + 35kHz hadron beamgas + 3.18MHz electron beamgas", rateScale/1000);
	
	TString mainPlotTitle = ", 1 fb^{-1} min-bias ";
	mainPlotTitle = energy + gevStr + beamSpecies + rateString + mainPlotTitle;
	
	


	
	
	TString plotname;
		
	plotname = "Norm3D_MEQ_fluence_neutron";

	TH3F * dose_source_neutron_fluence = (TH3F*)dose_source_neutron_fluence_file->Get(plotname);
	dose_source_neutron_fluence->SetDirectory(0);
	
	plotname = "Norm3D_fluence_proton";
	
	TH3F * dose_source_proton_fluence = (TH3F*)dose_source_proton_fluence_file->Get(plotname);
	dose_source_proton_fluence->SetDirectory(0);
	
	plotname = "Norm3D_total_ionizing_dose";
	
	TH3F * dose_source_TID = (TH3F*)dose_source_TID_file->Get(plotname);
	dose_source_TID->SetDirectory(0);
	
	plotname = "Norm3D_total_ionizing_dose";
	
	TH3F * dose_source_total_energy = (TH3F*)dose_source_total_energy_file->Get(plotname);
	dose_source_total_energy->SetDirectory(0);

	TH3F * dose_EM_3D; 
	if(radiationSource == "meq_fluence"){
		dose_EM_3D = (TH3F*)dose_source_neutron_fluence->Clone("clone");
		dose_EM_3D->Add(dose_source_proton_fluence);
	
		dose_EM_3D->SetName("3DNorm_1_MEQ_fluence_protons_and_neutrons");
	}
	else if(radiationSource == "TID"){
		dose_EM_3D = (TH3F*)dose_source_TID->Clone("clone");
		dose_EM_3D->SetName("3DNorm_total_ionizing_dose");
	} 
	else{
		dose_EM_3D = (TH3F*)dose_source_total_energy->Clone("clone");
		dose_EM_3D->SetName("3DNorm_total_deposited_energy");
	}
	
	dose_EM_3D->SetDirectory(0);
	

	//////FORMATTING STUFF GOES HERE
	
	double twoDPlotZMin = -500.0; //-8000.0 is nominal
	double twoDPlotZMax = 750.0; // 4000.0 is nominal
	double topDownPlotMaximum = 0.0;
	double topDownPlotMinimum = 0.0; //1e-9;
	
	if(radiationSource == "TID"){topDownPlotMinimum = 1e-9;}
	
	TPaveText * mainTitleText = new TPaveText(0.05, 0.91, 0.99, 0.99, "NB NDC");
	mainTitleText->SetFillColor(0);
	mainTitleText->AddText(mainPlotTitle);

	
	
	
	double x1flux = 0.16;
	double y1flux = 0.82;
	double x2flux = 0.6;
	double y2flux = 0.89;
	
	TPaveText * labelBox[17];

	labelBox[0] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[0]->SetFillColor(0);
	labelBox[0]->AddText("Roman Pots Station 1");
	
	labelBox[1] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[1]->SetFillColor(0);
	labelBox[1]->AddText("Roman Pots Station 2");
	
	labelBox[2] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[2]->SetFillColor(0);
	labelBox[2]->AddText("Lumi PCAL Front Half (10cm)");

	labelBox[3] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[3]->SetFillColor(0);
	labelBox[3]->AddText("Lumi PCAL Back Half (10cm)");
	
	labelBox[4] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[4]->SetFillColor(0);
	labelBox[4]->AddText("E_{kin.} > 1 MeV");
	
	labelBox[5] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[5]->SetFillColor(0);
	labelBox[5]->AddText("E_{kin.} < 1 MeV");
	
	labelBox[6] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[6]->SetFillColor(0);
	labelBox[6]->AddText("All Protons");

	labelBox[7] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[7]->SetFillColor(0);
	labelBox[7]->AddText("Proton production vtx");
	
	labelBox[8] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[8]->SetFillColor(0);
	labelBox[8]->AddText("Energy-Weighted Flux");
	
	labelBox[9] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[9]->SetFillColor(0);
	//labelBox[9]->SetFillStyle(0);
	//labelBox[9]->SetBorderSize(0);
	labelBox[9]->AddText("EM Accumulated Dose");
	
	labelBox[10] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[10]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[10]->AddText("Hadronic Accumulated Dose");
	
	labelBox[11] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[11]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[11]->AddText("Total Charged Hadron Fluence");
	
	labelBox[12] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[12]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[12]->AddText("Total e^{+/-} Fluence");
	
	labelBox[13] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[13]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[13]->AddText("Total #gamma Fluence");
	
	labelBox[14] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[14]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[14]->AddText("E_{kin.} > 20 MeV");
	
	labelBox[15] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[15]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[15]->AddText("Deposited Energy: e^{+/-} + #gamma");
	
	labelBox[16] = new TPaveText(x1flux, y1flux, x2flux, y2flux, "NB NDC");
	labelBox[16]->SetFillColor(0);
	//labelBox[10]->SetFillStyle(0);
	//labelBox[10]->SetBorderSize(0);
	labelBox[16]->AddText("Deposited Energy: h^{+/-}");
	
	
	//TPaveTextBoxes here
	
	TPaveText * detectorLabels[15];

	
	
	TBox * ZDCBox_xy = new TBox(-122, -30, -62, 30);

	ZDCBox_xy->SetLineColor(kBlack);
    ZDCBox_xy->SetLineWidth(1);
    ZDCBox_xy->SetFillStyle(0);
	
	detectorLabels[6] = new TPaveText(3500, -150, 3900, -130, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[6]->SetFillColor(0);
	detectorLabels[6]->SetBorderSize(0);
	detectorLabels[6]->AddText("ZDC");
	
	TBox * lumiExitWindow = new TBox(-1851.0, -4.0, -1850.0, 4.0);

	lumiExitWindow->SetLineColor(kBlack);
    lumiExitWindow->SetLineWidth(2);
    lumiExitWindow->SetFillStyle(0);
	
	TBox * lumiCollimator = new TBox(-2275.0, -6.5, -2245.0, 6.5);

	lumiCollimator->SetLineColor(kBlack);
    lumiCollimator->SetLineWidth(2);
    lumiCollimator->SetFillStyle(0);
	
	detectorLabels[10] = new TPaveText(-2500, 10, -1200, 30, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[10]->SetFillColor(0);
	detectorLabels[10]->SetBorderSize(0);
	detectorLabels[10]->AddText("Collimater and window");
	
	TBox * lumiSweeper = new TBox(-5660.0, -37.99, -5540.0, 37.99);

	lumiSweeper->SetLineColor(kBlack);
    lumiSweeper->SetLineWidth(1);
    lumiSweeper->SetFillStyle(0);
	
	TBox * lumiAnalyzer = new TBox(-6060.0, -37.99, -5940.0, 37.99);

	lumiAnalyzer->SetLineColor(kBlack);
    lumiAnalyzer->SetLineWidth(1);
    lumiAnalyzer->SetFillStyle(0);
	
	detectorLabels[7] = new TPaveText(-6200, 40, -5200, 55, "NB");
	//detectorLabels[5]->SetFillStyle(0);
	detectorLabels[7]->SetFillColor(0);
	detectorLabels[7]->SetBorderSize(0);
	detectorLabels[7]->AddText("Sweeper and Analyzer");
	
	TBox * lumiDirectPCAL = new TBox(-6530.0, -10.0, -6510.0, 10.0);

	lumiDirectPCAL->SetLineColor(kBlack);
    lumiDirectPCAL->SetLineWidth(1);
    lumiDirectPCAL->SetFillStyle(0);
	
	detectorLabels[11] = new TPaveText(-6550, -30, -6100, -20, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[11]->SetFillColor(0);
	detectorLabels[11]->SetBorderSize(0);
	detectorLabels[11]->AddText("Lumi. PCAL");
	
	//bECAL
	TBox * barrelEMCALBox_top = new TBox(-255, 78, 190, 120);
	
	barrelEMCALBox_top->SetLineColor(kBlack);
	barrelEMCALBox_top->SetLineWidth(2);
	barrelEMCALBox_top->SetFillStyle(0);
	
	//bECAL
	TBox * barrelEMCALBox_bottom = new TBox(-255, -120, 190, -78);
	
	barrelEMCALBox_bottom->SetLineColor(kBlack);
	barrelEMCALBox_bottom->SetLineWidth(2);
	barrelEMCALBox_bottom->SetFillStyle(0);
	
	detectorLabels[1] = new TPaveText(-240, 85, -180, 105, "NB");
	//detectorLabels[1]->SetFillStyle(0);
	detectorLabels[1]->SetFillColor(0);
	detectorLabels[1]->SetBorderSize(0);
	detectorLabels[1]->AddText("bECAL");
	
	//fHCAL Insert
	TBox * hcalInsertBox_top = new TBox(335, 10, 500, 24);
	
	hcalInsertBox_top->SetLineColor(kBlack);
	hcalInsertBox_top->SetLineWidth(2);
	hcalInsertBox_top->SetFillStyle(0);
	
	//fHCAL Insert
	TBox * hcalInsertBox_bottom = new TBox(335, -24, 500, -15);
	
	hcalInsertBox_bottom->SetLineColor(kBlack);
	hcalInsertBox_bottom->SetLineWidth(2);
	hcalInsertBox_bottom->SetFillStyle(0);
	
	//detectorLabels[4] = new TPaveText(505, 24, 705, 44, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	//detectorLabels[4]->SetFillColor(0);
	//detectorLabels[4]->SetBorderSize(0);
	//detectorLabels[4]->AddText("HCAL Insert");
	
	//fHCAL 
	TBox * hcalForwardBox_top = new TBox(335, 25, 500, 249); //should be 260!!
	
	hcalForwardBox_top->SetLineColor(kBlack);
	hcalForwardBox_top->SetLineWidth(2);
	hcalForwardBox_top->SetFillStyle(0);
	
	//fHCAL 
	TBox * hcalForwardBox_bottom = new TBox(335, -249, 500, -25); //should be -260!!!
	
	hcalForwardBox_bottom->SetLineColor(kBlack);
	hcalForwardBox_bottom->SetLineWidth(2);
	hcalForwardBox_bottom->SetFillStyle(0);
	
	detectorLabels[3] = new TPaveText(350, 200, 450, 220, "NB");
	//detectorLabels[3]->SetFillStyle(0);
	detectorLabels[3]->SetFillColor(0);
	detectorLabels[3]->SetBorderSize(0);
	detectorLabels[3]->AddText("pHCAL");
	
	//nECAL 
	TBox * nECALBox_top = new TBox(-196, 9, -175, 66);
	
	nECALBox_top->SetLineColor(kBlack);
	nECALBox_top->SetLineWidth(2);
	nECALBox_top->SetFillStyle(0);
	
	//nECAL 
	TBox * nECALBox_bottom = new TBox(-196, -63, -175, -6);
	
	nECALBox_bottom->SetLineColor(kBlack);
	nECALBox_bottom->SetLineWidth(2);
	nECALBox_bottom->SetFillStyle(0);
	
	//double labelTmpLow = -700;
	//double labelTmpHigh = -300; 
	
	double labelTmpLow = -325;
	double labelTmpHigh = -260;
	
	//if(twoDPlotZMin > -1000.0){
	//	labelTmpLow = -580;
	//	labelTmpHigh = -300; 
	//}
	
	detectorLabels[0] = new TPaveText(-270, 35, -215, 55, "NB");
	//detectorLabels[0]->SetFillStyle(0);
	detectorLabels[0]->SetFillColor(0);
	detectorLabels[0]->SetBorderSize(0);
	detectorLabels[0]->AddText("nECAL");
	//detectorLabels[0]->GetLine(0)->SetTextSize(0.04);
	
	//bHCAL 
	TBox * bHCALBox_top = new TBox(-300, 180, 320, 249); //should be 260 not 200
	
	bHCALBox_top->SetLineColor(kBlack);
	bHCALBox_top->SetLineWidth(2);
	bHCALBox_top->SetFillStyle(0);
	
	//bHCAL 
	TBox * bHCALBox_bottom = new TBox(-300, -249, 320, -180); //should be -260 not -200
	
	bHCALBox_bottom->SetLineColor(kBlack);
	bHCALBox_bottom->SetLineWidth(2);
	bHCALBox_bottom->SetFillStyle(0);
	
	detectorLabels[2] = new TPaveText(-265, 190, -200, 210, "NB");
	//detectorLabels[2]->SetFillStyle(0);
	detectorLabels[2]->SetFillColor(0);
	detectorLabels[2]->SetBorderSize(0);
	detectorLabels[2]->AddText("bHCAL");
	
	//b0 detector 
	TBox * b0Box = new TBox(540, -100, 740, 100);
	
	b0Box->SetLineColor(kBlack);
	b0Box->SetLineWidth(2);
	b0Box->SetFillStyle(0);
	
	detectorLabels[5] = new TPaveText(580, 32, 650, 52, "NB");
	//detectorLabels[5]->SetFillStyle(0);
	detectorLabels[5]->SetFillColor(0);
	detectorLabels[5]->SetBorderSize(0);
	detectorLabels[5]->AddText("B0 Magnet");
	
	//Low Q2 tagger 1
	TBox * lowQ2Tager_1 = new TBox(-1900-35.55375, -25.791750-7.39200, -1900.250959+35.55375, -25.791750+7.39200);
	
	lowQ2Tager_1->SetLineColor(kBlack);
	lowQ2Tager_1->SetLineWidth(2);
	lowQ2Tager_1->SetFillStyle(0);
	
	detectorLabels[8] = new TPaveText(-1900, -55, -1000, -35, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[8]->SetFillColor(0);
	detectorLabels[8]->SetBorderSize(0);
	detectorLabels[8]->AddText("Low Q2 Tagger 1");
	
	//Low Q2 tagger 1
	TBox * lowQ2Tager_2 = new TBox(-3750-35.55375, -55.791750-7.39200, -3750+35.55375, -55.791750+7.39200);
	
	lowQ2Tager_2->SetLineColor(kBlack);
	lowQ2Tager_2->SetLineWidth(2);
	lowQ2Tager_2->SetFillStyle(0);
	
	detectorLabels[9] = new TPaveText(-3900, -85, -3100, -65, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[9]->SetFillColor(0);
	detectorLabels[9]->SetBorderSize(0);
	detectorLabels[9]->AddText("Low Q2 Tagger 2");
	
	//Roman Pots 1
	//TBox * romanPots_1 = new TBox(2600.0, -96.0, 2604.0, -70);
	
	//romanPots_1->SetLineColor(kBlack);
	//romanPots_1->SetLineWidth(1);
	//romanPots_1->SetFillStyle(0);
	
	//Roman Pots 2
	//TBox * romanPots_2 = new TBox(2800.0, -110.0, 2804.0, -83.0);
	
	//romanPots_2->SetLineColor(kBlack);
	//romanPots_2->SetLineWidth(1);
	//romanPots_2->SetFillStyle(0);
	
	//Roman Pots 1
	TBox * romanPots_1 = new TBox(3254.73, -126.119, 3258.73, -100.119);
	
	romanPots_1->SetLineColor(kBlack);
	romanPots_1->SetLineWidth(1);
	romanPots_1->SetFillStyle(0);
	
	//Roman Pots 2
	TBox * romanPots_2 = new TBox(3424.55, -133.843, 3428.55, -107.843);
	
	romanPots_2->SetLineColor(kBlack);
	romanPots_2->SetLineWidth(1);
	romanPots_2->SetFillStyle(0);
	
	TBox * neutralExitWindow = new TBox(2964.70, -74.1765-(2966.70*TMath::Tan(0.004)), 2968.70, -74.1765+(2966.70*TMath::Tan(0.004)));
	
	neutralExitWindow->SetLineColor(kBlack);
	neutralExitWindow->SetLineWidth(1);
	neutralExitWindow->SetFillStyle(0);
	
	
	detectorLabels[4] = new TPaveText(3200, -170, 3700, -150, "NB");
	//detectorLabels[4]->SetFillStyle(0);
	detectorLabels[4]->SetFillColor(0);
	detectorLabels[4]->SetBorderSize(0);
	detectorLabels[4]->AddText("Roman Pots");
	
	//Roman Pots 1
	TBox * romanPots_1_x_y = new TBox(-96.0, -7.0, -70, 7.0);
	
	romanPots_1_x_y->SetLineColor(kBlack);
	romanPots_1_x_y->SetLineWidth(2);
	romanPots_1_x_y->SetFillStyle(0);
	
	//Roman Pots 2
	TBox * romanPots_2_x_y = new TBox(-110.0, -7.0, -83.0, 7.0);
	
	romanPots_2_x_y->SetLineColor(kBlack);
	romanPots_2_x_y->SetLineWidth(2);
	romanPots_2_x_y->SetFillStyle(0);
	
	
	///////////////////



	double yRangeMin = 0.0;
	double yRangeMax = 0.0;

	int yRangeBinMin = dose_EM_3D->GetYaxis()->FindBin(0.0);
	int yRangeBinMax = dose_EM_3D->GetYaxis()->FindBin(0.0);

	TString yRangeString;
	
	yRangeString.Form(" #rightarrow %.2f < y < %.2f cm (%d bin)", dose_EM_3D->GetYaxis()->GetBinCenter(yRangeBinMin) - 0.5*dose_EM_3D->GetYaxis()->GetBinWidth(yRangeBinMin), 
											  dose_EM_3D->GetYaxis()->GetBinCenter(yRangeBinMax) + 0.5*dose_EM_3D->GetYaxis()->GetBinWidth(yRangeBinMax),
											  yRangeBinMax - yRangeBinMin + 1);
	
	yRangeString = mainPlotTitle + yRangeString;
	
	TPaveText * mainTitleText2 = new TPaveText(0.1, 0.91, 0.85, 0.99, "NB NDC");
	mainTitleText2->SetFillColor(0);
	mainTitleText2->AddText(yRangeString);

	//TCanvas * fullRegionCanvas = new TCanvas("can5", "can5", 1500, 1000);


	//fullRegionCanvas->cd(1)->SetRightMargin(0.15);
	//fullRegionCanvas->cd(1)->SetLogz();
	
	dose_EM_3D->GetYaxis()->SetRange(dose_EM_3D->GetYaxis()->FindBin(0.0),dose_EM_3D->GetYaxis()->FindBin(0.0));
	dose_EM_3D->GetZaxis()->SetRange(dose_EM_3D->GetZaxis()->FindBin(twoDPlotZMin),dose_EM_3D->GetZaxis()->FindBin(twoDPlotZMax));
	

	TH2F* slice_x_z = (TH2F*) dose_EM_3D->Project3D("xz");
	
	//double x_location_min = -100.0;
	//double x_location_max = 100.0;
	
	//double y_location_min = 100.0;
	//double y_location_max = 100.0;
	
	dose_EM_3D->GetXaxis()->SetRange(dose_EM_3D->GetXaxis()->FindBin(x_location_min),dose_EM_3D->GetXaxis()->FindBin(x_location_max));
	dose_EM_3D->GetYaxis()->SetRange(dose_EM_3D->GetYaxis()->FindBin(y_location_min),dose_EM_3D->GetYaxis()->FindBin(y_location_max));
	dose_EM_3D->GetZaxis()->SetRange(dose_EM_3D->GetZaxis()->FindBin(z_location),dose_EM_3D->GetZaxis()->FindBin(z_location));
	

	TH2F* slice_x_y = (TH2F*) dose_EM_3D->Project3D("yx");
	
	//TH2F* slice_x_z = (TH2F*) dose_EM_3D->Project3DProfile("xz");
	
	
	slice_x_z->GetZaxis()->SetLabelSize(0.045);
	slice_x_z->GetZaxis()->SetTitle(zAxisTitle_full_IR);
	slice_x_z->GetZaxis()->SetTitleOffset(1.4);
	slice_x_z->SetNameTitle(yRangeString, "");
	
	slice_x_z->SetStats(0);
	slice_x_z->SetMinimum(topDownPlotMinimum);
	if(topDownPlotMaximum != 0.0){slice_x_z->SetMaximum(topDownPlotMaximum);}

   
	slice_x_z->GetZaxis()->SetTitleOffset(1.4);
	slice_x_z->GetXaxis()->SetTitle("Z [cm]");
	slice_x_z->GetYaxis()->SetTitle("X [cm]");
	
	
	slice_x_y->GetZaxis()->SetLabelSize(0.045);
	slice_x_y->GetZaxis()->SetTitle(zAxisTitle_full_IR);
	slice_x_y->GetZaxis()->SetTitleOffset(1.4);
	slice_x_y->SetNameTitle(yRangeString, "");
	
	slice_x_y->SetStats(0);
	slice_x_y->SetMinimum(topDownPlotMinimum);
	if(topDownPlotMaximum != 0.0){slice_x_y->SetMaximum(topDownPlotMaximum);}

   
	slice_x_y->GetZaxis()->SetTitleOffset(1.4);
	slice_x_y->GetXaxis()->SetTitle("X [cm]");
	slice_x_y->GetYaxis()->SetTitle("Y [cm]");

	
	TCanvas * canvas_main = new TCanvas("canv1", "canv1", 1800, 650);
	canvas_main->Divide(2,1);

	canvas_main->cd(1);
	canvas_main->cd(1)->SetRightMargin(0.15);
	canvas_main->cd(1)->SetLogz();
	slice_x_z->SetTitle(mainPlotTitle);
	slice_x_z->Draw("COLZ");
	
	if(twoDPlotZMin < -600.0){
		lumiExitWindow->Draw("SAME");
		lumiCollimator->Draw("SAME");
		lumiSweeper->Draw("SAME");
		lumiAnalyzer->Draw("SAME");
		lumiDirectPCAL->Draw("SAME");
		lowQ2Tager_1->Draw("SAME");
		lowQ2Tager_2->Draw("SAME");
	}
	
	//ZDCBox->Draw("SAME");
	barrelEMCALBox_top->Draw("SAME");
	barrelEMCALBox_bottom->Draw("SAME");
	hcalInsertBox_top->Draw("SAME");
	hcalInsertBox_bottom->Draw("SAME");
	hcalForwardBox_top->Draw("SAME");
	hcalForwardBox_bottom->Draw("SAME");
	nECALBox_top->Draw("SAME");
	nECALBox_bottom->Draw("SAME");
	bHCALBox_top->Draw("SAME");
	bHCALBox_bottom->Draw("SAME");
	b0Box->Draw("SAME");
	romanPots_1->Draw("SAME");
	romanPots_2->Draw("SAME");
	neutralExitWindow->Draw("SAME");
	
	detectorLabels[0]->Draw("SAME");
	detectorLabels[1]->Draw("SAME");
	detectorLabels[2]->Draw("SAME");
	detectorLabels[3]->Draw("SAME");
	detectorLabels[4]->Draw("SAME");
	detectorLabels[5]->Draw("SAME");
	detectorLabels[6]->Draw("SAME");
	
	if(twoDPlotZMin < -600.0){
		detectorLabels[7]->Draw("SAME");
		detectorLabels[8]->Draw("SAME");
		detectorLabels[9]->Draw("SAME");
		detectorLabels[10]->Draw("SAME");
		detectorLabels[11]->Draw("SAME");
	}
	
	canvas_main->cd(2);
	canvas_main->cd(2)->SetRightMargin(0.15);
	canvas_main->cd(2)->SetLogz();
	slice_x_y->SetTitle(mainPlotTitle);
	slice_x_y->Draw("COLZ");
	
	
	//mainTitleText2->Draw("SAME");
	
	TString twoDPlot = "2D_slice_zx_and_xy_";
	TString radiationType; 
	radiationType.Form("%s_%s_coordinates_%.1f_x_%.1f_%.1f_y_%.1f_at_z_%.1f", radiationSource.Data(), sourceParticle.Data(),
		x_location_min, x_location_max, y_location_min, y_location_max, z_location);
	
	
	twoDPlot = twoDPlot + radiationType + pdfFile;

	canvas_main->SaveAs(twoDPlot);

	
}

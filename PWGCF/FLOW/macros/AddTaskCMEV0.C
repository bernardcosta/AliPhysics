#include<TList.h>
#include "TSystem.h"
class AliAnalysisTaskCMEV0;


 //void AddTaskCMEV0(Double_t dcentrMin=0, Double_t dcentrMax=90, TString sAnalysisFile = "AOD", TString sDataSet = "2010", 
 AliAnalysisTaskCMEV0* AddTaskCMEV0(Double_t dcentrMin=0, Double_t dcentrMax=90., Int_t gFilterBit = 768, Int_t gClusterTPC = 70, 
 Double_t fpTLow = 0.2, Double_t fpTHigh = 10.0, Double_t fEtaLow = -0.8, Double_t fEtaHigh = 0.8, Double_t dDCAxy = 2.4, Double_t dDCAz  = 3.2,
 TString sAnalysisFile = "AOD", TString sDataSet = "2010", TString sAnalysisType = "AUTOMATIC", TString sEventTrigger = "MB", Bool_t bEventCutsQA = kFALSE, 
 Bool_t bTrackCutsQA = kFALSE, Double_t dVertexRange = 10., Bool_t bPileUp = kFALSE, Bool_t bPileUpTight = kFALSE, TString sCentEstimator = "V0", 
 Bool_t bFBeffi = kFALSE, TString sEfficiencyFB = "alien:///alice/cern.ch/user/m/mhaque/calib_files/FB768_Hijing_LHC15o.root",
 Bool_t bApplyNUA= kFALSE, TString sNUAFile="alien:///alice/cern.ch/user/m/mhaque/gain/Run2015o_Pass1_FB768_pT0p2_5GeV_NUA_Wgt_PosNeg_Run.root", 
 Bool_t bZDCGainEq= kFALSE, TString sZDCFile="alien:///alice/cern.ch/user/m/mhaque/gain/Run2015o_pass1_ZDNP_WgtTotEn_VsCentRun.root", 
 Bool_t bFillTPCQn=kFALSE, Bool_t bFillNUAhist= kFALSE, Bool_t bApplyNUACorr = kFALSE, Float_t fSetHarmonic = 2.0,  Bool_t bUseNUAinEP = kFALSE,
 const char *suffix = "")
{

  //gSystem->Load("libPWGflowBase.so");
  //gSystem->Load("libPWGflowTasks.so");

  gSystem->AddIncludePath("-I. -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include -g ");
  gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/EMCAL -I$ALICE_ROOT/ANALYSIS -I$ALICE_ROOT/OCDB -I$ALICE_ROOT/STEER/macros -I$ALICE_ROOT/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/TRD -I$ALICE_ROOT/ZDC -I$ALICE_ROOT/macros -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_PHYSICS/OADB $ALICE_PHYSICS/OADB/macros -I$ALICE_PHYSICS/PWGGA -I$ALICE_PHYSICS/PWGCF -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/TENDER -I$ALICE_PHYSICS/TENDER/Tender -I$ALICE_PHYSICS/TENDER/TenderSupplies -I$ALICE_PHYSICS/PARfiles -I$ALICE_PHYSICS/PWGCF/FLOW/macros I$ALICE_PHYSICS/PWGPP/ZDC -g ");

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  //gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"); // Needed for LHC2015o
  //AliMultSelectionTask *task = AddTaskMultSelection(kFALSE);            // kFALSE == User mode, kTRUE == Calibration mode
  //task->SetSelectedTriggerClass(AliVEvent::kINT7); //kMB ?                     
  //mgr->AddTask(task);


  TString taskFEname;
  taskFEname.Form("ZDCFlowEventTask%s", suffix);

  AliAnalysisTaskFlowEvent *taskFE = new AliAnalysisTaskFlowEvent(taskFEname,"",bEventCutsQA);

  taskFE->SetQAOn(bEventCutsQA);
  taskFE->SetAnalysisType(sAnalysisType); //sanalysisType = AUTOMATIC see the initializers!!

  if(sDataSet=="2015"){
    taskFE->SelectCollisionCandidates(AliVEvent::kINT7);
  }
  else{
    taskFE->SelectCollisionCandidates(AliVEvent::kMB);
  }

 
  taskFEname.Form("EventCuts%s", suffix);

  AliFlowEventCuts *cutsEvent = new AliFlowEventCuts(taskFEname);
  cutsEvent->SetCheckPileup(kFALSE);
  cutsEvent->SetPrimaryVertexZrange(-dVertexRange, dVertexRange);      // vertex-z cut
  cutsEvent->SetQA(bEventCutsQA);                                      // enable the qa plots
  cutsEvent->SetCutTPCmultiplicityOutliersAOD(kTRUE); 	               // multiplicity outlier cut

  if(sDataSet=="2015")
  {
   cutsEvent->SetCentralityPercentileRange(dcentrMin, dcentrMax, kTRUE);
  }
  else
  {
   cutsEvent->SetCentralityPercentileRange(dcentrMin, dcentrMax);
   cutsEvent->SetCheckPileup(kTRUE);
  }

  if(sCentEstimator=="V0")
    cutsEvent->SetCentralityPercentileMethod(AliFlowEventCuts::kV0);

  if(sCentEstimator=="TPC")
    cutsEvent->SetCentralityPercentileMethod(AliFlowEventCuts::kTPConly);

  if(sDataSet == "2011"){
    cutsEvent->SetLHC11h(kTRUE);
  }
  else if(sDataSet == "2010"){
    cutsEvent->SetLHC10h(kTRUE);
  }


  taskFEname.Form("RefMultCuts%s", suffix);

  AliFlowTrackCuts* RefMultCuts = new AliFlowTrackCuts(taskFEname);
  RefMultCuts->SetParamType(AliFlowTrackCuts::kAODFilterBit);
  RefMultCuts->SetAODfilterBit(gFilterBit);
  RefMultCuts->SetMinimalTPCdedx(-99999);
  RefMultCuts->SetMaxDCAToVertexXY(dDCAxy);
  RefMultCuts->SetMaxDCAToVertexZ(dDCAz);
  RefMultCuts->SetMinNClustersTPC(gClusterTPC);
  RefMultCuts->SetMinChi2PerClusterTPC(0.1);
  RefMultCuts->SetMaxChi2PerClusterTPC(4.);
  RefMultCuts->SetPtRange(fpTLow,fpTHigh);
  RefMultCuts->SetEtaRange(fEtaLow,fEtaHigh);
  RefMultCuts->SetAcceptKinkDaughters(kFALSE);

  cutsEvent->SetRefMultCuts(RefMultCuts);
  cutsEvent->SetRefMultMethod(AliFlowEventCuts::kTPConly);


  taskFE->SetCutsEvent(cutsEvent);	//pass these cuts to your flow event task

  taskFEname.Form("RP_cuts_%s", suffix);

  AliFlowTrackCuts *cutsRP  =  new AliFlowTrackCuts(taskFEname);
  cutsRP->SetParamType(AliFlowTrackCuts::kAODFilterBit);  //sets how we want to select the tracks.
  cutsRP->SetAODfilterBit(gFilterBit);                    
  cutsRP->SetPhiMin(0.);
  cutsRP->SetPhiMax(TMath::TwoPi());
  cutsRP->SetDivSigma(kTRUE);
  cutsRP->SetQA(bTrackCutsQA);
  cutsRP->SetMinimalTPCdedx(-99999);
  cutsRP->SetMinNClustersTPC(gClusterTPC);
  cutsRP->SetPtRange(fpTLow,fpTHigh);
  cutsRP->SetEtaGap(-2.0,2.0);  
  cutsRP->SetEtaRange(-5.0,5.0);
  cutsRP->SetRequireCharge(kTRUE);
  cutsRP->SetMinChi2PerClusterTPC(0.1);
  cutsRP->SetMaxChi2PerClusterTPC(4.0);
  cutsRP->SetAcceptKinkDaughters(kFALSE);

  taskFEname.Form("POI_cuts_%s", suffix);
  AliFlowTrackCuts *cutsPOI   = new AliFlowTrackCuts(taskFEname);
  cutsPOI->SetParamType(AliFlowTrackCuts::kAODFilterBit);
  cutsPOI->SetAODfilterBit(gFilterBit);
  cutsPOI->SetMinimalTPCdedx(-99999);
  cutsPOI->SetMinNClustersTPC(gClusterTPC);
  cutsPOI->SetPhiMin(0.);
  cutsPOI->SetPhiMax(TMath::TwoPi());
  cutsPOI->SetPtRange(fpTLow,fpTHigh);
  cutsPOI->SetEtaRange(fEtaLow,fEtaHigh);
  cutsPOI->SetRequireCharge(kTRUE);
  cutsPOI->SetQA(bTrackCutsQA);
  cutsPOI->SetMinChi2PerClusterTPC(0.1);
  cutsPOI->SetMaxChi2PerClusterTPC(4.0);
  cutsPOI->SetAcceptKinkDaughters(kFALSE);

  taskFE->SetCutsRP(cutsRP);
  taskFE->SetCutsPOI(cutsPOI);
  taskFE->SetSubeventEtaRange(-5.0,-2.0, 2.0,5.0);


  // Finally add the task to the manager
  mgr->AddTask(taskFE);


  TString file = AliAnalysisManager::GetCommonFileName();

  TString ContainerFE;
  ContainerFE.Form("FlowEventCont_%s", suffix);


  AliAnalysisDataContainer *coutputFE = mgr->CreateContainer(ContainerFE,AliFlowEventSimple::Class(),AliAnalysisManager::kExchangeContainer);
  AliAnalysisDataContainer    *cinput = mgr->GetCommonInputContainer();  //AOD event

  mgr->ConnectInput( taskFE, 0, cinput); 	//connect the input data (AOD) to the flow event task
  mgr->ConnectOutput(taskFE, 1, coutputFE); 	//get the output of taskFE to a exchange container.




  TString taskFEQA = file;      // file is the common outfile filename
  taskFEQA   += ":QAcharge";
  //taskFEQA   += suffix;    should I do this or not?

  TString ContainerFEQA;
  ContainerFEQA.Form("FEContQA_%s", suffix);

  AliAnalysisDataContainer *coutputFEQA = mgr->CreateContainer(ContainerFEQA,TList::Class(),AliAnalysisManager::kOutputContainer,taskFEQA.Data());
  mgr->ConnectOutput(taskFE, 2, coutputFEQA);          // kOutputContainer: written to the output file


  //==========================================================================================

  TString TaskZDCflow;
  TaskZDCflow.Form("TaskZDCFlow_%s", suffix);

  AliAnalysisTaskCMEV0 *taskQC_prot = new AliAnalysisTaskCMEV0(TaskZDCflow);

  if(sDataSet == "2015")
  {
    taskQC_prot->SelectCollisionCandidates(AliVEvent::kINT7);
  }
  else{
    taskQC_prot->SelectCollisionCandidates(AliVEvent::kMB);
  }

  taskQC_prot->SetHarmonic(fSetHarmonic);  
  taskQC_prot->SetCentEstimator(sCentEstimator);
  taskQC_prot->SetRejectPileUp(bPileUp);  
  taskQC_prot->SetRejectPileUpTight(bPileUpTight); //kTRUE:700,kFALSE:15000
  taskQC_prot->SetDataSet(sDataSet);   
//taskQC_prot->SetAnalysisSet(sAnalysisDef); 
  taskQC_prot->SetApplyNUACorr(bApplyNUACorr);
  taskQC_prot->SetStoreTPCQnAvg(bFillTPCQn);
  taskQC_prot->SetFillNUAHist(bFillNUAhist);
  taskQC_prot->SetApplyZDCCorr(bZDCGainEq);
  taskQC_prot->SetApplyNUAinEP(bUseNUAinEP);


  if(bFBeffi){
    TFile* FileFBeffi  = TFile::Open(sEfficiencyFB,"READ");
    TList* FBEffiListUse = dynamic_cast<TList*>(FileFBeffi->FindObjectAny("fMcEffiHij"));
     
    if(FBEffiListUse) {
     std::cout<<"\n\n Efficiency Histograms found\n:"<<FBEffiListUse->ls()<<"\n\n"<<std::endl;
       taskQC_prot->SetFBEfficiencyList(FBEffiListUse);
    }
    else{
       std::cout<<"\n\n !!!!**** ERROR: FB Efficiency Histograms not found  *****\n\n"<<std::endl;
      //exit(1);
    }
  }

  if(bApplyNUA){
     TFile* fNUAFile = TFile::Open(sNUAFile,"READ");
     if(!fNUAFile) {
       printf("\n\n *** ERROR: NUA wgt file not found! **EXIT** \n\n");
       exit(1);
     } 
     TList* fListNUA = dynamic_cast<TList*>(fNUAFile->FindObjectAny("fNUA_ChPosChNeg"));
     if(fListNUA){
       taskQC_prot->SetInputListNUA(fListNUA);
     }
     else{
       printf("\n\n *** ERROR: NUA wgt List not found! **EXIT** \n\n");
       return NULL;
     }
  }

  if(bZDCGainEq){
     TFile* fZDCGainFile = TFile::Open(sZDCFile,"READ");
     if(!fZDCGainFile) {
       printf("\n\n *** ERROR: ZDC wgt file not found! **EXIT** \n\n");
       exit(1);
     } 

     TList* fZDCWgtUse = dynamic_cast<TList*>(fZDCGainFile->FindObjectAny("fZDN_ZDP_Wgts"));
  
     if(fZDCWgtUse) {
       taskQC_prot->SetGainCorrZDNP(fZDCWgtUse);
     }
     else{
       printf("\n\n !!!!**** ERROR:ZDC Channel wgt List not found **EXIT**!!!\n\n");
       exit(1);
     }
  }


  /*
  if(bApplyRecent){
     TFile* fRecentFile = TFile::Open(sRecentFile,"READ");
     if(!fRecentFile) {
       printf("\n\n **** ERROR: ZDC Recenter file not found! **EXIT** \n\n");
       exit(1);
     }

     TList* fZDCRecentUse = dynamic_cast<TList*>(fRecentFile->FindObjectAny("recenterZDC"));
  
     if(fZDCRecentUse) {
       taskQC_prot->SetZDCESEList(fZDCRecentUse);
     }
     else{
       printf("\n\n !!!!**** ERROR:ZDC Recenter Histograms not found **EXIT**!!!\n\n");
       exit(1);
     }

  }//if(bApplyRecent)
*/

















  mgr->AddTask(taskQC_prot);            // connect the task to the analysis manager
  mgr->ConnectInput(taskQC_prot, 0, cinput);      // give AOD event to my Task..!!
  mgr->ConnectInput(taskQC_prot, 1, coutputFE);   // give FlowEvent object to my Task..!!

 //mgr->ConnectInput(taskQC_prot, 2,(AliAnalysisDataContainer*) mgr->GetContainers()->FindObject("ZDCEPExchangeContainer"));


















  TString outputSP = file;      // file is the common outfile filename
  outputSP += ":ZDCgains";
  //outputSP   += suffix;    should I do this or not?

  TString fZDCCont1;
  fZDCCont1.Form("ResultsZDCCME_%s", suffix);

  AliAnalysisDataContainer *coutputSP1 = mgr->CreateContainer(fZDCCont1,TList::Class(),AliAnalysisManager::kOutputContainer,outputSP.Data());
  mgr->ConnectOutput(taskQC_prot, 1, coutputSP1);

  TString fZDCCont2;
  fZDCCont2.Form("QAandCalibHist_%s", suffix);

  AliAnalysisDataContainer *coutputSP2 = mgr->CreateContainer(fZDCCont2,TList::Class(),AliAnalysisManager::kOutputContainer,outputSP.Data());
  mgr->ConnectOutput(taskQC_prot, 2, coutputSP2);

  //if(!mgr->InitAnalysis())  // check if we can initialize the manager
  //{
  //  return;
  //}


  return taskQC_prot;

   printf("\n\n ***** AddTask Configured properly. *******\n\n");

}//main ends

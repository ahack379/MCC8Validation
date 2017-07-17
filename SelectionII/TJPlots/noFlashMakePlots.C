#include "TCanvas.h"
#include "TFile.h"
#include "TMath.h"
#include "THStack.h"

int n = 0;
const int nsamples = 4;
const int ncomps = 14;
//const int ncomps = 12;

float tot_on = 31403; // 525613; //341100; //341493; // 547616
float tot_off = 22369 ; //379202; //377273; //370896; // 382718
float tot_bnbcos = 38850; //101600; //101850;  // 191362
float tot_intime = 25874; //32;  // 4804

float scale_intime = 10.279 ; // 5.12
float scale_onoffbeam = 1.23 * (382718./tot_off) * (tot_on/547616.) ;

float int_per_1e20POT = 99035.2 ; 
float dataPOT = 0.495 * (tot_on) / 547616  ; // Currently off by roughly 20%, excluding MEC 0.495
float mcbnbcos_POT = float(tot_bnbcos)/int_per_1e20POT ; // * dataPOT ; // 2.3

//sample 0 = OnBeam
//sample 1 = OffBeam
//sample 2 = MCBNBCos
//sample 3 = MCCosInTime

//void plot(TFile *f, char *name){
void plot(TFile *f, std::string name0){
  
  auto name = name0.c_str() ;
  TCanvas *can = new TCanvas(Form("can_%d",n),name);
  ++n;

  TH1F *h[nsamples][ncomps];
  for (int i = 0; i<nsamples; ++i){
    for (int j = 0; j<ncomps; ++j){
      h[i][j] = (TH1F*)f->Get(Form("%s_%d_%d",name,i,j));

      if (i==2) h[i][j]->Scale(dataPOT/mcbnbcos_POT); //0.495/2.3);
      if (i==3) h[i][j]->Scale(dataPOT/mcbnbcos_POT * scale_intime * tot_bnbcos/tot_intime) ; //0.495/2.3*5.12*191362/4804);
    }
  }

  std::cout<<"Entries: "<<h[0][0]->GetEntries()<<std::endl ;
  std::cout<<"Entries: "<<h[1][0]->GetEntries()*scale_onoffbeam<<std::endl ;
  std::cout<<"Entries: "<<h[2][0]->GetEntries()<<std::endl ;

  //if (strcmp(name,"hVtxx")==0){
  //  cout<<"Data "<<h[0][0]->Integral()<<"-"<<scale_onoffbeam*h[1][0]->Integral()<<"="<<h[0][0]->Integral()-scale_onoffbeam*h[1][0]->Integral()<<endl;
  //  cout<<"MC "<<h[2][0]->Integral()<<endl;
  //  cout<<"Cosmic "<<h[2][1]->Integral()<<endl;
  //  cout<<"Out of FV nu "<<h[2][2]->Integral()<<endl;
  //  cout<<"NC "<<h[2][3]->Integral()<<endl;
  //  cout<<"nue "<<h[2][4]->Integral()<<endl;
  //  cout<<"nubar "<<h[2][5]->Integral()<<endl;
  //  cout<<"CCQ "<<h[2][6]->Integral()<<endl;
  //  cout<<"CCRES "<<h[2][7]->Integral()<<endl;
  //  cout<<"CCDIS "<<h[2][8]->Integral()<<endl;
  //  cout<<"CCCoh "<<h[2][9]->Integral()<<endl;
  //  cout<<"Intime cosmic "<<h[3][0]->Integral()<<endl;
  //  cout<<"Efficiency "<<(h[2][6]->Integral()+h[2][7]->Integral()+h[2][8]->Integral()+h[2][9]->Integral())/(41261.*0.495/2.3)<<endl;
  //  cout<<"Purity "<<(h[2][6]->Integral()+h[2][7]->Integral()+h[2][8]->Integral()+h[2][9]->Integral())/(h[2][0]->Integral()+h[3][0]->Integral())<<endl;
  //}

  std::cout<<"\nEntries: "<<h[0][0]->GetEntries() - h[1][0]->GetEntries()*scale_onoffbeam<<std::endl ;
  std::cout<<"Entries: "<<h[2][0]->GetEntries()*dataPOT/mcbnbcos_POT<<std::endl ;
  h[0][0]->Add(h[1][0],-scale_onoffbeam);

  h[0][0]->SetMarkerStyle(20);
  h[0][0]->SetMarkerSize(1);
  h[0][0]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][0]->GetMaximum(), h[2][0]->GetMaximum())*1.2);
  //if (strcmp(name,"hTrackPhi")==0){
  if (fabs(h[0][0]->GetMean()-(h[0][0]->GetXaxis()->GetXmin() + (h[0][0]->GetXaxis()->GetXmax() - h[0][0]->GetXaxis()->GetXmin())/2))<0.05*(h[0][0]->GetXaxis()->GetXmax() - h[0][0]->GetXaxis()->GetXmin())){
    h[0][0]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][0]->GetMaximum(), h[2][0]->GetMaximum())*2);
  }
  h[0][0]->DrawCopy();
  THStack *stack = new THStack(Form("stack_%s",name), h[0][0]->GetTitle());
  h[2][1]->SetFillColor(7);
  h[2][1]->SetLineColor(7);
  stack->Add(h[2][1]);
  h[2][3]->SetFillColor(6);
  h[2][3]->SetLineColor(6);
  stack->Add(h[2][3]);
  h[2][2]->SetFillColor(9);
  h[2][2]->SetLineColor(9);
  stack->Add(h[2][2]);
  h[2][4]->SetFillColor(9);
  h[2][4]->SetLineColor(9);
  stack->Add(h[2][4]);
  h[2][5]->SetFillColor(9);
  h[2][5]->SetLineColor(9);
  stack->Add(h[2][5]);
  h[2][6]->SetFillColor(5);
  h[2][6]->SetLineColor(5);
  stack->Add(h[2][6]);
  h[2][7]->SetFillColor(2);
  h[2][7]->SetLineColor(2);
  stack->Add(h[2][7]);
  h[2][8]->SetFillColor(3);
  h[2][8]->SetLineColor(3);
  stack->Add(h[2][8]);
  h[2][9]->SetFillColor(46);
  h[2][9]->SetLineColor(46);
  stack->Add(h[2][9]);
  h[2][12]->SetFillColor(30);
  h[2][12]->SetLineColor(30);
  stack->Add(h[2][12]);
  stack->Draw("hist same");
  h[0][0]->DrawCopy("same");
  gPad->RedrawAxis();
  //if (strcmp(name,"hTrackPhi")==0){
  if (fabs(h[0][0]->GetMean()-(h[0][0]->GetXaxis()->GetXmin() + (h[0][0]->GetXaxis()->GetXmax() - h[0][0]->GetXaxis()->GetXmin())/2))<0.05*(h[0][0]->GetXaxis()->GetXmax() - h[0][0]->GetXaxis()->GetXmin())){
    TLegend *leg = new TLegend(0.2,0.5,0.6,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][0], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][1], "Cosmic Background","f");
    //leg->AddEntry(h[2][3], "NC Background","f");
    leg->AddEntry(h[2][2], "Neutrino Interaction","f");
    //leg->AddEntry(h[2][6], "CCQE","f");
    //leg->AddEntry(h[2][7], "CCRes","f");
    //leg->AddEntry(h[2][8], "CCDIS","f");
    //leg->AddEntry(h[2][9], "CCCoh","f");
    //leg->AddEntry(h[2][12], "MEC","f");
    leg->Draw();
  }
  else if (h[0][0]->GetMean() > h[0][0]->GetXaxis()->GetXmin() + (h[0][0]->GetXaxis()->GetXmax() - h[0][0]->GetXaxis()->GetXmin())/2){
    TLegend *leg = new TLegend(0.2,0.5,0.6,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][0], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][1], "Cosmic Background","f");
    //leg->AddEntry(h[2][3], "NC Background","f");
    leg->AddEntry(h[2][2], "Neutrino Interaction","f");
    //leg->AddEntry(h[2][6], "CCQE","f");
    //leg->AddEntry(h[2][7], "CCRes","f");
    //leg->AddEntry(h[2][8], "CCDIS","f");
    //leg->AddEntry(h[2][9], "CCCoh","f");
    //leg->AddEntry(h[2][12], "MEC","f");
    leg->Draw();
  }
  else{
    TLegend *leg = new TLegend(0.5,0.5,0.9,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][0], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][1], "Cosmic Background","f");
    //leg->AddEntry(h[2][3], "NC Background","f");
    leg->AddEntry(h[2][2], "Neutrino Interaction","f");
    //leg->AddEntry(h[2][6], "CCQE","f");
    //leg->AddEntry(h[2][7], "CCRes","f");
    //leg->AddEntry(h[2][8], "CCDIS","f");
    //leg->AddEntry(h[2][9], "CCCoh","f");
    //leg->AddEntry(h[2][12], "MEC","f");
    leg->Draw();
  }

//  h[2][0]->Scale(0.495/2.3);
//  //h[2][0]->Scale(h[0][0]->Integral()/h[2][0]->Integral());
//  h[0][0]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][0]->GetMaximum(), h[2][0]->GetMaximum())*1.2);
//  h[0][0]->DrawCopy();
//  h[2][0]->SetLineColor(2);
//  h[2][0]->DrawCopy("same hist");
  can->Print(Form("%s.png",name));  

}

//void plot2(TFile *f, char *name){
void plot2(TFile *f, std::string name0){
  
  auto name = name0.c_str() ;
  TCanvas *can1 = new TCanvas(Form("can_%d",n),name);
  ++n;

  TH1F *h[nsamples][ncomps];
  for (int i = 0; i<nsamples; ++i){
    for (int j = 0; j<ncomps; ++j){
      h[i][j] = (TH1F*)f->Get(Form("%s_%d_%d",name,i,j));
      if (i==2) h[i][j]->Scale(dataPOT/mcbnbcos_POT); //0.495/2.3);
    }
  }
  h[0][0]->DrawCopy();
  h[1][0]->Scale(scale_onoffbeam); //1.23);
  h[1][0]->SetLineColor(2);
  h[1][0]->SetMarkerColor(2);
  h[1][0]->DrawCopy("same");
  gPad->RedrawAxis();
//  h[0][0]->Add(h[1][0],-1.23);
//  h[2][0]->Scale(0.495/2.3);
//  //h[2][0]->Scale(h[0][0]->Integral()/h[2][0]->Integral());
  //h[0][0]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][0]->GetMaximum(), h[2][0]->GetMaximum())*1.2);
  //h[0][0]->DrawCopy();
  //h[2][0]->SetLineColor(2);
  //h[2][0]->DrawCopy("same hist");

  if (h[0][0]->GetMean() > h[0][0]->GetXaxis()->GetXmax()/2){
    TLegend *leg0 = new TLegend(0.2,0.6,0.6,0.9);
    leg0->SetFillStyle(0);
    leg0->AddEntry(h[0][0],"Beam On", "ple");
    leg0->AddEntry(h[1][0],"Beam Off","f");
    leg0->Draw();
  }
  else{
    TLegend *leg0 = new TLegend(0.5,0.6,0.9,0.9);
    leg0->SetFillStyle(0);
    leg0->AddEntry(h[0][0],"Beam On", "f");
    leg0->AddEntry(h[1][0],"Beam Off","f");
    leg0->Draw();
    }

  can1->Print(Form("%s1.png",name));  
  std::cout<<"on off scale : "<<scale_onoffbeam<<std::endl; 

  TCanvas *can2 = new TCanvas(Form("can_%d",n),name);
  ++n;
  h[0][0]->Add(h[1][0],-1);//scale_onoffbeam);
  h[0][0]->SetMarkerStyle(20);
  h[0][0]->SetMarkerSize(1);
  h[0][0]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][0]->GetMaximum(), h[2][0]->GetMaximum())*1.2);
  h[0][0]->DrawCopy();
  THStack *stack = new THStack(Form("stack_%s",name), h[0][0]->GetTitle());
  h[2][1]->SetLineColor(7);
  h[2][1]->SetFillColor(7);
  stack->Add(h[2][1]);
  h[2][2]->SetLineColor(2);
  h[2][2]->SetFillColor(2);
  stack->Add(h[2][2]);
  stack->Draw("histsame");
  h[0][0]->DrawCopy("same");
  gPad->RedrawAxis();

  if (h[0][0]->GetMean() > h[0][0]->GetXaxis()->GetXmax()/2){
    TLegend *leg2 = new TLegend(0.2,0.6,0.6,0.9);
    leg2->SetFillStyle(0);
    leg2->AddEntry(h[0][0],"Data: Beam On - Beam Off", "ple");
    leg2->AddEntry(h[2][1],"Cosmic background","f");
    leg2->AddEntry(h[2][2],"Neutrino signal","f");
    leg2->Draw();
  }
  else{
    TLegend *leg2 = new TLegend(0.5,0.6,0.9,0.9);
    leg2->SetFillStyle(0);
    leg2->AddEntry(h[0][0],"Data: Beam On - Beam Off", "ple");
    leg2->AddEntry(h[2][1],"Cosmic background","f");
    leg2->AddEntry(h[2][2],"Neutrino signal","f");
    leg2->Draw();
  }

  can2->Print(Form("%s2.png",name));  
   
}


//void plot3(TFile *f, char *name){
void plot3(TFile *f, std::string name0){
  
  auto name = name0.c_str() ;
  TCanvas *can = new TCanvas(Form("can_%d",n),name);
  TCanvas *can1 = new TCanvas(Form("can1_%d",n),name,800,400);
  can1->Divide(2,1);
  ++n;

  TH1F *h[nsamples][ncomps];
  for (int i = 0; i<nsamples; ++i){
    for (int j = 0; j<ncomps; ++j){
      h[i][j] = (TH1F*)f->Get(Form("%s_%d_%d",name,i,j));
      if (i==2) h[i][j]->Scale(dataPOT/mcbnbcos_POT); //0.495/2.3);
      if (i==3) h[i][j]->Scale(dataPOT/mcbnbcos_POT * scale_intime * tot_bnbcos/tot_intime) ; //0.495/2.3*5.12*191362/4804);
    }
  }
  can1->cd(1);
 //h[0][0]->Add(h[1][0],-scale_onoffbeam) ; 
  h[1][0]->Scale(scale_onoffbeam) ;
  h[1][0]->DrawCopy("colz"); //OffBeam
  gPad->RedrawAxis();
  can1->cd(2); 
  h[0][0]->DrawCopy("colz"); //OnBeam
  gPad->RedrawAxis();
  can1->Print(Form("%s1.png",name));  

  TCanvas *can2 = new TCanvas(Form("can_%d",n),name,800,400);
  can2->Divide(2,1);
  ++n;

  can2->cd(1);
  //h[2][2]->DrawCopy("colz"); //BNBCos, Cosmics
  h[3][0]->DrawCopy("colz"); // InTime (aka OffBeam)

  can2->cd(2);
  h[2][0]->Add(h[1][0],scale_onoffbeam) ; 
  h[2][0]->DrawCopy("colz"); //BNBCos // + InTime (aka OnBeam) 
  gPad->RedrawAxis();

  //h[2][1]->DrawCopy("colz"); //BNBCos, Neutrinos

  gPad->RedrawAxis();
  can2->Print(Form("%s2.png",name));  

}
void noFlashMakePlots(){

  std::cout<<"N OnBeam     : "<<tot_on<<std::endl ; 
  std::cout<<"N OffBeam    : "<<tot_off<<std::endl ;
  std::cout<<"N MCBNBCos   : "<<tot_bnbcos<<std::endl ;
  std::cout<<"N MCCosInTime: "<<tot_intime<<std::endl ;


  std::cout<<"Data POT  : "<<dataPOT<<std::endl ;
  std::cout<<"MCBNB POT : "<<mcbnbcos_POT<<std::endl ;
  std::cout<<"Scale InTime to MCBNB: "<<scale_intime * tot_bnbcos/tot_intime<<std::endl;
  std::cout<<"Scale Off to OnBeam    : "<<scale_onoffbeam<<std::endl ;
  std::cout<<"Scale MC to OnBeam     : "<<dataPOT/mcbnbcos_POT<<std::endl;
  std::cout<<"Scale InTime to OnBeam : "<<dataPOT/mcbnbcos_POT*scale_intime * tot_bnbcos/tot_intime<<std::endl;


  gStyle->SetOptStat(0);

  //TFile *f = new TFile("hist_pandoraNuPMA_pmtrack.root");
  //TFile *f = new TFile("FULLEST2hist_pandoraNu_pandoraNu.root");
  //TFile *f = new TFile("FULLESThistpandoraNu_pandoraNu.root");
  //TFile *f = new TFile("TightMult1hist_pandoraNu_pandoraNu.root");
  //TFile *f = new TFile("CCpi0hist_dataMCComp_fixFillsSTAT_v3.root");

  //TFile *f = new TFile("../FlashDist/CCpi0hist_dataMCComp_v4_flashtreev0_stats.root");
  //TFile *f = new TFile("../SpaceChargeStudy/CCpi0hist_dataMCComp_v6_meetingSuggestions_pandoraCosmic.root"); //CCpi0hist_dataMCComp_v4_flashtreev0_stats.root");
  //TFile *f = new TFile("SelectionII_OutputHists_NoFlash.root");
  //TFile *f = new TFile("SelectionII_OutputHists_Flashes_OpticalFixes.root");
  TFile *f = new TFile("SelectionII_OutputHists_pandoraNuPMA_NoFlash.root");

  // Stacked MC composition-of-signal vs Data plots
  plot(f,"hTrackLength");
  plot(f,"hTrackMul");
  plot(f,"hTrackCosz");
  plot(f,"hTrackPhi");
  //plot(f,"hVtxx");
  //plot(f,"hVtxy");
  //plot(f,"hVtxz");
  //plot(f,"hTrackStartx");
  //plot(f,"hTrackStarty");
  //plot(f,"hTrackStartz");
  //plot(f,"hTrackEndx");
  //plot(f,"hTrackEndy");
  //plot(f,"hTrackEndz");

  //// All Mult
  //plot2(f,"hMul");
  //// Mult 1
  //plot2(f,"hTrackLength1");
  //// Mult 2
  //plot2(f,"hTrackLength3");
  //plot2(f,"hCos1");
  //plot2(f,"hTrackEndy1");
  //// Mult 3
  //plot2(f,"hTrackLength2");
  //plot2(f,"hCos2");
  //// Mult > 1
  //plot2(f,"hCos3");

  //// 2D Hist Plots
  //plot3(f,"hSingleTrackLengthRatioVsdEdxRatio");
  //plot3(f,"hLongTrackdEdxStartEnd");
  //plot3(f,"hCos0VsLen1");
  //plot3(f,"hCosVsLen");
}

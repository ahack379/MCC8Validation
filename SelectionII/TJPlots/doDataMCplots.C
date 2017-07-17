#include "TCanvas.h"
#include "TFile.h"
#include "TMath.h"
#include "THStack.h"

int n = 0;
const int nsamples = 4;
const int ncomps = 14; //10;

float tot_on = 525613; //18264; //341493; // 547616
float tot_off = 379202; //17719; // 370896; // 38718
float tot_bnbcos = 101600; //24950; // 101850;  // 191362
float tot_intime = 25874; //25745;  // 4804

//float tot_on = 341493; // 547616
//float tot_off =  370896; // 38718
//float tot_bnbcos = 101850;  // 191362
//float tot_intime = 25874; //25745;  // 4804

float scale_intime = 10.279 ; // 5.12
float scale_onoffbeam = 1.23 * (382718./tot_off) * (tot_on/547616.) ;

float int_per_1e20POT = 99035.2 ; 
float dataPOT = 0.495 * (tot_on) / 547616 ; // 0.495
float mcbnbcos_POT = float(tot_bnbcos)/int_per_1e20POT ; // * dataPOT ; // 2.3

//sample 0 = OnBeam
//sample 1 = OffBeam
//sample 2 = MCBNBCos
//sample 3 = MCCosInTime

//void plot(TFile *f, char *name){
void plot(TFile *f, std::string name0,int ncomps_pass,int stage=0){
  
  std::cout<<"SCALE ON OFF BEAM : "<<scale_onoffbeam<<std::endl ;
  std::cout<<"Data POT : "<<dataPOT<<std::endl ;
  std::cout<<"MCBNB POT : "<<mcbnbcos_POT<<std::endl ;
  std::cout<<"Ratio : "<<dataPOT/mcbnbcos_POT<<std::endl;
  std::cout<<"InTimeScale: "<<dataPOT/mcbnbcos_POT*scale_intime * tot_bnbcos/tot_intime<<std::endl;

  auto name = name0.c_str() ;
  TCanvas *can = new TCanvas(Form("can_%d",n),name);
  ++n;

  TH1F *h[nsamples][ncomps_pass];
  for (int i = 0; i<nsamples; ++i){
    for (int j = 0; j<ncomps_pass; ++j){
      h[i][j] = (TH1F*)f->Get(Form("%s_%d_%d",name,i,j));
      if (i==2) h[i][j]->Scale(dataPOT/mcbnbcos_POT); //0.495/2.3);
      //if (i==3) h[i][j]->Scale(dataPOT/mcbnbcos_POT * scale_intime * tot_bnbcos/tot_intime) ; //0.495/2.3*5.12*191362/4804);
     }
  }

    h[0][stage]->Add(h[1][stage],-scale_onoffbeam);
    h[0][stage]->SetMarkerStyle(20);
    h[0][stage]->SetMarkerSize(1);
    h[0][stage]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][stage]->GetMaximum(), h[2][stage]->GetMaximum())*1.2);
    //if (strcmp(name,"hTrackPhi")==0){
    if (fabs(h[0][stage]->GetMean()-(h[0][stage]->GetXaxis()->GetXmin() + (h[0][stage]->GetXaxis()->GetXmax() - h[0][stage]->GetXaxis()->GetXmin())/2))<0.05*(h[0][stage]->GetXaxis()->GetXmax() - h[0][stage]->GetXaxis()->GetXmin())){
      h[0][stage]->GetYaxis()->SetRangeUser(0,TMath::Max(h[0][stage]->GetMaximum(), h[2][stage]->GetMaximum())*2);
    }
    h[0][stage]->DrawCopy();
    THStack *stack = new THStack(Form("stack_%s",name), h[0][stage]->GetTitle());

  if( ncomps_pass > 10 ){
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
  }
  else{
    stack->Add(h[2][stage]);
    h[2][stage]->SetFillColor(4);
    h[2][stage]->SetLineColor(4);
  }

  stack->Draw("hist same");
  h[0][stage]->DrawCopy("same");
  gPad->RedrawAxis();

  if (fabs(h[0][stage]->GetMean()-(h[0][stage]->GetXaxis()->GetXmin() + (h[0][stage]->GetXaxis()->GetXmax() - h[0][stage]->GetXaxis()->GetXmin())/2))<0.05*(h[0][stage]->GetXaxis()->GetXmax() - h[0][stage]->GetXaxis()->GetXmin())){
    TLegend *leg = new TLegend(0.2,0.5,0.6,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][stage], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][stage], "MC BNB Cos","f");
    leg->Draw();
  }
  else if (h[0][stage]->GetMean() > h[0][stage]->GetXaxis()->GetXmin() + (h[0][stage]->GetXaxis()->GetXmax() - h[0][stage]->GetXaxis()->GetXmin())/2){
    TLegend *leg = new TLegend(0.2,0.5,0.6,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][stage], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][stage], "MC BNB Cos","f");
    leg->Draw();
  }
  else{
    TLegend *leg = new TLegend(0.5,0.5,0.9,0.9);
    leg->SetFillStyle(0);
    leg->AddEntry(h[0][stage], "Data: Beam On - Beam Off", "ple");
    leg->AddEntry(h[2][stage], "MC BNB Cos","f");
    leg->Draw();
  }

  can->Print(Form("%s.png",name));  

}

void doDataMCplots(){

  gStyle->SetOptStat(0);

  TFile *f = new TFile("CCpi0hist_dataMCComp_fixFillsSTAT_v3.root"); //CCpi0hist_dataMCComp_v2pandoraNu_pandoraNu.root"); //hist_DataMCCompare.root");
  //TFile *f = new TFile("TightMult1hist_pandoraNu_pandoraNu.root"); //hist_DataMCCompare.root");

 // plot(f,"hTrackLength",14);
 // plot(f,"hTrackCosz",14);
 // plot(f,"hTrackPhi",14);
 // plot(f,"hVtxx",14);
 // plot(f,"hVtxy",14);
 // plot(f,"hVtxz",14);
 // plot(f,"hTrackStartx",14);
 // plot(f,"hTrackStarty",14);
 // plot(f,"hTrackStartz",14);
 // plot(f,"hTrackEndx",14);

  int stage = 2;

  plot(f,"hLenBefore",3,stage);
  plot(f,"hTrackCoszBefore",3,stage);
  plot(f,"hTrackStartxBefore",3,stage);
  plot(f,"hTrackStartyBefore",3,stage);
  plot(f,"hTrackStartzBefore",3,stage);
  plot(f,"hTrackEndxBefore",3,stage);
  plot(f,"hTrackEndyBefore",3,stage);
  plot(f,"hTrackEndzBefore",3,stage);
  plot(f,"hTrackPhiBefore",3,stage);
  plot(f,"hNTracksBefore",3,stage);
  //plot(f,"hNVtxBefore",3,stage);
  //plot(f,"hVtxxBefore",3,stage);
  //plot(f,"hVtxyBefore",3,stage);
  //plot(f,"hVtxzBefore",3,stage);

}

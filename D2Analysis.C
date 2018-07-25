#include <iostream>
#include "/mnt/c/rootbuild/include/RooRealVar.h"
#include "/home/pacejohn/home/Git/RestFrames/include/RestFrames/RestFrames.hh"
#include "/mnt/c/rootbuild/include/TTree.h"
#include "/mnt/c/rootbuild/include/TBranch.h"
#include "/mnt/c/rootbuild/include/TAttLine.h"
#include "/mnt/c/rootbuild/include/RooDataSet.h"
#include "/mnt/c/rootbuild/include/RooAddPdf.h"
#include "/mnt/c/rootbuild/include/RooPlot.h"
#include "/mnt/c/rootbuild/include/TCanvas.h"
#include "/mnt/c/rootbuild/include/TGraph.h"
#include "/mnt/c/rootbuild/include/RooNDKeysPdf.h"
#include "/mnt/c/rootbuild/include/TMultiGraph.h"
#include "/mnt/c/rootbuild/include/TGraphErrors.h"
#include "/mnt/c/rootbuild/include/TRandom3.h"

using namespace RooFit;
using namespace RestFrames;
using namespace std;

RooDataSet* recoDataA;
RooDataSet* recoDataB;
RooDataSet* recoDataC;
RooRealVar recoMassD1a;
RooRealVar recoCosD1a;

//Custom function for generating 2D PDFs since RooNDKeysPdf's default generate is SLOW.
//Uses a random number to determine which data set to pull from, then another random
//number to determine which data point from that set to pull.
RooDataSet* randomGenerate(RooDataSet* A, RooDataSet* B, RooDataSet* C,
                           double fitA, double fitB, double fitC, int Nevents)
{
  double fitNormalizer = fitA + fitB + fitC;
  const RooArgSet* holder;
  RooRealVar recoMassD1a("recoMassD1a", "recoMassD1a", 0, 1000);
  RooRealVar recoCosD1a("recoCosD1a", "recoCosD1a", -1, 1);
  RooDataSet* recoToy = new RooDataSet("recoToy", "recoToy", RooArgSet(recoMassD1a, recoCosD1a));
  TRandom3* gRandom = new TRandom3();
  long double rando = 0;
  int cat = 0;

  for (int i = 0; i < Nevents; i++)
  {
    TRandom3* gRandom = new TRandom3(0);
    rando = gRandom -> Rndm();
    //cout << "Random number generated for category: " << rando << endl;

    if (rando < (fitA / fitNormalizer))
    {cat = 0;}

    else if (rando < ((fitA + fitB) / fitNormalizer))
    {cat = 1;}

    else
    {cat = 2;}

    rando = gRandom -> Rndm();

    if (cat == 0)
    {
      holder = A -> get( int(rando * 1000) );
      recoToy -> add(*holder);
    }

    else if (cat == 1)
    {
      holder = B -> get( int(rando * 1000) );
      recoToy -> add(*holder);
    }

    else
    {
      holder = C -> get( int(rando * 1000) );
      recoToy -> add(*holder);
    }
    
    delete gRandom;
  }

  return(recoToy);
};

void D2Analysis()
{  

  TFile* fOpen = TFile::Open("/mnt/c/rootbuild/macros/test.root", "READ");
  
  recoDataA = (RooDataSet*) fOpen -> Get("recoDataA");
  recoDataB = (RooDataSet*) fOpen -> Get("recoDataB");
  recoDataC = (RooDataSet*) fOpen -> Get("recoDataC");

  //Values to be estimated
  RooRealVar recoMassD1a("recoMassD1a", "recoMassD1a", 0, 1000);
  RooRealVar recoCosD1a("recoCosD1a", "recoCosD1a", -1, 1);

  //Fit parameters for RooAddPdf and their true values
  RooRealVar fitA("fitA", "fitA", 0, 0, 1000000);
  RooRealVar fitB("fitB", "fitB", 0, 0, 1000000);
  RooRealVar fitC("fitC", "fitC", 0, 0, 1000000);

  double fitATrue = 30;
  double fitBTrue = 50;
  double fitCTrue = 20;

  //Normalization of fit parameters
  double parTrue[3];
  parTrue[0] = fitATrue / (fitATrue + fitBTrue + fitCTrue);
  parTrue[1] = fitBTrue / (fitATrue + fitBTrue + fitCTrue);
  parTrue[2] = fitCTrue / (fitATrue + fitBTrue + fitCTrue);

  //For generating toy data later
  int dataPts = 0;
  int toyExperiments = 0;
  int toyEvents = 0;
  int toyEventsHolder = 0;

  //Holder variables for rolling sums in error calculations
  double fAValHolder = 0;
  double fAErrHolder = 0;
  double fARatioHolder = 0;
  double fAPullHolder = 0;
  double fAPullErrHolder = 0;
  double fBValHolder = 0;
  double fBErrHolder = 0;
  double fBRatioHolder = 0;
  double fBPullHolder = 0;
  double fBPullErrHolder = 0;
  double fCValHolder = 0;
  double fCErrHolder = 0;
  double fCRatioHolder = 0;
  double fCPullHolder = 0;
  double fCPullErrHolder = 0;

  //Holder variables for RMS error calculations
  double fARatioHolder2 = 0;
  double fAPullHolder2 = 0;
  double fBRatioHolder2 = 0;
  double fBPullHolder2 = 0;
  double fCRatioHolder2 = 0;
  double fCPullHolder2 = 0;

  //Avgs to plot/feed to tree. Used in experiment loop.
  double fAValAvg = 0;
  double fAErrAvg = 0;
  double fARatioAvg = 0;
  double fAPullAvg = 0;
  double fAPullErrAvg = 0;
  double fBValAvg = 0;
  double fBErrAvg = 0;
  double fBRatioAvg = 0;
  double fBPullAvg = 0;
  double fBPullErrAvg = 0;
  double fCValAvg = 0;
  double fCErrAvg = 0;
  double fCRatioAvg = 0;
  double fCPullAvg = 0;
  double fCPullErrAvg = 0;

  //Values to store in tree branches. Used in event loop. Possibly useless.
  double fitAVal = 0;
  double fitAErr = 0;
  double fitARatio = 0; //Err over Val
  double fitAPull = 0;
  double fitBVal = 0; 
  double fitBErr = 0;
  double fitBRatio = 0; //Err over Val
  double fitBPull = 0;
  double fitCVal = 0; 
  double fitCErr = 0;
  double fitCRatio = 0; //Err over Val
  double fitCPull = 0;

  //Normalized fit parameters
  double parFit[3];

  //Errors from total differential
  double parErr[3];

  //Axes to be used for plotting later
  vector<double> x_ntoy;
  vector<double> y_ratio[3];
  vector<double> y_pull[3];
  vector<double> y_ratio_err[3];
  vector<double> y_pull_err[3];

  cout<<"How many toy experiments? "<<endl;
  cin>>toyExperiments;
  cout<<"How many events would you like to end up with? "<<endl;
  cin>>toyEvents;
  cout<<"How many data points per amount of events?"<<endl;
  cin>>dataPts;

  toyEventsHolder = toyEvents / toyExperiments;

  TFile fOut("D2AnalysisData.root", "RECREATE");

  //Array of 2D histograms for plotting later
  TH2D* histPull[3];
  for (int i = 0; i < 3; i++)
  {
    char* histName = new char[100];
    sprintf(histName, "histPull%d", i);
    histPull[i] = new TH2D(histName, histName, toyExperiments,
                           toyEventsHolder/2, toyEvents + toyEventsHolder/2,
                           64, -3, 3);
  }

  TTree* AnalysisData = new TTree("AnalysisData", "AnalysisData");

  //V - val, E - ratio error, R - ratio, PE - pull error, P - pull (you try typing all that out over again)
  TBranch* fAVBranch = AnalysisData -> Branch("fA_values", &fAValAvg);
  TBranch* fAEBranch = AnalysisData -> Branch("fA_ratio_errors", &fAErrAvg);
  TBranch* fARBranch = AnalysisData -> Branch("fA_ratios", &fARatioAvg);
  TBranch* fAPEBranch = AnalysisData -> Branch("fA_pull_errors", &fAPullErrAvg);
  TBranch* fAPBranch = AnalysisData -> Branch("fA_pulls", &fAPullAvg);
  TBranch* fBVBranch = AnalysisData -> Branch("fB_values", &fBValAvg);
  TBranch* fBEBranch = AnalysisData -> Branch("fB_ratio_errors", &fBErrAvg);
  TBranch* fBRBranch = AnalysisData -> Branch("fB_ratios", &fBRatioAvg);
  TBranch* fBPEBranch = AnalysisData -> Branch("fB_pull_errors", &fBPullErrAvg);
  TBranch* fBBranch = AnalysisData -> Branch("fB_pulls", &fBPullAvg);
  TBranch* fCVBranch = AnalysisData -> Branch("fC_values", &fCValAvg);
  TBranch* fCEBranch = AnalysisData -> Branch("fC_ratio_errors", &fCErrAvg);
  TBranch* fCRBranch = AnalysisData -> Branch("fC_ratios", &fCRatioAvg);
  TBranch* fCPEBranch = AnalysisData -> Branch("fC_pull_errors", &fCPullErrAvg);
  TBranch* fCPBranch = AnalysisData -> Branch("fC_pulls", &fCPullAvg);

  fOpen->cd();

  //Generate PDFs from each Decay/Reco dataset
  //THESE NEED TO TAKE SAME MASS/COS
  RooNDKeysPdf recoPDFA("recoPDFA", "PDF from Decay/Reco A", 
                         RooArgList(recoMassD1a, recoCosD1a), *recoDataA);
  RooNDKeysPdf recoPDFB("recoPDFB", "PDF from Decay/Reco B", 
                         RooArgList(recoMassD1a, recoCosD1a), *recoDataB);
  RooNDKeysPdf recoPDFC("recoPDFC", "PDF from Decay/Reco C", 
                         RooArgList(recoMassD1a, recoCosD1a), *recoDataC);

  //RooAddPdf from data-generated PDFs
  RooAddPdf recoPDF("recoPDF", "PDF from 3 Visible Particle Decay",
                     RooArgList(recoPDFA, recoPDFB, recoPDFC),
                     RooArgList(fitA, fitB, fitC));
  
  for (int i = 0; i < toyExperiments; i++)
  {
    for (int j = 0; j < dataPts; j++)
    {
      fitA.setVal(fitATrue);
      fitB.setVal(fitBTrue);
      fitC.setVal(fitCTrue);
      cout<< "EXPERIMENT: " << i << endl;
      cout<< "FIT A VAL PREFIT: " << fitA.getValV() << endl;
      cout<< "FIT B VAL PREFIT: " << fitB.getValV() << endl;
      cout<< "FIT C VAL PREFIT: " << fitC.getValV() << endl;
      
      RooDataSet* recoToy = randomGenerate(recoDataA, recoDataB, recoDataC, fitATrue, fitBTrue,      fitCTrue, toyEventsHolder);

      recoPDF.fitTo(*recoToy);

      cout<<"FIT A VAL POSTFIT: " << fitA.getValV() << endl;
      cout<<"FIT B VAL POSTFIT: " << fitB.getValV() << endl;
      cout<<"FIT C VAL POSTFIT: " << fitC.getValV() << endl;

      fitAVal = fitA.getValV();
      fitAErr = fitA.getError();
      fitBVal = fitB.getValV();
      fitBErr = fitB.getError();
      fitCVal = fitC.getValV();
      fitCErr = fitC.getError();

      parFit[0] = fitAVal / (fitAVal + fitBVal + fitCVal);
      parFit[1] = fitBVal / (fitAVal + fitBVal + fitCVal);
      parFit[2] = fitCVal / (fitAVal + fitBVal + fitCVal);

      //Total differential of fit equation f = f_a + f_b + f_c
      parErr[0] = sqrt( (fitBVal+fitCVal)*(fitBVal+fitCVal)*fitAErr*fitAErr +
		  fitAVal*fitAVal*( fitBErr*fitBErr + fitCErr*fitCErr ) )/
                  (fitAVal+fitBVal+fitCVal)/(fitAVal+fitBVal+fitCVal);
      parErr[1] = sqrt( (fitAVal+fitCVal)*(fitAVal+fitCVal)*fitBErr*fitBErr +
		  fitBVal*fitBVal*( fitAErr*fitAErr + fitCErr*fitCErr ) )/
                  (fitAVal+fitBVal+fitCVal)/(fitAVal+fitBVal+fitCVal);
      parErr[2] = sqrt( (fitBVal+fitAVal)*(fitBVal+fitAVal)*fitCErr*fitCErr +
		  fitCVal*fitCVal*( fitBErr*fitBErr + fitAErr*fitAErr ) )/
                  (fitAVal+fitBVal+fitCVal)/(fitAVal+fitBVal+fitCVal);

      parErr[0] = fitAErr / (fitAVal + fitBVal + fitCVal);
      parErr[1] = fitBErr / (fitAVal + fitBVal + fitCVal);
      parErr[2] = fitCErr / (fitAVal + fitBVal + fitCVal);

      fitARatio = fitAErr / fitAVal;
      fitBRatio = fitBErr / fitBVal;
      fitCRatio = fitCErr / fitCVal;

      fitAPull = (parFit[0] - parTrue[0]) / parErr[0];
      fitBPull = (parFit[1] - parTrue[1]) / parErr[1];
      fitCPull = (parFit[2] - parTrue[2]) / parErr[2];
  
      //Store nonerror quantities in rolling sum
      fAValHolder += fitAVal;
      fARatioHolder += fitARatio;
      fAPullHolder += fitAPull;
      fBValHolder += fitBVal;
      fBRatioHolder += fitBRatio;
      fBPullHolder += fitBPull;
      fCValHolder += fitCVal;
      fCRatioHolder += fitCRatio;
      fCPullHolder += fitCPull;

      //For use in later error calculations (maybe rename these to ErrHolders)
      fARatioHolder2 += fitARatio * fitARatio;
      fAPullHolder2 += fitAPull * fitAPull;
      fBRatioHolder2 += fitBRatio * fitBRatio;
      fBPullHolder2 += fitBPull * fitBPull;
      fCRatioHolder2 += fitCRatio * fitCRatio;
      fCPullHolder2 += fitCPull * fitCPull;
      
      for (int p = 0; p < 3; p++)
      {
        double fillVal = (parFit[p]-parTrue[p]) / parErr[p];
        cout<<"Fill Value: "<<fillVal<<endl;
        histPull[p] -> Fill(toyEventsHolder, fillVal);
      }

      delete recoToy;
    }

    x_ntoy.push_back(toyEventsHolder);

    //Determining avgs of nonerror quantities
    fAValAvg = fAValHolder / dataPts;
    fARatioAvg = fARatioHolder / dataPts;
    fAPullAvg = fAPullHolder / dataPts;
    fBValAvg = fBValHolder / dataPts;
    fBRatioAvg = fBRatioHolder / dataPts;
    fBPullAvg = fBPullHolder / dataPts;
    fCValAvg = fCValHolder / dataPts;
    fCRatioAvg = fCRatioHolder / dataPts;
    fCPullAvg = fCPullHolder / dataPts;

    //ErrAvg feels like clumsy name - RMS avg? Ask Dr. Rogan
    fAErrAvg = sqrt(fARatioHolder2/dataPts - fARatioAvg*fARatioAvg);
    fAPullErrAvg = sqrt(fAPullHolder2/dataPts - fAPullAvg*fAPullAvg); 
    fBErrAvg = sqrt(fBRatioHolder2/dataPts - fBRatioAvg*fBRatioAvg);
    fBPullErrAvg = sqrt(fBPullHolder2/dataPts - fBPullAvg*fBPullAvg); 
    fCErrAvg = sqrt(fCRatioHolder2/dataPts - fCRatioAvg*fCRatioAvg);
    fCPullErrAvg = sqrt(fCPullHolder2/dataPts - fCPullAvg*fCPullAvg); 

    //Store ratio and pull avgs and errors for plots
    y_ratio[0].push_back(fARatioAvg);
    y_ratio[1].push_back(fBRatioAvg);
    y_ratio[2].push_back(fCRatioAvg);

    y_ratio_err[0].push_back(fAErrAvg);
    y_ratio_err[1].push_back(fBErrAvg);
    y_ratio_err[2].push_back(fCErrAvg);
  
    y_pull[0].push_back(fAPullAvg);
    y_pull[1].push_back(fBPullAvg);
    y_pull[2].push_back(fCPullAvg);
 
    y_pull_err[0].push_back(fAPullErrAvg);
    y_pull_err[1].push_back(fBPullErrAvg);
    y_pull_err[2].push_back(fCPullErrAvg);

    AnalysisData -> Fill();

    fAValHolder = 0;
    fARatioHolder = 0;
    fAPullHolder = 0;
    fBValHolder = 0;
    fBRatioHolder = 0;
    fBPullHolder = 0;
    fCValHolder = 0;
    fCRatioHolder = 0;
    fCPullHolder = 0;

    fARatioHolder2 = 0;
    fAPullHolder2 = 0;
    fBRatioHolder2 = 0;
    fBPullHolder2 = 0;
    fCRatioHolder2 = 0;
    fCPullHolder2 = 0;

    if (toyEventsHolder < toyEvents) {toyEventsHolder = toyEventsHolder + (toyEvents / toyExperiments);}
  }

  //Plotting stuff - number of histograms vs number of data points
  int Npoint = x_ntoy.size();
  double X_ntoy[3][Npoint];
  double X_err[Npoint];
  double Y_ratio[3][Npoint];
  double Y_ratio_err[3][Npoint];
  double Y_pull[3][Npoint];
  double Y_pull_err[3][Npoint];

  for (int i = 0; i < Npoint; i++)
  {
    X_err[i] = 0;

    for (int j = 0; j < 3; j++)
    {
      X_ntoy[j][i] = x_ntoy[i] + toyEvents / toyExperiments / 5 * j;
      Y_ratio[j][i] = y_ratio[j][i];
      Y_ratio_err[j][i] = y_ratio_err[j][i];
      Y_pull[j][i] = y_pull[j][i];
      Y_pull_err[j][i] = y_pull_err[j][i];
    }
  }

  TMultiGraph* mg_ratio = new TMultiGraph();
  TMultiGraph* mg_pull = new TMultiGraph();

  TGraphErrors* gr_ratio[3];
  TGraphErrors* gr_pull[3];

  for (int i = 0; i < 3; i++)
  {
    gr_ratio[i] = (TGraphErrors*) new TGraphErrors(Npoint, X_ntoy[i], Y_ratio[i], X_err, Y_ratio_err[i]);
    gr_pull[i] = (TGraphErrors*) new TGraphErrors(Npoint, X_ntoy[i], Y_pull[i], X_err, Y_pull_err[i]);
    gr_ratio[i] -> SetMarkerStyle(22);
    gr_pull[i] -> SetMarkerStyle(23);
    gr_ratio[i] -> SetMarkerColor(2+i); //red - A, green - B, blue - C
    gr_pull[i] -> SetMarkerColor(2+i);
    gr_ratio[i] -> SetLineColor(2+i);
    gr_pull[i] -> SetLineColor(2+i);
    mg_ratio -> Add(gr_ratio[i]);
    mg_pull -> Add(gr_pull[i]);
  }

  TCanvas* errorCanvas = new TCanvas("errorCanvas", "errorCanvas", 1200, 1200);
  errorCanvas -> SetGridx();
  errorCanvas -> SetGridy();
  errorCanvas -> Divide(2,3);
  
  errorCanvas -> cd(1);
  mg_ratio -> Draw("AP");
  
  errorCanvas -> cd(2);
  mg_pull -> Draw("AP");

  fOpen->Close();
  fOut.cd();

  /*errorCanvas -> cd(3);
  histPull[0] -> Draw("colz");

  errorCanvas -> cd(4);
  histPull[1] -> Draw("colz");
 
  errorCanvas -> cd(5);
  histPull[2] -> Draw("colz");*/

  cout<<"WRITING DATA TO Analysis.root"<<endl;
  //TFile fOut("D2AnalysisData.root", "RECREATE");
  cout<<"FILE OBJECT CREATED"<<endl;
  AnalysisData -> Write("AnalysisData", TObject::kOverwrite);
  cout<<"TTREE WRITTEN"<<endl;
  histPull[0]-> Write("histPull0");
  cout<<"HISTPULL0 WRITTEN"<<endl;
  histPull[1] -> Write("histPull1");
  cout<<"HISTPULL1 WRITTEN"<<endl;
  histPull[2] -> Write("histPull2");
  cout<<"HISTPULL2 WRITTEN"<<endl;
  fOut.Close();
  cout<<"FILE CLOSED END MY SUFFERING"<<endl;
};

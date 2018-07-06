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
#include "/mnt/c/rootbuild/include/RooKeysPdf.h"

using namespace RooFit;
using namespace RestFrames;
using namespace std;

void dataSetGenerator() {

  int dataSetPts = 1000; //to be changed according to data generated

  //Open ROOT file from Reconstructor and retrieve TTree
  TFile* fOpen = TFile::Open("/mnt/c/rootbuild/macros/recoData.root");
  
  TTree* recoDataTreeA = (TTree*) fOpen -> Get("recoDataA");
  TTree* recoDataTreeB = (TTree*) fOpen -> Get("recoDataB");
  TTree* recoDataTreeC = (TTree*) fOpen -> Get("recoDataC");

  RooRealVar recoMassD1a("recoMassD1a", "recoMassD1a", 0, 1000);
  RooRealVar recoCosD1a("recoCosD1a", "recoCosD1a", -1, 1);
/*
  RooRealVar recoMassD1b("recoMassD1b", "recoMassD1b", 0, 1000);
  RooRealVar recoCosD1b("recoCosD1b", "recoCosD1b", -1, 1);
  RooRealVar recoMassD1c("recoMassD1c", "recoMassD1c", 0, 1000);
  RooRealVar recoCosD1c("recoCosD1c", "recoCosD1c", -1, 1);
*/  

  //Fill datasets with data from TTree (what a cool damn constructor)
  //THREE TREES TWO VARS
  RooDataSet recoDataA("recoDataA", "Data from Decay/Reco A",
                       recoDataTreeA, RooArgSet(recoMassD1a, recoCosD1a));
  RooDataSet recoDataB("recoDataB", "Data from Decay/Reco B",
                       recoDataTreeB, RooArgSet(recoMassD1a, recoCosD1a));
  RooDataSet recoDataC("recoDataC", "Data from Decay/Reco C",
                       recoDataTreeC, RooArgSet(recoMassD1a, recoCosD1a));

  fOpen -> Close();

 /* //Generate PDFs from each Decay/Reco dataset (do we need datasets)
  RooKeysPdf recoPDFA("recoPDFA", "PDF from Reconstruction of Tree A", 
                       recoMassD1a, recoDataA, RooKeysPdf::MirrorBoth);

  //RooDataSet* recoToyA = recoPDFA.generate(RooArgList(recoMassD1a), 1000);

  RooKeysPdf recoPDFB("recoPDFB", "PDF from Reconstruction of Tree B", 
                       recoMassD1b, recoDataB, RooKeysPdf::MirrorBoth);

  //RooDataSet* recoToyB = recoPDFB.generate(RooArgList(recoMassD1b), 1000);

  RooKeysPdf recoPDFC("recoPDFC", "PDF from Reconstruction of Tree C", 
                       recoMassD1c, recoDataC, RooKeysPdf::MirrorBoth);

  //RooDataSet* recoToyC = recoPDFC.generate(RooArgList(recoMassD1c), 1000);

  //Construct RooAddPdf from dataset PDFs
  RooRealVar fitA("fitA", "fitA", 0, 1000000);
  RooRealVar fitB("fitB", "fitB", 0, 1000000);
  RooRealVar fitC("fitC", "fitC", 0, 1000000);

  double fitATrue = 50;
  double fitBTrue = 100;
  double fitCTrue = 80;

  double parTrue[3];
  parTrue[0] = fitATrue / (fitATrue + fitBTrue + fitCTrue);
  parTrue[1] = fitBTrue / (fitATrue + fitBTrue + fitCTrue);
  parTrue[2] = fitCTrue / (fitATrue + fitBTrue + fitCTrue);

  RooAddPdf recoSumPDF("recoSumPDF", "Combined PDF from All Reconstructions",
                        RooArgList(recoPDFA, recoPDFB, recoPDFC),
                        RooArgList(fitA, fitB, fitC));

  fitA.setVal(fitATrue);
  fitB.setVal(fitBTrue);
  fitC.setVal(fitCTrue);

  RooDataSet* recoSumToy = recoSumPDF.generate(RooArgList(recoMassD1a));
  recoSumPDF.fitTo(*recoSumToy);

  RooPlot* testPlot = recoMassD1a.frame();
  recoSumToy -> plotOn(testPlot);
  recoSumPDF.plotOn(testPlot, LineColor(kBlue));
  recoSumPDF.plotOn(testPlot, Components(recoPDFA), LineStyle(kDashed), LineColor(kBlue + 3));
  recoSumPDF.plotOn(testPlot, Components(recoPDFB), LineStyle(kDashed), LineColor(kRed + 3));
  recoSumPDF.plotOn(testPlot, Components(recoPDFC), LineStyle(kDashed), LineColor(kGreen + 3));
  testPlot -> Draw(); */
  
  TFile fOut("test.root", "RECREATE");
  recoDataA.Write("recoDataA", TObject::kOverwrite);
  recoDataB.Write("recoDataB", TObject::kOverwrite);
  recoDataC.Write("recoDataC", TObject::kOverwrite);
  fOut.Close();
}

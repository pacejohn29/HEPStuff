#include <iostream>
#include <string>
#include "/home/pacejohn/home/Git/RestFrames/include/RestFrames/RestFrames.hh"
#include "/mnt/c/rootbuild/include/TTree.h"
#include "/mnt/c/rootbuild/include/TBranch.h"

using namespace ROOT;
using namespace RestFrames;
using namespace std;

/////////////////////////////////////////////////////////
// THINGS JOHN MUST DO:
// GET DATA FROM FILE CREATED BY RECONSTRUCTION x
// GENERATE 3 ROODATASETS FROM THIS DATA - ONE FOR EACH RECONSTRUCTION x
// GENERATE ROONKEYSPDF FROM THESE ROODATASETS x
// DO ERROR ANALYSIS LIKE BEFORE
// On the side: Find out how to make canvases suck less
// CHECK OTHER RECONSTRUCTIONS TOO
/////////////////////////////////////////////////////////

void Reconstructor()
{
  SetStyle();
  bool makePlots = false;
  string choice = "y";
  
  //same total mass
  double mD0 = 1000;

  //three separate cases
  double mD1a = 400;
  double wD1a = 50;
  
  double mD1b = 550;
  double wD1b = 50;

  double mD1c = 700;
  double wD1c = 50;

  double m1 = .1;
  double m2 = 1;
  double m3 = 10;

  //number of generated events
  int Ngen = 1000;

  cout<<"Plot data? (y/n): ";
  cin>>choice;

  while (choice != "y" && choice != "Y" && choice != "n" && choice != "N") {
    cout<<"Good input pls: ";
    cin>>choice;
  }

  if (choice != 'y' || 'Y'){makePlots = false;}
  else {makePlots = true;}

  g_Log << LogInfo << "ARE YOU FEELING IT NOW MR KRABS" << LogEnd;

  LabGenFrame LAB_Ga("LAB_Ga", "LAB");
  DecayGenFrame D0_Ga("D0_Ga", "D_{0}");
  ResonanceGenFrame D1_Ga("D1_Ga", "D_{1}");
  VisibleGenFrame V1_Ga("V1_Ga", "V_{1}");
  VisibleGenFrame V2_Ga("V2_Ga", "V_{2}");
  VisibleGenFrame V3_Ga("V3_Ga", "V_{3}");

  LabGenFrame LAB_Gb("LAB_Gb", "LAB");
  DecayGenFrame D0_Gb("D0_Gb", "D_{0}");
  ResonanceGenFrame D1_Gb("D1_Gb", "D_{1}");
  VisibleGenFrame V1_Gb("V1_Gb", "V_{1}");
  VisibleGenFrame V2_Gb("V2_Gb", "V_{2}");
  VisibleGenFrame V3_Gb("V3_Gb", "V_{3}");

  LabGenFrame LAB_Gc("LAB_Gc", "LAB");
  DecayGenFrame D0_Gc("D0_Gc", "D_{0}");
  ResonanceGenFrame D1_Gc("D1_Gc", "D_{1}");
  VisibleGenFrame V1_Gc("V1_Gc", "V_{1}");
  VisibleGenFrame V2_Gc("V2_Gc", "V_{2}");
  VisibleGenFrame V3_Gc("V3_Gc", "V_{3}");

  LabRecoFrame LAB_Ra("LAB_Ra", "LAB");
  DecayRecoFrame D0_Ra("D0_Ra", "D_{0}");
  DecayRecoFrame D1_Ra("D1_Ra", "D_{1}");
  VisibleRecoFrame V1_Ra("V1_Ra", "V_{1}");
  VisibleRecoFrame V2_Ra("V2_Ra", "V_{2}");
  VisibleRecoFrame V3_Ra("V3_Ra", "V_{3}");

  LabRecoFrame LAB_Rb("LAB_Rb", "LAB");
  DecayRecoFrame D0_Rb("D0_Rb", "D_{0}");
  DecayRecoFrame D1_Rb("D1_Rb", "D_{1}");
  VisibleRecoFrame V1_Rb("V1_Rb", "V_{1}");
  VisibleRecoFrame V2_Rb("V2_Rb", "V_{2}");
  VisibleRecoFrame V3_Rb("V3_Rb", "V_{3}");

  LabRecoFrame LAB_Rc("LAB_Rc", "LAB");
  DecayRecoFrame D0_Rc("D0_Rc", "D_{0}");
  DecayRecoFrame D1_Rc("D1_Rc", "D_{1}");
  VisibleRecoFrame V1_Rc("V1_Rc", "V_{1}");
  VisibleRecoFrame V2_Rc("V2_Rc", "V_{2}");
  VisibleRecoFrame V3_Rc("V3_Rc", "V_{3}");
  
  LAB_Ga.SetChildFrame(D0_Ga); //Setting LabFrames to respective DecayGenFrames
  LAB_Gb.SetChildFrame(D0_Gb);
  LAB_Gc.SetChildFrame(D0_Gc);

  LAB_Ra.SetChildFrame(D0_Ra); //To DecayRecoFrames
  LAB_Rb.SetChildFrame(D0_Rb);
  LAB_Rc.SetChildFrame(D0_Rc);

  //Adding respective ChildFrames to DecayGenFrames
  D0_Ga.AddChildFrame(D1_Ga); D0_Ga.AddChildFrame(V3_Ga);
  D0_Gb.AddChildFrame(D1_Gb); D0_Gb.AddChildFrame(V1_Gb);
  D0_Gc.AddChildFrame(D1_Gc); D0_Gc.AddChildFrame(V2_Gc);

  //To DecayRecoFrames
  D0_Ra.AddChildFrame(D1_Ra); D0_Ra.AddChildFrame(V3_Ra);
  D0_Rb.AddChildFrame(D1_Rb); D0_Rb.AddChildFrame(V3_Rb);
  D0_Rc.AddChildFrame(D1_Rc); D0_Rc.AddChildFrame(V3_Rc);
  
  //Adding respective ChildFrames to ResonanceGenFrames
  D1_Ga.AddChildFrame(V1_Ga); D1_Ga.AddChildFrame(V2_Ga); 
  D1_Gb.AddChildFrame(V2_Gb); D1_Gb.AddChildFrame(V3_Gb);
  D1_Gc.AddChildFrame(V1_Gc); D1_Gc.AddChildFrame(V3_Gc);

  //To ResonanceRecoFrames
  D1_Ra.AddChildFrame(V1_Ra); D1_Ra.AddChildFrame(V2_Ra);
  D1_Rb.AddChildFrame(V1_Rb); D1_Rb.AddChildFrame(V2_Rb);
  D1_Rc.AddChildFrame(V1_Rc); D1_Rc.AddChildFrame(V2_Rc);

  LAB_Ga.InitializeTree();
  LAB_Gb.InitializeTree();
  LAB_Gc.InitializeTree();

  LAB_Ra.InitializeTree();
  LAB_Rb.InitializeTree();
  LAB_Rc.InitializeTree();

  //set masses for gen trees
  D0_Ga.SetMass(mD0);
  D0_Gb.SetMass(mD0);
  D0_Gc.SetMass(mD0);
  
  D1_Ga.SetMass(mD1a); D1_Ga.SetWidth(wD1a);
  D1_Gb.SetMass(mD1b); D1_Gb.SetWidth(wD1b);
  D1_Gc.SetMass(mD1c); D1_Gc.SetWidth(wD1c);

  V1_Ga.SetMass(m1); V2_Ga.SetMass(m2); V3_Ga.SetMass(m3);
  V1_Gb.SetMass(m1); V2_Gb.SetMass(m2); V3_Gb.SetMass(m3);
  V1_Gc.SetMass(m1); V2_Gc.SetMass(m2); V3_Gc.SetMass(m3);

  LAB_Ga.InitializeAnalysis();
  LAB_Gb.InitializeAnalysis();
  LAB_Gc.InitializeAnalysis();

  LAB_Ra.InitializeAnalysis();
  LAB_Rb.InitializeAnalysis();
  LAB_Rc.InitializeAnalysis();

  TTree* recoDataA = new TTree("recoDataA", "recoDataA"); //To store data in after
  TTree* recoDataB = new TTree("recoDataB", "recoDataB"); //To store data in after
  TTree* recoDataC = new TTree("recoDataC", "recoDataC"); //To store data in after

  //For plotting
  double recoMassD1a = 0;
  double recoCosD1a = 0;
  double recoMassD1b = 0;
  double recoCosD1b = 0;
  double recoMassD1c = 0;
  double recoCosD1c = 0;

  TBranch* recoMassD1aBranch = recoDataA -> Branch("recoMassD1a", &recoMassD1a);
  TBranch* recoCosD1aBranch = recoDataA -> Branch("recoCosD1a", &recoCosD1a);
  TBranch* recoMassD1bBranch = recoDataB -> Branch("recoMassD1a", &recoMassD1b);
  TBranch* recoCosD1bBranch = recoDataB -> Branch("recoCosD1a", &recoCosD1b);
  TBranch* recoMassD1cBranch = recoDataC -> Branch("recoMassD1a", &recoMassD1c);
  TBranch* recoCosD1cBranch = recoDataC -> Branch("recoCosD1a", &recoCosD1c);

  TH1F* histMassa = new TH1F("histMassa", "histMassa", 1000, 0, 1000);
  TH1F* histCosa = new TH1F("histCosa", "histCosa", 1000, -1, 1);
  TH2F* hist2Da = new TH2F("hist2Da", "hist2Da", 128, 0, 1000, 128, -1, 1);
  TH1F* histMassb = new TH1F("histMassb", "histMassb", 1000, 0, 1000);
  TH1F* histCosb = new TH1F("histCosb", "histCosb", 1000, 0, 1000);
  TH2F* hist2Db = new TH2F("hist2Db", "hist2Db", 128, 0, 1000, 128, -1, 1);
  TH1F* histMassc = new TH1F("histMassc", "histMassc", 1000, 0, 1000);
  TH1F* histCosc = new TH1F("histCosc", "histCosc", 1000, 0, 1000);
  TH2F* hist2Dc = new TH2F("hist2Dc", "hist2Dc", 128, 0, 1000, 128, -1, 1);

  for (int i = 0; i < Ngen; i++)
  {
    if (i % ((std::max(Ngen, 10)) / 10) == 0)
    {g_Log << LogInfo << "Generating Event " << i << " of " << Ngen << LogEnd;}
  
    LAB_Ga.ClearEvent();
    LAB_Gb.ClearEvent();
    LAB_Gc.ClearEvent();

    //REMEMBER YOU SET IT TO EXPONENTIAL GENERATION
    D1_Ga.SetMass(gRandom -> Exp(100));

    while (LAB_Ga.InitializeAnalysis() == false){
      D1_Ga.SetMass(gRandom -> Exp(100));
    }

    D1_Gb.SetMass(gRandom -> Exp(100));

    while(LAB_Gb.InitializeAnalysis() == false){
      D1_Gb.SetMass(gRandom -> Exp(100));
    }

    D1_Gc.SetMass(gRandom -> Exp(100));
    
    while(LAB_Gc.InitializeAnalysis() == false){
      D1_Gc.SetMass(gRandom -> Exp(100));
    }

    LAB_Ga.AnalyzeEvent();
    LAB_Gb.AnalyzeEvent();
    LAB_Gc.AnalyzeEvent();

    LAB_Ra.ClearEvent();
    V1_Ra.SetLabFrameFourVector(V1_Ga.GetFourVector());
    V2_Ra.SetLabFrameFourVector(V2_Ga.GetFourVector());
    V3_Ra.SetLabFrameFourVector(V3_Ga.GetFourVector());
    LAB_Ra.AnalyzeEvent();

    LAB_Rb.ClearEvent();
    V1_Rb.SetLabFrameFourVector(V1_Gb.GetFourVector());
    V2_Rb.SetLabFrameFourVector(V2_Gb.GetFourVector());
    V3_Rb.SetLabFrameFourVector(V3_Gb.GetFourVector());
    LAB_Rb.AnalyzeEvent();

    LAB_Rc.ClearEvent();
    V1_Rc.SetLabFrameFourVector(V1_Gc.GetFourVector());
    V2_Rc.SetLabFrameFourVector(V2_Gc.GetFourVector());
    V3_Rc.SetLabFrameFourVector(V3_Gc.GetFourVector());
    LAB_Rc.AnalyzeEvent();

    //CHANGED TO FILL TTREE WITH ONLY ONE MASS/DECAY ANGLE VALUE
    recoMassD1a = D1_Ra.GetMass();
    recoCosD1a = D1_Ra.GetCosDecayAngle();
    recoMassD1b = D1_Rb.GetMass();
    recoCosD1b = D1_Rb.GetCosDecayAngle();
    recoMassD1c = D1_Rc.GetMass();
    recoCosD1c = D1_Rc.GetCosDecayAngle();

    histMassa -> Fill(recoMassD1a);
    histCosa -> Fill(recoCosD1a);
    hist2Da -> Fill(recoMassD1a, recoCosD1a);
    histMassb -> Fill(recoMassD1b);
    histCosb -> Fill(recoCosD1b);
    hist2Db -> Fill(recoMassD1b, recoCosD1b);
    histMassc -> Fill(recoMassD1c);
    histCosc -> Fill(recoCosD1c);
    hist2Dc -> Fill(recoMassD1c, recoCosD1c);

    recoDataA -> Fill();
    recoDataB -> Fill();
    recoDataC -> Fill();

    /*cout << "Tree A: " << D1_Ga.GetMass() << " " << D1_Ga.GetCosDecayAngle() << endl;
    cout << "Tree B: " << D1_Gb.GetMass() << " " << D1_Gb.GetCosDecayAngle() << endl;
    cout << "Tree C: " << D1_Gc.GetMass() << " " << D1_Gc.GetCosDecayAngle() << endl;*/
  }

  if (makePlots == true){
  TCanvas* treeCanvas = new TCanvas("treeCanvas", "Tree Canvas", 1200, 1200);
  treeCanvas -> Divide(3,3);

  TreePlot* plotGena = new TreePlot("plotGena", "plotGena");
  TreePlot* plotGenb = new TreePlot("plotGenb", "plotGenb");
  TreePlot* plotGenc = new TreePlot("plotGenc", "plotGenc");

  TreePlot* plotRecoa = new TreePlot("plotRecoa", "plotRecoa");
  TreePlot* plotRecob = new TreePlot("plotRecob", "plotRecob");
  TreePlot* plotRecoc = new TreePlot("plotRecoc", "plotRecoc");
  
  plotGena -> SetTree(LAB_Ga);
  plotGena -> Draw("GenTreeA", "Generator Tree (a)");
  treeCanvas -> cd(4);
  plotRecoa -> SetTree(LAB_Ra);
  plotRecoa -> Draw("RecoTreeA", "Reconstruction Tree (a)");
  treeCanvas -> cd(7);
  hist2Da -> Draw("colz");

  treeCanvas -> cd(2);
  plotGenb -> SetTree(LAB_Gb);
  plotGenb -> Draw("GenTreeB", "Generator Tree (b)");
  treeCanvas -> cd(5);
  plotRecob -> SetTree(LAB_Rb);
  plotRecob -> Draw("RecoTreeB", "Reconstruction Tree (b)");
  treeCanvas -> cd(8);
  hist2Db -> Draw("colz");

  treeCanvas -> cd(3);
  plotGenc -> SetTree(LAB_Gc);
  plotGenc -> Draw("GenTreeC", "Generator Tree (c)");
  treeCanvas -> cd(6);
  plotRecoc -> SetTree(LAB_Rc);
  plotRecoc -> Draw("RecoTreeC", "Reconstruction Tree (c)");
  treeCanvas -> cd(9);
  hist2Dc -> Draw("colz");
  //Get TreePlots from created ROOT file, find out how to plot TCanvases on other TCanvas
  }

  TFile fOut("recoData.root", "RECREATE");
  recoDataA -> Write("recoDataA", TObject::kOverwrite);
  recoDataB -> Write("recoDataB", TObject::kOverwrite);
  recoDataC -> Write("recoDataC", TObject::kOverwrite);
  histMassa -> Write("histMassa");
  histCosa -> Write("histCosa");
  hist2Da -> Write("hist2Da");
  histMassb -> Write("histMassb");
  histCosb -> Write("histCosb");
  hist2Db -> Write("hist2Db");
  histMassc -> Write("histMassc");
  histCosc -> Write("histCosc");
  hist2Dc -> Write("hist2Dc");
  fOut.Close();
}

RooDataSet* randomGenerate(RooDataSet* A, RooDataSet* B, RooDataSet* C,
                           double fitA, double fitB, double fitC, int Nevents)
{
  double fitNormalizer = fitA + fitB + fitC;
  const RooArgSet* holder;
  RooRealVar recoMassD1a("recoMassD1a", "recoMassD1a", 0, 1000);
  RooRealVar recoCosD1a("recoCosD1a", "recoCosD1a", -1, 1);
  RooDataSet* recoToy = new RooDataSet("recoToy", "recoToy", RooArgSet(recoMassD1a, recoCosD1a));
  double rando = 0;
  TRandom3* gRandom = new TRandom3();
  int cat = 0;

  for (int i = 0; i < Nevents; i++)
  {
    rando = gRandom -> Rndm();

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
  
  }

  return(recoToy);
};
void ROOTRandomSleep(Double_t scale = 0, Int_t seed=0){
  /*
   * Have ROOT sleep for time in seconds
   * The time is between zero (0) and "scale" seconds.
   * 	The default is NO sleep time is initiated
   * 	For scale == 1, then ROOT will sleep between 0 to 1 seconds
   * The seed is from the current Unix time unless otherwise stated
   * An additional seed is set to avoid getting the same results
   * when this is called at the same time for different instances
   */
  if(scale <= 0){
    return;
  }
  if(seed==0){
    const TDatime time;
    seed = time.Get();
  }
  const TRandom2 ran2(seed);
  const TRandom3 ran3(TMath::Nint(seed/(ran2.Rndm()+0.001)));
  const UInt_t sleepTime = TMath::Nint(ran3.Rndm()*1e3*scale);//seconds
  cout << "Sleeping for " << 1e-3*sleepTime << " seconds " << endl;
  gSystem->Sleep(sleepTime);

}

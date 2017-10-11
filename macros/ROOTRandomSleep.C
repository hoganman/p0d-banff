void ROOTRandomSleep(Double_t scale = 0, Int_t seed=0){
  /*
   * Have ROOT sleep for time in seconds
   * The time is between zero (0) and "scale" seconds.
   * 	The default is NO sleep time is initiated
   * 	For scale == 1, then ROOT will sleep between 0 to 1 seconds
   * The seed is from the current Unix time unless otherwise stated
   */
  if(scale <= 0){
    return; 
  }
  if(seed==0){
    const TDatime time;
    seed = time.Get();
  }
  const TRandom3 ran(seed);
  const UInt_t sleepTime = 
    TMath::Nint(ran.Rndm()*1e3*scale);//seconds
  cout << "Sleeping for " << (Double_t)sleepTime/1e3 
    << " seconds " << endl;
  gSystem->Sleep(sleepTime);

}

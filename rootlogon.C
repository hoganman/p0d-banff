{
    if(!gSystem->Getenv("P0DBANFFROOT") || !gSystem->FindFile(gSystem->Getenv("LD_LIBRARY_PATH"),"libP0DBANFF.so"))
    {
        return;
    }
    TGaxis::SetMaxDigits(3);
    gSystem->Load("libTree");
    gSystem->Load("libP0DBANFF");
    P0DBANFFInterface interface;
    printf("Loaded P0DBANFFInterface as \"interface\"\n");
    interface.LoadColorBlindPalette();
}

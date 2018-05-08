{
    if(!gSystem->Getenv("P0DBANFFROOT") || !gSystem->FindFile(gSystem->Getenv("LD_LIBRARY_PATH"),"libP0DBANFF.so"))
    {
        printf("Unable to load libP0DBANFF\n");
        return;
    }
    TGaxis::SetMaxDigits(3);
    gSystem->Load("libGraf");
    gSystem->Load("libGraf3d");
    gSystem->Load("libTree");
    gSystem->Load("libP0DBANFF");
    P0DBANFFInterface interface;
    interface.LoadColorBlindPalette();
    printf("Loaded P0DBANFFInterface as \"interface\"\n");
}

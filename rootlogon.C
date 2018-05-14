{
    if(!gSystem->Getenv("P0DBANFFROOT"))
    {
        printf("ERROR: P0DBANFFROOT NOT set\n");
        printf("Unable to load libP0DBANFF\n");
        return;
    }
    if(!gSystem->FindFile(gSystem->Getenv("LD_LIBRARY_PATH"),"libP0DBANFF.so"))
    {
        printf("ERROR: libP0DBANFF NOT found in LD_LIBRARY_PATH\n.");
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

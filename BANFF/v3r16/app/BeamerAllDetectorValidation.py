#!/usr/bin/python
from ROOT import *
import sys
import math
import os
from subprocess import call
from array import array

def AddSlide(SlideFile, NameSlide, cov, corr, diag, diff):
    
    print "Adding slide "+NameSlide
    SlideFile.write("\\begin{frame}{"+NameSlide+"}\n")
    SlideFile.write("\\begin{center}\n")
    SlideFile.write("\\includegraphics[width=0.48\\textwidth]{img/"+diag+"}\n")
    SlideFile.write("\\includegraphics[width=0.48\\textwidth]{img/"+corr+"} \\\\\n")
    if diff == "":
        SlideFile.write("\\end{center}\n")
        SlideFile.write("\\includegraphics[width=0.48\\textwidth]{img/"+cov+"}\n")
    else:
        print "Adding "+diff+" to the slide "+NameSlide
        SlideFile.write("\\includegraphics[width=0.48\\textwidth]{img/"+cov+"}\n")
        SlideFile.write("\\includegraphics[width=0.48\\textwidth]{img/"+diff+"}\n")
        SlideFile.write("\\end{center}\n")
        
    SlideFile.write("\\end{frame}\n")


def AddSyst(SlideFile,NameSyst1,NameSyst2,Suffix,index):
    f = TFile("./"+NameSyst1+Suffix,"READ")
    print index
    cov  = f.Get(str(index)+"_Total_Covariance_Matrix")
    corr = f.Get(str(index)+"_Total_Correlation_Matrix")
    diag = f.Get(str(index)+"_mc_sys_error_redu")
    f2 = TFile("../inputs/HighlandThrows.root")

    cov .SetStats(0)
    corr.SetStats(0)
    corr.SetMinimum(-1)
    corr.SetMaximum( 1)
 
    diag.SetStats(0)
    covname  = "TOTAL_"+NameSyst1+"_"+NameSyst2+"_Cov.pdf"
    corrname = "TOTAL_"+NameSyst1+"_"+NameSyst2+"_Corr.pdf"
    diagname = "TOTAL_"+NameSyst1+"_"+NameSyst2+"_Diag.pdf"

    c = TCanvas()
    tm = gStyle.GetPadTopMargin   ()
    bm = gStyle.GetPadBottomMargin()
    rm = gStyle.GetPadRightMargin ()
    lm = gStyle.GetPadLeftMargin  ()
    gStyle.SetPadTopMargin   (tm)
    gStyle.SetPadBottomMargin(0.15)
    gStyle.SetPadRightMargin (0.15)
    gStyle.SetPadLeftMargin  (0.15)
    cov.Draw("COLZ")
    c.SaveAs("img/"+covname)
    corr.Draw("COLZ")
    c.SaveAs("img/"+corrname)
    diag.Draw("")
    c.SaveAs("img/"+diagname)
    
    slideName = NameSyst1
    if NameSyst2 != "":
        slideName = slideName+" "+NameSyst2
    
    SlideFile.write("\\section{"+slideName+"}\n")
    print "Adding section "+slideName
        
    AddSlide(SlideFile, slideName+" All samples", covname, corrname, diagname, "")
    
    scov  = TH2D("Covariance",  "Covariance;Muon Momentum Bins (0-5GeV);Muon Momentum Bins (0-5GeV)",  20, 0, 20, 20, 0, 20)
    scorr = TH2D("Correlation", "Correlation;Muon Momentum Bins (0-5GeV);Muon Momentum Bins (0-5GeV)", 20, 0, 20, 20, 0, 20)
    sdiag = TH1D("Diagonal",    "Diagonal;Muon Momentum Bins (0-5GeV)",                                20, 0, 20)
    
    SampleName = ["FGD1CC0Pi","FGD1CC1Pi","FGD1CCOth","FGD2CC0Pi","FGD2CC1Pi","FGD2CCOth"]
    
    for isample in xrange(0,6):

        # some intricated way of changing the name of the syst...
        HighlandHistoName = ""
        flag=0 
        if SampleName[isample].find("CC0Pi") != -1:
            HighlandHistoName = "CC-0Pi"
        elif SampleName[isample].find("CC1Pi") != -1:
            HighlandHistoName = "CC-1PiPlus"
        elif SampleName[isample].find("CCOth") != -1:
            HighlandHistoName = "CC-Other"
            
        HighlandHistoName = HighlandHistoName+"_"

        if NameSyst1=="BFieldDist" != -1:
            HighlandHistoName = HighlandHistoName+"bfield"
        elif NameSyst1=="MomScale" != -1:
            HighlandHistoName = HighlandHistoName+"momscale"
        elif NameSyst1=="MomResol" != -1:
            HighlandHistoName = HighlandHistoName+"momresol"
        elif NameSyst1=="TpcPid" != -1:
            HighlandHistoName = HighlandHistoName+"tpcpid"
        elif NameSyst1=="FgdPid" != -1:
            HighlandHistoName = HighlandHistoName+"fgdpid"
        elif NameSyst1=="weight" != -1 and NameSyst2=="ChargeIDEff" != -1:
            HighlandHistoName = HighlandHistoName+"chargeideff"
        elif NameSyst1=="weight" != -1 and NameSyst2=="TpcClusterEff" != -1:
            HighlandHistoName = HighlandHistoName+"tpcclustereff"
        elif NameSyst1=="weight" != -1 and NameSyst2=="TpcTrackEff" != -1:
            HighlandHistoName = HighlandHistoName+"tpctrackeff"
        elif NameSyst1=="weight" != -1 and NameSyst2=="TpcFgdMatchEff" != -1:
            HighlandHistoName = HighlandHistoName+"tpcfgdmatcheff"
        elif NameSyst1=="weight" != -1 and NameSyst2=="MichelEleEff" != -1:
            HighlandHistoName = HighlandHistoName+"michel"
        elif NameSyst1=="weight" != -1 and NameSyst2=="PileUp" != -1:
            HighlandHistoName = HighlandHistoName+"pileup"
        elif NameSyst1=="weight" != -1 and NameSyst2=="FgdMass" != -1:
            HighlandHistoName = HighlandHistoName+"fgdmass"
        elif NameSyst1=="weight" != -1 and NameSyst2=="OOFV" != -1:
            HighlandHistoName = HighlandHistoName+"oofv"
        elif NameSyst1=="weight" != -1 and NameSyst2=="SIPion" != -1:
            HighlandHistoName = HighlandHistoName+"sipion"
        elif NameSyst1=="weight" != -1 and NameSyst2=="SIProton" != -1:
            HighlandHistoName = HighlandHistoName+"siproton"
        else:
            print NameSyst1+" and "+NameSyst2+" not found"
            flag=1

        HighlandHistoName=HighlandHistoName+"_syst_"

        if SampleName[isample].find("FGD1") != -1:
            HighlandHistoName = HighlandHistoName+"FGD1"
        elif SampleName[isample].find("FGD2") != -1:
            HighlandHistoName = HighlandHistoName+"FGD2"
            
        if flag == 0:
            print HighlandHistoName
            hdiag = f2.Get(HighlandHistoName)
            hdiag2 = hdiag.Clone()
            hdiag3 = hdiag.Clone()
        else:
            hdiag2 = diag
            
        for iBin in xrange(1,21):
            hdiag2.SetBinContent(iBin,diag.GetBinContent(iBin+isample*20))
            if flag == 0:
                if diag.GetBinContent(iBin+isample*20) > 0.0001:
                    hdiag.SetBinContent(iBin, hdiag.GetBinContent(iBin)/diag.GetBinContent(iBin+isample*20))
                else:
                    hdiag.SetBinContent(iBin, 0)
            for jBin in xrange(1,21):
                scov .SetBinContent(iBin,jBin,cov .GetBinContent(iBin+isample*20,jBin+isample*20))
                scorr.SetBinContent(iBin,jBin,corr.GetBinContent(iBin+isample*20,jBin+isample*20))
        covname  = SampleName[isample]+"_"+NameSyst1+"_"+NameSyst2+"_Cov.pdf"
        corrname = SampleName[isample]+"_"+NameSyst1+"_"+NameSyst2+"_Corr.pdf"
        diagname = SampleName[isample]+"_"+NameSyst1+"_"+NameSyst2+"_Diag.pdf"
        diffname = SampleName[isample]+"_"+NameSyst1+"_"+NameSyst2+"_Diff.pdf"
        if flag == 1:
            diffname=""
        scov .SetStats(0)
        scorr.SetStats(0)
        scorr.SetMinimum(-1)
        scorr.SetMaximum( 1)
        hdiag2.SetStats(0)
        hdiag2.SetLineWidth(2)
        hdiag2.SetLineColor(kBlack)
        scov.Draw("COLZ")
        c.SaveAs("img/"+covname)
        scorr.Draw("COLZ")
        c.SaveAs("img/"+corrname)
        hdiag2.Draw("")
        if flag == 0:
            hdiag3.SetLineColor(kBlack)
            hdiag3.SetLineStyle(2)
            hdiag3.SetLineWidth(2)
            hdiag3.Draw("SAME")
            leg = TLegend(0.4,0.8,0.7,0.9)
            leg.AddEntry(hdiag3, "Highland", "L")
            leg.AddEntry(hdiag2, "Psyche", "L")
            leg.SetFillColor(0)
            leg.Draw()
            c.SaveAs("img/"+diagname)
            hdiag.SetStats(0)
            hdiag.SetLineWidth(2)
            hdiag.SetLineColor(kBlack)
            hdiag.GetYaxis().SetTitle("Difference errors (\%)")
            hdiag.SetTitle("HighlandErrors / PsycheErrors")
            min=hdiag.GetBinContent(hdiag.GetMinimumBin())
            max=hdiag.GetBinContent(hdiag.GetMaximumBin())
            # max=hdiag.GetMaximum()
            # min=hdiag.GetMinimum()
            print "min "+str(min)
            print "max "+str(max)
            hdiag.SetMaximum(max*1.2)
            hdiag.SetMinimum(0.)
            hdiag.Draw("")
            c.SaveAs("img/"+diffname)
        else:
            c.SaveAs("img/"+diagname)
                    
        AddSlide(SlideFile, SampleName[isample]+" "+slideName, covname, corrname, diagname, diffname)


    f.Close()
    f2.Close()


def main():
    gROOT.SetBatch(kTRUE);
    
    #NameOfFile = ["MomScale","MomResol","TpcPid","FgdPid","weight"]
    NameOfFile = ["BFieldDist","MomScale","MomResol","TpcPid","FgdPid"]
    #NameOfFile = ["all"]
    Suffix = "_Comparison.root"
    CorrespondanceWeights = ["ChargeIDEff","TpcClusterEff","TpcTrackEff","TpcFgdMatchEff","TpcECalMatchEff","FgdHybridTrackEff","MichelEleEff","PileUp","FgdMass","OOFV",
                             "SIPion","SIProton","SandMu"]
    
    outFile = open("presentation.tex","w")
    outFile.write("\\documentclass{beamer}\n")
    outFile.write("\\usepackage[english] {babel}\n")
    outFile.write("\\usepackage[T1]      {fontenc}\n")
    outFile.write("\\usepackage{amsmath, amsfonts, graphicx}\n")
    outFile.write("\\usepackage{bibunits, tikz, version}\n")
    # outFile.write("\\usetheme[pageofpages=of,% String used between the current page and the\n")
    # outFile.write("% total page count.\n")
    # outFile.write("alternativetitlepage=true,% Use the fancy title page.\n")
    # outFile.write("]{Torino}\n")
    # outFile.write("\\usecolortheme{nouvelle}\n")
    # outFile.write("\n")
    outFile.write("\\title{Psyche Validations}\n")
    outFile.write("\\author{Pierre Lasorak}\n")
    outFile.write("\\date{\\today}\n")
    outFile.write("\n")
    outFile.write("\\begin{document}\n")
    outFile.write("\\maketitle\n")
    NCont = 400;
    gStyle.SetNumberContours(NCont);
    gStyle.SetOptStat(0);
    gStyle.SetPadRightMargin(0.13)
    
    NRGBs = 3;
    stops = [0.00, 0.5, 1.00]
    red   = [0.00, 1.0, 1.00]
    green = [0.00, 1.0, 0.00]
    blue  = [1.00, 1.0, 0.00]
    
    stopsArray = array('d', stops)
    redArray   = array('d', red)
    greenArray = array('d', green)
    blueArray  = array('d', blue)
    TColor.CreateGradientColorTable(NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
        
    for fileN in NameOfFile:
        print fileN
        if fileN == "weight":
            for syst in xrange(0,20):
                AddSyst(outFile,fileN,CorrespondanceWeights[syst],Suffix,syst)
        else:
            AddSyst(outFile,fileN,"",Suffix,0) 

    outFile.write("\\end{document}\n")
    outFile.close()
    call(["pdflatex", "presentation.tex"])
    
if __name__ == "__main__":
    main()


    

#!/usr/bin/python
from ROOT import *
import glob

filelist = glob.glob("/home/myslik/data/FinalToyFits_20150422/*.root")
filelist2 = glob.glob("/home/myslik/data/FixedCCQEToyFits_20150224/*.root")

outFile = TFile("chi2File_400Toys_20150422_forexplanation.root","RECREATE")

#Create the PDF output file.
c1 = TCanvas()
c1.Print("chi2File_400Toys_20150422_forexplanation.pdf[")

plotNames = ["total", "samples", "cc0pi", "cc1pi", "ccOth", "anuccqe",
"anuccnqe", "nuccqe", "nuccnqe", "prior", "flux", "xsec", "obsNorm"]

#[NBins,min, max]
plotLim = [[60,400.0,1000.0], [60,400.0,1000.0], [30,0.0,300.0], [30,0.0,300.0],
        [40,0.0,400.0], [20,0.0,100.0], [20,0.0,100.0],
        [20,0.0,100.0],[20,0.0,100.0],[40,0.0,100.0],[30,0.0,60.0],[30,0.0,60.0],[30,0.0,60.0]]


#dataFitResults = [743.89, 697.30, 216.71, 167.96, 233.94, 11.315, 11.165,
#        26.33, 29.87, 46.59, 9.51, 7.66, 29.42]

#dataFitResults = [746.628,700.068,220.158,171.658,229.643,12.2696,11.301,
#        25.355,29.6832,46.5596,9.80448,7.55803,29.1971]

#The buggy fit result
#dataFitResults = [755.664,699.253,211.816,181.591,233.381,13.7872,14.0395,
#        22.4843,22.154,56.4111,12.5145,14.9064,28.9902]

#The current best fit.
dataFitResults2 = [769.402,709.574,221.772,181.763,234.135,13.7862,14.124,
        22.1124,21.8817,59.8279,12.4922,17.7499,29.5858]

vectorNames =["FittedMinimizedQuantity0", "FittedTotalSampleContribs0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedPriorTerm0","FittedPriorTerm0","FittedPriorTerm0","FittedPriorTerm0"]
vectorIndices = [0,0,0,1,2,3,4,5,6,3,0,1,2]

outHists = []
outHists2 = []
for i in xrange(0,len(plotNames)):
    outHists.append(TH1D(plotNames[i],plotNames[i] +";#Delta#chi^{2};Number of toy experiments",plotLim[i][0],plotLim[i][1],plotLim[i][2]))
    outHists2.append(TH1D(plotNames[i]+str(2),plotNames[i] +";#Delta#chi^{2};Number of toy experiments",plotLim[i][0],plotLim[i][1],plotLim[i][2]))

TotalFits = 0
FitsWithChi2GTData = 0


#Loop through all the files in the filelist.
for fidx in xrange(0, len(filelist)):

    #Skip the two where the fits failed.
    #if ("_212.root" in fileName) or ("_54.root" in fileName):
    #    continue

    #Skip the 2 where the fits failed.
    if ("_336.root" in filelist[fidx]) or ("_65.root" in filelist[fidx]):
        continue

    TotalFits += 1 

    #Otherwise, load the file.
    curr = TFile(filelist[fidx])
    curr2 = TFile(filelist2[fidx])

    #Now load in the vector for each plot.
    vectors = []
    vectors2 = []
    
    for vect in vectorNames:
        vectors.append(curr.Get(vect))
        vectors2.append(curr2.Get(vect))

    #Now, fill each histogram
    for i in xrange(0, len(outHists)):

        relevantVect = vectors[i]
        relevantVect2 = vectors2[i]
        outHists[i].Fill(relevantVect[vectorIndices[i]])
        outHists2[i].Fill(relevantVect2[vectorIndices[i]])

        #If the total value being added to the distribution is greater than the data
        #value, count it.
        if(i == 0):

            if(relevantVect[vectorIndices[i]] > dataFitResults2[i]):

                FitsWithChi2GTData += 1

#Can now print out the p value.
print "Total number of fits: " + str(TotalFits)
print "Number of fits with greater chi2 than data: " + str(FitsWithChi2GTData)
print "p-value = " + str(float(FitsWithChi2GTData)/float(TotalFits))


#Now write out to PDF:
for i in xrange(0, len(outHists)):

    c1.cd()
    outHists2[i].SetLineWidth(3)
    outHists2[i].SetLineColor(kBlue)
    outHists2[i].Draw()

    outHists[i].SetLineWidth(3)
    outHists[i].SetLineColor(kBlack)
    outHists[i].Draw("sames")
    # outHists[i].Draw()

    outHists2[i].SetMaximum(max(outHists2[i].GetMaximum(),outHists[i].GetMaximum()))

    c1.Modified()
    c1.Update()
    
    #Add a TLine in red from yMin to yMax at x = data fit result.
    #Unconstrained fit with slightly different prior in blue.
#    dataLine = TLine(dataFitResults[i],c1.GetUymin(),dataFitResults[i],c1.GetUymax())
#    dataLine.SetLineWidth(3)
#    dataLine.SetLineColor(kBlue)
#    dataLine.Draw("same")

#    dataLine2 = TLine(dataFitResults2[i],c1.GetUymin(),dataFitResults2[i],c1.GetUymax())
#    dataLine2.SetLineWidth(3)
#    dataLine2.SetLineColor(kRed)
#    dataLine2.Draw("same")

    #If it's the first one, add a chi2 distribution in red.
    if i == 0:
        pdf = TF1("pdf","3980*ROOT::Math::chisquared_pdf(x,[0],[1])",400,1000)
        pdf.SetParameters(560,0)
        pdf.Draw("same")


    c1.Modified()
    c1.Update()

    c1.Print("chi2File_400Toys_20150422_forexplanation.pdf")   


#Close the PDF file.
c1.Print("chi2File_400Toys_20150422_forexplanation.pdf]")

#With all histograms filled, write them to the ROOT file for now.
outFile.Write()
outFile.Close()





#outHist = TH1F("total","total;#Delta#chi^{2};Number of toy experiments",30,400.0,700.0)
#sampleHist = TH1F("samples","samples;#Delta#chi^{2};Number of toy experiments",40,300.0,700.0)
#priorHist = TH1F("prior","prior;#Delta#chi^{2};Number of toy experiments",24,0.0,60.0)
#
#for file in filelist:
#
#    curr = TFile(file)
#    
#    fmq = curr.Get("FittedMinimizedQuantity0")
#    ftsc = curr.Get("FittedTotalSampleContribs0")
#    fpt = curr.Get("FittedPriorTerm0")
#
#    if fmq:
#	    outHist.Fill(fmq[0])
#
#    if ftsc:
#        sampleHist.Fill(ftsc[0])
#
#    if fpt:
#        priorHist.Fill(fpt[3])
#
#
#outCanvas = TCanvas()
#sampleCanvas = TCanvas()
#priorCanvas = TCanvas()
#outCanvas.cd()
#
#outHist.Draw()
#outCanvas.Modified()
#outCanvas.Update()
#outCanvas.Print("totalchi2_at_minimum.pdf")
#
#sampleCanvas.cd()
#sampleHist.Draw()
#sampleCanvas.Modified()
#sampleCanvas.Update()
#sampleCanvas.Print("samplechi2_at_minimum.pdf")
#
#priorCanvas.cd()
#priorHist.Draw()
#priorCanvas.Modified()
#priorCanvas.Update()
#priorCanvas.Print("priorchi2_at_minimum.pdf")
#
#
#
#outFile.Write()
#outFile.Close()

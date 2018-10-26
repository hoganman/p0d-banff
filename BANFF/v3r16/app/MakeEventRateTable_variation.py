#!/usr/bin/python
#Reads in a user supplied throw file with nominal MC for many different weight
#combinations, and outputs a .tex file that can be made into a table.
#Usage: MakeEventRateTable_variation.py inputFile outputFile
from ROOT import *
import sys
import copy
import math

# Load BANFF one sigma information
fileName = sys.argv[1]

xsecinfile = TFile("/mnt/home/morri631/BANFF_Summer2018_Analysis/software/BANFF/v3r16/inputs/xsec_parameters_2018a_NOMINAL_v11.root")
xsec_cov   = xsecinfile.Get("xsec_cov")
xsec_prior = xsecinfile.Get("xsec_param_prior")
xsec_ub    = xsecinfile.Get("xsec_param_ub")

sigma = []

for i in xrange(0,xsec_cov.GetNrows()):
    if xsec_prior(i)+sqrt(xsec_cov(i,i))<xsec_ub(i):
        sigma.append(sqrt(xsec_cov(i,i)))
    else:
         sigma.append(xsec_ub(i)-xsec_prior(i))
         
# Load MaCH3 one sigma information
values_mach3 = []
mach3 = open("inputs/run_20181SigmasXsec.log", "r") 
lines =  mach3.readlines() 
dontwant = ["Setting", "FGD1", "FGD2", "Succesfully","Info"]
for line in lines:
    words = line.split()
    if words[0] not in dontwant:
        values_mach3.append(words[0])

inFile = TFile(fileName)

outFile = open(sys.argv[2],"w")

# Start by printing the needed header information to the output file.
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage[paperwidth=15in, paperheight=13in,margin=0.25in]{geometry}\n")
outFile.write("\\usepackage{booktabs}     \n")
#outFile.write("\\usepackage{changepage}\n")
outFile.write("\\usepackage{multirow}\n")
outFile.write("\\usepackage{graphicx,color}\n")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{table}[ht!]\n")
outFile.write("\\centering\n")
outFile.write("\\resizebox{0.95\\textwidth}{!}{\n")
outFile.write("\\renewcommand{\\arraystretch}{1.2}\n")
outFile.write("\\begin{tabular}{ c  c || c || c | c || c | c || c | c || c |}\n")
outFile.write("\\midrule[1.3pt]\n")


outFile.write("\\textbf{\\emph{Parameter}}& \\textbf{\\emph{$\\mathbf{1\sigma}$ value}} & \\textbf{\\emph{Sample}}  &  \\textbf{\\emph{BANFF $\\mathbf{-1\sigma}$}}  &  \\textbf{MaCh3 \\emph{$\\mathbf{-1\sigma}$}}  &  \\textbf{\\emph{BANFF nominal}}  &  \\textbf{\\emph{MaCh3 nominal}}  &  \\textbf{\\emph{BANFF $\\mathbf{+1\sigma}$}} &  \\textbf{\\emph{MaCh3 $\\mathbf{+1\sigma}$}} & (\\textbf{\\emph{BANFF - MaCh3 / BANFF) $\\mathbf{+1\sigma}$}}  \\\\ \n")
outFile.write("\\midrule[1.3pt]\n")


sampleName = ["FGD1 CC0$\pi$", "FGD1 CC1$\pi$", "FGD1 CCOther", 
              "FGD2 CC0$\pi$", "FGD2 CC1$\pi$", "FGD2 CCOther",
              "FGD1 Anu-CCQE", "FGD1 Anu-CCNQE", 
              "FGD2 Anu-CCQE", "FGD2 Anu-CCNQE", 
              "FGD1 Nu-CCQE", "FGD1 Nu-CCNQE", 
              "FGD2 Nu-CCQE", "FGD2 Nu-CCNQE"]

# Now grab the sample names from the sample name TObjString.
sampleNames =  inFile.Get("sampleNames")
paramNames  =  inFile.Get("parameterNames")

sampleNameList = ["FGD1_numuCC_0pi"                  ,
                  "FGD1_numuCC_1pi"                  ,
                  "FGD1_numuCC_other"                ,
                  "FGD2_numuCC_0pi"                  ,
                  "FGD2_numuCC_1pi"                  ,
                  "FGD2_numuCC_other"                ,

                  "FGD1_anti-numuCC_QE"              ,
                  "FGD1_anti-numuCC_nQE"             ,
                  "FGD2_anti-numuCC_1_track"         ,
                  "FGD2_anti-numuCC_N_tracks"        ,

                  "FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_",
                  "FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode",
                  "FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_",
                  "FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode"]

paramNameList = [];

for i in xrange(100,132):
    paramNameList.append(paramNames.At(i).GetString().Data())
    paramNames.At(i).Print("")

# For each sample, load in the corresponding list of THnD objects to fill the table.
histogramList0 = [] #  nominal BANFF
histogramList  = [] # +1 sigma BANFF
histogramList2 = [] # -1 sigma BANFF

for pName in paramNameList:
    thisHistList0 = []
    for sName in sampleNameList:
        histName0 = sName + "_predMC_" + pName + "_m0sigma"
        thisHistList0.append(inFile.Get(histName0))
    histogramList0.append(thisHistList0)

    thisHistList = []
    for sName in sampleNameList:
        histName = sName + "_predMC_" + pName + "_1sigma"
        thisHistList.append(inFile.Get(histName))
    histogramList.append(thisHistList)

    thisHistList2 = []
    for sName in sampleNameList:
        histName2 = sName + "_predMC_" + pName + "_m1sigma"
        thisHistList2.append(inFile.Get(histName2))
    histogramList2.append(thisHistList2)


print len(values_mach3)
print len(paramNameList)
print len(sampleNameList)

# Extract MaCH3 values with original MaCH3 ordering
mach3_ordered = []
for i in xrange(0,len(paramNameList)):
    param_temp = []
    for j in xrange(0,len(sampleNameList)):
        vary_temp = []
        
        vary_temp.append(values_mach3[i*3 + j*len(paramNameList)*3  ])
        vary_temp.append(values_mach3[i*3 + j*len(paramNameList)*3+1])
        vary_temp.append(values_mach3[i*3 + j*len(paramNameList)*3+2])
        param_temp.append(vary_temp)
    mach3_ordered.append(param_temp)
    
#for i in xrange(0,len(mach3_ordered)):
#    print mach3_ordered[i]

# Move MaCH3 ordered params to BANFF ordering (FSI first)
final_mach3_temp = copy.deepcopy(mach3_ordered)
for i in xrange(0,len(paramNameList)):
    if i < 5:
        final_mach3_temp[i] = copy.deepcopy(mach3_ordered[i + len(paramNameList) - 5])
    else:
        final_mach3_temp[i] = copy.deepcopy(mach3_ordered[i - 5])
    
final_mach3 = copy.deepcopy(final_mach3_temp)     
for i in xrange(0,len(paramNameList)):
    final_mach3[i][0 ] = copy.deepcopy(final_mach3_temp[i][0 ])   
    final_mach3[i][1 ] = copy.deepcopy(final_mach3_temp[i][1 ])    
    final_mach3[i][2 ] = copy.deepcopy(final_mach3_temp[i][2 ])      
    final_mach3[i][3 ] = copy.deepcopy(final_mach3_temp[i][3 ])    
    final_mach3[i][4 ] = copy.deepcopy(final_mach3_temp[i][4 ])     
    final_mach3[i][5 ] = copy.deepcopy(final_mach3_temp[i][5 ])      
    final_mach3[i][6 ] = copy.deepcopy(final_mach3_temp[i][6 ])      
    final_mach3[i][7 ] = copy.deepcopy(final_mach3_temp[i][7 ])      
    final_mach3[i][8 ] = copy.deepcopy(final_mach3_temp[i][8 ])      
    final_mach3[i][9 ] = copy.deepcopy(final_mach3_temp[i][9 ])      
    final_mach3[i][10] = copy.deepcopy(final_mach3_temp[i][10])      
    final_mach3[i][11] = copy.deepcopy(final_mach3_temp[i][11])   
    final_mach3[i][12] = copy.deepcopy(final_mach3_temp[i][12])     
    final_mach3[i][13] = copy.deepcopy(final_mach3_temp[i][13])      
            
#Now loop through the samples and histograms and put the correct number in each table entry.
for i in xrange(0,len(paramNameList)-1):
    
    iter_param = 0
    lineContent = "\\multirow{14}{*}{\\textbf{\\emph{" +  str(i+1)+" : "+ paramNameList[i].replace("_", "\_") + "}}}"
    
    stringVal = "{0:.2f}".format(round(sigma[i],2))
    lineContent += " & \\multirow{14}{*}{\\textbf{\\emph{" + stringVal + "}}}" 
    
    
    #Now loop through the histograms and add their values in too.
    for j,hist in enumerate(histogramList[i]):
        if iter_param != 0:
            lineContent = "  \cline{3-9} & "
         
        lineContent += " & " + sampleName[j]
        
        hist0 = histogramList0[i]
        hist2 = histogramList2[i]

        val = hist2[j].Projection(1,0).GetSumOfWeights()
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  " + stringVal

        val = float(final_mach3[i][j][0])
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  " + stringVal
        
        val = hist0[j].Projection(1,0).GetSumOfWeights()
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  " + stringVal
        
        val = float(final_mach3[i][j][1])
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  "  + stringVal 
                
        val = hist.Projection(1,0).GetSumOfWeights()
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  " + stringVal
        
        val = float(final_mach3[i][j][2])
        stringVal = "{0:.2f}".format(round(val,2))
        lineContent += " &  " + stringVal
        
        val = math.fabs((hist.Projection(1,0).GetSumOfWeights() - float(final_mach3[i][j][2]))/ hist.Projection(1,0).GetSumOfWeights())
        stringVal = "{0:.6f}".format(round(val,7))
        lineContent += " &  " + stringVal
        
        #Having reached the end of the line, terminate it.
        lineContent += " \\\\ \n"

        #Now the line is complete.  Write it out to the output file.
        outFile.write(lineContent)
        iter_param = 1
    outFile.write("\\midrule[1.3pt]\n")
    if (i+1)%3 == 0:
        outFile.write("\\end{tabular}\n")
        outFile.write("}\n")
        outFile.write("\\centering\n")
        outFile.write("\caption{Event rate broken by sample for one $\sigma$ variation of each parameter. MaCh3 and BANFF are presented for comparison}\n")
        outFile.write("\\end{table}\n")
        outFile.write("\\addtocounter{table}{-1}\n") 
        outFile.write("\\begin{table}[ht!]\n")
        outFile.write("\\resizebox{0.95\\textwidth}{!}{\n")     
        outFile.write("\\centering\n")
        outFile.write("\\renewcommand{\\arraystretch}{1.2}\n")
        outFile.write("\\begin{tabular}{ c  c || c || c | c || c | c || c | c || c |}\n")
        outFile.write("\\midrule[1.3pt]\n")


        outFile.write("\\textbf{\\emph{Parameter}}& \\textbf{\\emph{$\\mathbf{1\sigma}$ value}} & \\textbf{\\emph{Sample}}  &  \\textbf{\\emph{BANFF $\\mathbf{-1\sigma}$}}  &  \\textbf{MaCh3 \\emph{$\\mathbf{-1\sigma}$}}  &  \\textbf{\\emph{BANFF nominal}}  &  \\textbf{\\emph{MaCh3 nominal}}  &  \\textbf{\\emph{BANFF $\\mathbf{+1\sigma}$}} &  \\textbf{\\emph{MaCh3 $\\mathbf{+1\sigma}$}} & (\\textbf{\\emph{BANFF - MaCh3 / BANFF) $\\mathbf{+1\sigma}$}} \\\\ \n")
        outFile.write("\\midrule[1.3pt]\n")
        
#With all lines of the table written, terminate the document and close the
#output file.
outFile.write("\\end{tabular}\n")
outFile.write("}\n")
outFile.write("\\centering\n")
outFile.write("\caption{Event rate broken by sample for one $\sigma$ variation of each parameter. MaCh3 and BANFF are presented for comparison}\n")
outFile.write("\\end{table}\n")
outFile.write("\\addtocounter{table}{-1}\n") 
outFile.write("\\end{document}\n")
outFile.close()


#!/usr/bin/python
from ROOT import *
import sys
import math
import os

postfitFile = TFile(sys.argv[1])

paramNames = postfitFile.Get("param_list")
prefitParams = postfitFile.Get("prefit_params")
prefitCov = postfitFile.Get("prefit_cov")

postfitParams = postfitFile.Get("postfit_params")
postfitCov = postfitFile.Get("postfit_cov")

outFile = open(sys.argv[2],"w")
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage[paperwidth=8.5in, paperheight=11.0in,margin=0.25in]{geometry}")
outFile.write("\\usepackage{longtable}")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{centering}\n")


#Set up the parameter ranges for the tables.
parameterRanges = [[0,24] #NuMode SK
        ,[25,49] #ANuMode SK
        ,[50,postfitParams.GetNrows()-1]] #Xsec

#Multiplicative factors stored in a dictionary for parameters that have them.
neutNom = {}
neutNom[106] = 1.2 #MAQE
neutNom[107] = 217 #pF_C
neutNom[108] = 225 #pF_O 
neutNom[112] = 100 #MEC_O
neutNom[113] = 100 #MEC_O
neutNom[119] = 1.01 #CA5
neutNom[120] = 0.95 #MARES
neutNom[121] = 1.30 #BgSclRes


units = {}
units[106] = "GeV/c$^2$" #MAQE
units[107] = "MeV/c" #pF_C
units[108] = "MeV/c" #pF_O #pF_O 
units[112] = "\%" #MEC_O
units[113] = "\%" #MEC_O
units[120] = "GeV/c$^2$" #MARES


#Now, for each parameter range, generate a table.
for paramRange in parameterRanges:
    tableString = "\\begin{longtable}{ | c | c | c |}\n"
    outFile.write(tableString)
    outFile.write("\\hline\n")

    tableString = "Parameter & PreFit & PostFit \\\\"
    outFile.write(tableString)

    for ip in xrange(paramRange[0],paramRange[1] +1):

        paramString = "\\hline\n"
    
        paramName = paramNames.At(ip).GetString().Data() 
        paramName = paramName.replace("_","\\char`_")
        if(ip in units):
            paramName += "("+units[ip]+")"
       
        paramString += "\\texttt{"+ paramName + "} "
        prefitVal = prefitParams[ip]
        prefitErr = math.sqrt(prefitCov[ip][ip])

        postfitVal = postfitParams[ip]
        postfitErr = math.sqrt(postfitCov[ip][ip])

        if ip in neutNom:
            prefitVal = prefitVal*neutNom[ip]
            prefitErr = prefitErr*neutNom[ip]
            
            postfitVal = postfitVal*neutNom[ip]
            postfitErr = postfitErr*neutNom[ip]


        stringVal = '%s' % float('%.5g' % prefitVal)
        stringErr = '%s' % float('%.5g' % prefitErr)
        paramString += "& " + stringVal + " $\pm$ " + stringErr + " "

        stringVal = '%s' % float('%.5g' % postfitVal)
        stringErr = '%s' % float('%.5g' % postfitErr)
        paramString += "& " + stringVal + " $\pm$ " + stringErr + " "
        paramString += "\\\\ "
        outFile.write(paramString)

    tableString =  "\n\\hline\n\\end{longtable}\n"
    outFile.write(tableString)
#    outFile.write("\\\\\n")
    outFile.write("\\vspace{2.5cm}")
outFile.write("\\end{centering}\n")
outFile.write("\\end{document}\n")
outFile.close()

#Run latex out the output file.
os.system("pdflatex " + sys.argv[2])

import ROOT
import TurnOnPlot_DATA as TurnOnPlot
import math

### Edit here ###

# TRIGGERS MUST BE DECLARED
#triggers = ["HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v", "HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v", "Pt_26GeV", "Pt_34GeV"]
trigger = "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"
# PLOT TITLES
#plotTitles = ["HLT MediumIsoPFTau32 Data - MC", "HLT MediumIsoPFTau20 Data - MC"]
# ROOT FILE CONTAINING THE DATA
dataFileName = "fitOutput_SingleElectron.root"
# ROOT FILE CONTAINING THE MC
DYFileName = "fitOutput_DYJetsToLLM50EG_Trig35_looseMuCut.root"
#mcFileName = "FittedTurnOn_Final_MC_0_500GeV.root"

### Do not edit from here ###
datafile = ROOT.TFile.Open(dataFileName)
mcfile = ROOT.TFile.Open(DYFileName)

# plotting style
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat()
ROOT.gStyle.SetOptFit(0)
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetFrameLineWidth(1)
ROOT.gStyle.SetPadBottomMargin(0.13)
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadRightMargin(0.05)

ROOT.gStyle.SetLabelFont(42,"X")
ROOT.gStyle.SetLabelFont(42,"Y")
ROOT.gStyle.SetLabelSize(0.04,"X")
ROOT.gStyle.SetLabelSize(0.04,"Y")
ROOT.gStyle.SetLabelOffset(0.01,"Y")
ROOT.gStyle.SetTickLength(0.02,"X")
ROOT.gStyle.SetTickLength(0.02,"Y")
ROOT.gStyle.SetLineWidth(1)
ROOT.gStyle.SetTickLength(0.02 ,"Z")

ROOT.gStyle.SetTitleSize(0.1)
ROOT.gStyle.SetTitleFont(42,"X")
ROOT.gStyle.SetTitleFont(42,"Y")
ROOT.gStyle.SetTitleSize(0.05,"X")
ROOT.gStyle.SetTitleSize(0.05,"Y")
ROOT.gStyle.SetTitleOffset(1.1,"X")
ROOT.gStyle.SetTitleOffset(1.4,"Y")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetPaintTextFormat("3.2f")
ROOT.gROOT.ForceStyle()

histo_Data = datafile.Get("histo_" + trigger)
histo_MC = mcfile.Get("histo_" + trigger)
axis = histo_Data.GetXaxis()
nbins = axis.GetNbins()
scalefact_eFakes = ROOT.TGraphAsymmErrors(nbins)
y1 = histo_Data.GetY()
y2 = histo_MC.GetY()
for val1, val2 in zip(y1, y2):
    print "{} \t {} \t {}".format(val1, val2, val1 / val2)
for binn, ydata, ymc in zip(range(nbins), y1, y2):
    x, y = ROOT.Double(0.), ROOT.Double(0.)
    histo_Data.GetPoint(binn, x, y)
    yDataLow, yDataHigh = histo_Data.GetErrorYlow(binn), histo_Data.GetErrorYhigh(binn)
    yDataErr = (yDataLow + yDataHigh)/2.
    yMCLow, yMCHigh = histo_MC.GetErrorYlow(binn), histo_MC.GetErrorYhigh(binn)
    yMCErr = (yMCLow + yMCHigh)/2.
    if math.isnan(ydata) or math.isnan(ymc):
        scalefact_eFakes.SetPoint(binn, x, 0.)
    else:
        scalefact_eFakes.SetPoint(binn, x, float(ydata) / ymc)
        #try:
        #    ylow = yDataLow/yMCHigh
        #except ZeroDivisionError:
        #    ylow = 0
        #try:
        #    yhigh = yDataHigh/yMCLow
        #except ZeroDivisionError:
        #    yhigh = 0
        yerr = float(ydata) / ymc * math.sqrt(yDataErr**2/ydata**2 + yMCErr**2/ymc**2)
        xerr = histo_Data.GetErrorXlow(binn)
        scalefact_eFakes.SetPointError(binn, xerr, xerr, yerr, yerr)

datafile.Close()
mcfile.Close()

# Create scalefactor graph for jet fakes.

datafile = ROOT.TFile.Open("fitOutput_Data_jetEnriched.root")
mcfile = ROOT.TFile.Open("fitOutput_JetFakes.root")

histo_Data = datafile.Get("histo_" + trigger)
histo_MC = mcfile.Get("histo_" + trigger)
axis = histo_Data.GetXaxis()
nbins = axis.GetNbins()
scalefact_jetFakes = ROOT.TGraphAsymmErrors(nbins)
y1 = histo_Data.GetY()
y2 = histo_MC.GetY()
for binn, ydata, ymc in zip(range(nbins), y1, y2):
    x, y = ROOT.Double(0.), ROOT.Double(0.)
    histo_Data.GetPoint(binn, x, y)
    yDataLow, yDataHigh = histo_Data.GetErrorYlow(binn), histo_Data.GetErrorYhigh(binn)
    yDataErr = (yDataLow + yDataHigh)/2.
    yMCLow, yMCHigh = histo_MC.GetErrorYlow(binn), histo_MC.GetErrorYhigh(binn)
    yMCErr = (yMCLow + yMCHigh)/2.
    if math.isnan(ydata) or math.isnan(ymc):
        scalefact_jetFakes.SetPoint(binn, x, 0.)
    else:
        scalefact_jetFakes.SetPoint(binn, x, float(ydata) / ymc)
        #try:
        #    ylow = yDataLow/yMCHigh
        #except ZeroDivisionError:
        #    ylow = 0
        #try:
        #    yhigh = yDataHigh/yMCLow
        #except ZeroDivisionError:
        #    yhigh = 0
        yerr = float(ydata) / ymc * math.sqrt(yDataErr**2/ydata**2 + yMCErr**2/ymc**2)
        xerr = histo_Data.GetErrorXlow(binn)
        scalefact_jetFakes.SetPointError(binn, xerr, xerr, yerr, yerr)




canvas1 = ROOT.TCanvas()
canvas1.SetLogx()
canvas1.SetGrid()
scalefact_eFakes.SetTitle("")
scalefact_eFakes.GetXaxis().SetTitle("Offline p^{#tau}_{T}")
scalefact_eFakes.GetYaxis().SetTitle("data / MC")
scalefact_eFakes.GetXaxis().SetMoreLogLabels()
scalefact_eFakes.GetXaxis().SetRangeUser(80, 1000)
scalefact_eFakes.SetMarkerStyle(20)
scalefact_eFakes.SetLineWidth(2)
scalefact_eFakes.Draw("APZ")
canvas1.Update()
canvas1.Print("plots/SF_eToTauFakes.pdf", "pdf")
canvas1.Print("plots/SF_eToTauFakes.png", "png")
canvas1.Print("plots/SF_eToTauFakes.eps", "eps")
#raw_input()

canvas2 = ROOT.TCanvas()
canvas2.SetLogx()
canvas2.SetGrid()
scalefact_jetFakes.SetTitle("")
scalefact_jetFakes.GetXaxis().SetTitle("Offline p^{#tau}_{T}")
scalefact_jetFakes.GetYaxis().SetTitle("data / MC")
scalefact_jetFakes.GetXaxis().SetMoreLogLabels()
scalefact_jetFakes.GetXaxis().SetRangeUser(80, 1000)
scalefact_jetFakes.SetMarkerStyle(20)
scalefact_jetFakes.SetLineWidth(2)
scalefact_jetFakes.Draw("APZ")
canvas2.Update()
canvas2.Print("plots/SF_jetToTauFakes.pdf", "pdf")
canvas2.Print("plots/SF_jetToTauFakes.png", "png")
canvas2.Print("plots/SF_jetToTauFakes.eps", "eps")
#raw_input()

canvas3 = ROOT.TCanvas()
canvas3.SetLogx()
canvas3.SetGrid()
scalefact_eFakes.SetTitle("")
scalefact_eFakes.GetXaxis().SetTitle("Offline p^{#tau}_{T}")
scalefact_eFakes.GetYaxis().SetTitle("data / MC")
scalefact_eFakes.GetXaxis().SetMoreLogLabels()
scalefact_eFakes.SetMarkerStyle(20)
scalefact_eFakes.SetLineWidth(2)
scalefact_jetFakes.GetXaxis().SetRangeUser(80, 1000)
scalefact_jetFakes.GetYaxis().SetRangeUser(0, 1.56)
scalefact_jetFakes.SetMarkerStyle(21)
scalefact_jetFakes.SetLineWidth(2)
scalefact_jetFakes.SetMarkerColor(ROOT.kBlue+1)
scalefact_eFakes.SetMarkerColor(ROOT.kGreen+1)
scalefact_jetFakes.SetLineColor(ROOT.kBlue+1)
scalefact_eFakes.SetLineColor(ROOT.kGreen+1)
scalefact_jetFakes.Draw("APZ")
scalefact_eFakes.Draw("same PZ")
legend = ROOT.TLegend(0.77, 0.77, 0.9485, 0.9385)
legend.SetTextFont(42)
legend.SetFillColor(10)
#legend.SetTextSize(0.9*extraTextSize)
legend.SetBorderSize(0)
legend.SetFillStyle(1001)
legend.SetTextSize(0.76*0.07)
legend.AddEntry(scalefact_eFakes, "e #rightarrow #tau_{h}", "lep")
legend.AddEntry(scalefact_jetFakes, "jet #rightarrow #tau_{h}", "lep")
legend.Draw()

canvas3.Update()
canvas3.Print("plots/SF_comparison.pdf", "pdf")
canvas3.Print("plots/SF_comparison.png", "png")
canvas3.Print("plots/SF_comparison.eps", "eps")
raw_input()


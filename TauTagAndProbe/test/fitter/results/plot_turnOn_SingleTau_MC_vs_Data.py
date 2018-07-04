import ROOT
import TurnOnPlot_DATA as TurnOnPlot

### Edit here ###

# TRIGGERS MUST BE DECLARED
#triggers = ["HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v", "HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v", "Pt_26GeV", "Pt_34GeV"]
triggers = ["HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"]
# PLOT TITLES
#plotTitles = ["HLT MediumIsoPFTau32 Data - MC", "HLT MediumIsoPFTau20 Data - MC"]
# ROOT FILE CONTAINING THE DATA
#dataFileName = "FittedTurnOn_Final_Data_0_500GeV.root"
tRed = ROOT.TColor(3001, 1., 0., 0., "tRed"     , 0.35);
tGreen   = ROOT.TColor(3002, 0., 1., 0., "tGreen"   , 0.35);
tBlue    = ROOT.TColor(3003, 0., 0., 1., "tBlue"    , 0.35);
tMagenta = ROOT.TColor(3004, 1., 0., 1., "tMagenta" , 0.35);
tCyan    = ROOT.TColor(3005, 0., 1., 1., "tCyan"    , 0.35);
tYellow  = ROOT.TColor(3006, 1., 1., 0., "tYellow"  , 0.35);
tOrange  = ROOT.TColor(3007, 1., .5, 0., "tOrange"  , 0.35);

plot_dict = {1: {'Name': "DYJetsToLLM50EG_Trig35_looseMuCut",
                 'Legend': "MC e #rightarrow #tau_{h}",
                 'MarkerColor': ROOT.kGreen+3,
                 'MarkerStyle': 22,
                 'LineColor': ROOT.kGreen+3,
                 'LineStyle': 1,
                 'drawOption': "3l",
                 'FillColor': tGreen,
                 'FillStyle': 1001,
                 'LegendOptions': "fl"
                 },
             3: {'Name': "mc_WJets",
                 'Legend': "MC jet #rightarrow #tau_{h}",
                 'MarkerColor': ROOT.kBlue+2,
                 'MarkerStyle': 21,
                 'LineColor': ROOT.kBlue+2,
                 'LineStyle': 1,
                 'drawOption': "3l",
                 'FillColor': tBlue,
                 'FillStyle': 1001,
                 'LegendOptions': "fl"
                 },
             5: {'Name': "ZprimeToTauTau",
                 'Legend': "MC real #tau_{h}",
                 'MarkerColor': ROOT.kRed+2,
                 'MarkerStyle': 20,
                 'LineColor': ROOT.kRed+2,
                 'LineStyle': 1,
                 'drawOption': "3l",
                 'FillColor': tRed,
                 'FillStyle': 1001,
                 'LegendOptions': "fl"
                 },
             4: {'Name': "Data_jetEnriched",
                 'Legend': "2017 data, jet #rightarrow #tau_{h} enriched",
                 'MarkerColor': ROOT.kBlue,
                 'MarkerStyle': 21,
                 'LineColor': ROOT.kBlue,
                 'LineStyle': 1,
                 'drawOption': "pl",
                 'FillColor': tOrange,
                 'FillStyle': 1001,
                 'LegendOptions': "lep"
                 },
             2: {'Name': "SingleElectron",
                 'Legend': "2017 data, e #rightarrow #tau_{h} enriched",
                 'MarkerColor': ROOT.kGreen+1,
                 'MarkerStyle': 22,
                 'LineColor': ROOT.kGreen+1,
                 'LineStyle': 1,
                 'drawOption': "pl",
                 'FillColor': tOrange,
                 'FillStyle': 1001,
                 'LegendOptions': "lep"
                 }
             }

# ROOT FILE CONTAINING THE MC

### Do not edit from here ###
turnons = []
plots = []

for trigger in triggers:
    #for infile, nick, leg, col, dOpt, mark, fill in zip(inputFiles, nicks, legend_entries, colors, drawOptions, markers, fills):
    for plot in sorted(plot_dict.keys()):
        infile = ROOT.TFile.Open("fitOutput_" + plot_dict[plot]['Name'] + ".root")
        plot_dict[plot]['Histo'] = infile.Get("histo_" + trigger)
        plot_dict[plot]['Histo'].__class__ = ROOT.RooHist
        plot_dict[plot]['Fit'] = infile.Get("fit_" + trigger)
        plot_dict[plot]['Fit'].__class__ = ROOT.RooCurve
        infile.Close()
        turnons.append(TurnOnPlot.TurnOn(**plot_dict[plot]))
    plots.append(TurnOnPlot.TurnOnPlot(TriggerName = trigger + "genuine fake"))
    plots[-1].name = "turnOn_MC_" + trigger + "_vsData"
    plots[-1].xRange = (80, 1000)
    #plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
    plots[-1].legendPosition = (0.44,0.15,0.94,0.33)
    plots[-1].lumi = "41.29 fb^{-1} "
    plots[-1].numLegCols = 2
    for turnon in turnons:
        plots[-1].addTurnOn(turnon)

canvas = []
for plot in plots:
    canvas.append(plot.plot())


#for f in inputFiles:
#    f.Close()

raw_input()


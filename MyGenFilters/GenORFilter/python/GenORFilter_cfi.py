import FWCore.ParameterSet.Config as cms

genORFilter = cms.EDFilter(
    "GenORFilter",
    jets           = cms.InputTag("tmpAk4GenJetsNoNu"),
    mets           = cms.InputTag("tmpGenMetTrue"),
    leptonsAll     = cms.InputTag("genLeptonsAll"),
    leptonsHard    = cms.InputTag("genLeptonsFromHardProcess"),
    jetPtCut       = cms.double(20.0),
    jetEtaCut      = cms.double(2.5),
    genHTcut       = cms.double(50.0),
    genMETcut      = cms.double(80.0),
    minLeptons     = cms.uint32(2),
    minLeptonsHard = cms.uint32(2)
)

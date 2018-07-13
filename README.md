# HEPStuff
For generating decay/reconstruction data with RestFrames to be used later

Reconstructor uses the RestFrames package (more information at http://restframes.com/) to generate data for simulated particle decay trees and their reconstructions and stores this data in a TTree. dataSetGenerator takes in this data from the TTree and converts it to unbinned RooDataSets which can be used to generate probability distribution functions in RooFit.

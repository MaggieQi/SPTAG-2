#ifdef DefineBKTParameter

// DefineBKTParameter(VarName, VarType, DefaultValue, RepresentStr)
DefineBKTParameter(m_sBKTFilename, std::string, std::string("tree.bin"), "TreeFilePath")
DefineBKTParameter(m_sGraphFilename, std::string, std::string("graph.bin"), "GraphFilePath")
DefineBKTParameter(m_sDataPointsFilename, std::string, std::string("vectors.bin"), "VectorFilePath")

DefineBKTParameter(m_pTrees.m_iTreeNumber, int, 1L, "BKTNumber")
DefineBKTParameter(m_pTrees.m_iBKTKmeansK, int, 32L, "BKTKmeansK")
DefineBKTParameter(m_pTrees.m_iBKTLeafSize, int, 8L, "BKTLeafSize")
DefineBKTParameter(m_pTrees.m_iSamples, int, 1000L, "Samples")


DefineBKTParameter(m_pGraph.m_iTPTNumber, int, 32L, "TpTreeNumber")
DefineBKTParameter(m_pGraph.m_iTPTLeafSize, int, 2000L, "TPTLeafSize")
DefineBKTParameter(m_pGraph.m_numTopDimensionTPTSplit, int, 5L, "NumTopDimensionTpTreeSplit")

DefineBKTParameter(m_pGraph.m_iNeighborhoodSize, int, 32L, "NeighborhoodSize")
DefineBKTParameter(m_pGraph.m_iNeighborhoodScale, int, 16L, "GraphNeighborhoodScale")
DefineBKTParameter(m_pGraph.m_iCEFScale, int, 4L, "GraphCEFScale")
DefineBKTParameter(m_pGraph.m_iRefineIter, int, 0L, "RefineIterations")
DefineBKTParameter(m_pGraph.m_iCEF, int, 1000L, "CEF")
DefineBKTParameter(m_pGraph.m_iMaxCheckForRefineGraph, int, 10000L, "MaxCheckForRefineGraph")

DefineBKTParameter(m_iNumberOfThreads, int, 1L, "NumberOfThreads")
DefineBKTParameter(m_iDistCalcMethod, SPTAG::DistCalcMethod, SPTAG::DistCalcMethod::Cosine, "DistCalcMethod")

DefineBKTParameter(m_iMaxCheck, int, 8192L, "MaxCheck")
DefineBKTParameter(m_iThresholdOfNumberOfContinuousNoBetterPropagation, int, 3L, "ThresholdOfNumberOfContinuousNoBetterPropagation")
DefineBKTParameter(m_iNumberOfInitialDynamicPivots, int, 50L, "NumberOfInitialDynamicPivots")
DefineBKTParameter(m_iNumberOfOtherDynamicPivots, int, 4L, "NumberOfOtherDynamicPivots")

#endif

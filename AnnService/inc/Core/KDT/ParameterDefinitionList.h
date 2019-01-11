#ifdef DefineKDTParameter

// DefineKDTParameter(VarName, VarType, DefaultValue, RepresentStr)
DefineKDTParameter(m_sKDTFilename, std::string, std::string("tree.bin"), "TreeFilePath")
DefineKDTParameter(m_sGraphFilename, std::string, std::string("graph.bin"), "GraphFilePath")
DefineKDTParameter(m_sDataPointsFilename, std::string, std::string("vectors.bin"), "VectorFilePath")

DefineKDTParameter(m_pTrees.m_iTreeNumber, int, 1L, "KDTNumber")
DefineKDTParameter(m_pTrees.m_numTopDimensionKDTSplit, int, 5L, "NumTopDimensionKDTSplit")
DefineKDTParameter(m_pTrees.m_iSamples, int, 100L, "NumSamplesKDTSplitConsideration")

DefineKDTParameter(m_pGraph.m_iTPTNumber, int, 32L, "TPTNumber")
DefineKDTParameter(m_pGraph.m_iTPTLeafSize, int, 2000L, "TPTLeafSize")
DefineKDTParameter(m_pGraph.m_numTopDimensionTPTSplit, int, 5L, "NumTopDimensionTPTSplit")

DefineKDTParameter(m_pGraph.m_iNeighborhoodSize, int, 32L, "NeighborhoodSize")
DefineKDTParameter(m_pGraph.m_iNeighborhoodScale, int, 16L, "GraphNeighborhoodScale")
DefineKDTParameter(m_pGraph.m_iCEFScale, int, 4L, "GraphCEFScale")
DefineKDTParameter(m_pGraph.m_iRefineIter, int, 0L, "RefineIterations")
DefineKDTParameter(m_pGraph.m_iCEF, int, 1000L, "CEF")
DefineKDTParameter(m_pGraph.m_iMaxCheckForRefineGraph, int, 10000L, "MaxCheckForRefineGraph")

DefineKDTParameter(m_iNumberOfThreads, int, 1L, "NumberOfThreads")
DefineKDTParameter(m_iDistCalcMethod, SPTAG::DistCalcMethod, SPTAG::DistCalcMethod::Cosine, "DistCalcMethod")

DefineKDTParameter(m_iMaxCheck, int, 8192L, "MaxCheck")
DefineKDTParameter(m_iThresholdOfNumberOfContinuousNoBetterPropagation, int, 3L, "ThresholdOfNumberOfContinuousNoBetterPropagation")
DefineKDTParameter(m_iNumberOfInitialDynamicPivots, int, 50L, "NumberOfInitialDynamicPivots")
DefineKDTParameter(m_iNumberOfOtherDynamicPivots, int, 4L, "NumberOfOtherDynamicPivots")

#endif

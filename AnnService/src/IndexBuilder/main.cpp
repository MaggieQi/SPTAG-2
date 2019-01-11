#include "inc/IndexBuilder/ThreadPool.h"
#include "inc/IndexBuilder/Options.h"
#include "inc/IndexBuilder/VectorSetReader.h"
#include "inc/Core/VectorIndex.h"
#include "inc/Core/Common.h"
#include "inc/Helper/SimpleIniReader.h"

#include <memory>
#include <iostream>

using namespace SPTAG;

int main(int argc, char* argv[])
{
    std::shared_ptr<IndexBuilder::BuilderOptions> options(new IndexBuilder::BuilderOptions);
    if (!options->Parse(argc - 1, argv + 1))
    {
        exit(1);
    }
    IndexBuilder::ThreadPool::Init(options->m_threadNum);
    auto indexBuilder = VectorIndex::CreateInstance(options->m_indexAlgoType, options->m_inputValueType);

    Helper::IniReader iniReader;
    if (!options->m_builderConfigFile.empty())
    {
        iniReader.LoadIniFile(options->m_builderConfigFile);
    }

    for (int i = 1; i < argc; i++)
    {
        std::string param(argv[i]);
        int idx = (int)param.find("=");
        if (idx < 0) continue;

        std::string paramName = param.substr(0, idx);
        std::string paramVal = param.substr(idx + 1);
        std::string sectionName;
        idx = (int)paramName.find(".");
        if (idx >= 0) {
            sectionName = paramName.substr(0, idx);
            paramName = paramName.substr(idx + 1);
        }
        iniReader.SetParameter(sectionName, paramName, paramVal);
        std::cout << "Set [" << sectionName << "]" << paramName << " = " << paramVal << std::endl;
    }

    if (!iniReader.DoesParameterExist("Index", "NumberOfThreads")) {
        iniReader.SetParameter("Index", "NumberOfThreads", std::to_string(options->m_threadNum));
    }
    for (const auto& iter : iniReader.GetParameters("Index"))
    {
        indexBuilder->SetParameter(iter.first.c_str(), iter.second.c_str());
    }

    ErrorCode code;
    if (options->m_inputFiles.find("BIN:") == 0) {
        options->m_inputFiles = options->m_inputFiles.substr(4);
        std::ifstream inputStream(options->m_inputFiles, std::ifstream::binary);
        if (!inputStream.is_open()) {
            fprintf(stderr, "Failed to read input file.\n");
            exit(1);
        }
        int row, col;
        inputStream.read((char*)&row, sizeof(int));
        inputStream.read((char*)&col, sizeof(int));
        std::uint64_t totalRecordVectorBytes = ((std::uint64_t)GetValueTypeSize(options->m_inputValueType)) * row * col;
        ByteArray vectorSet = ByteArray::Alloc(totalRecordVectorBytes);
        char* vecBuf = reinterpret_cast<char*>(vectorSet.Data());
        inputStream.read(vecBuf, totalRecordVectorBytes);
        inputStream.close();

        std::shared_ptr<VectorSet> p_vectorSet(new BasicVectorSet(vectorSet, options->m_inputValueType, col, row));
        code = indexBuilder->BuildIndex(p_vectorSet, nullptr);
        indexBuilder->SaveIndex(options->m_outputFolder);
    }
    else {
        auto vectorReader = IndexBuilder::VectorSetReader::CreateInstance(options);
        if (ErrorCode::Success != vectorReader->LoadFile(options->m_inputFiles))
        {
            fprintf(stderr, "Failed to read input file.\n");
            exit(1);
        }
        code = indexBuilder->BuildIndex(vectorReader->GetVectorSet(), vectorReader->GetMetadataSet());
        indexBuilder->SaveIndex(options->m_outputFolder);
    }

    if (ErrorCode::Success != code)
    {
        fprintf(stderr, "Failed to build index.\n");
        exit(1);
    }
    return 0;
}

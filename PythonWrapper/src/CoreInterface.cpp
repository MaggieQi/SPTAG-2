#include "inc/CoreInterface.h"
#include "inc/Helper/StringConvert.h"


AnnIndex::AnnIndex(SizeType p_dimension)
    : m_algoType(SPTAG::IndexAlgoType::BKT),
      m_inputValueType(SPTAG::VectorValueType::Float),
      m_dimension(p_dimension)
{
    m_inputVectorSize = SPTAG::GetValueTypeSize(m_inputValueType) * m_dimension;
}


AnnIndex::AnnIndex(const char* p_algoType, const char* p_valueType, SizeType p_dimension)
    : m_algoType(SPTAG::IndexAlgoType::Undefined),
      m_inputValueType(SPTAG::VectorValueType::Undefined),
      m_dimension(p_dimension)
{
    SPTAG::Helper::Convert::ConvertStringTo<SPTAG::IndexAlgoType>(p_algoType, m_algoType);
    SPTAG::Helper::Convert::ConvertStringTo<SPTAG::VectorValueType>(p_valueType, m_inputValueType);
    m_inputVectorSize = SPTAG::GetValueTypeSize(m_inputValueType) * m_dimension;
}


AnnIndex::AnnIndex(const std::shared_ptr<SPTAG::VectorIndex>& p_index)
    : m_algoType(p_index->GetIndexAlgoType()),
      m_inputValueType(p_index->GetVectorValueType()),
      m_dimension(p_index->GetFeatureDim()),
      m_index(p_index)
{
    m_inputVectorSize = SPTAG::GetValueTypeSize(m_inputValueType) * m_dimension;
}


AnnIndex::~AnnIndex()
{
}


bool
AnnIndex::Build(ByteArray p_data, SizeType p_num)
{
    if (nullptr == m_index)
    {
        m_index = SPTAG::VectorIndex::CreateInstance(m_algoType, m_inputValueType);
    }
    if (nullptr == m_index || p_num == 0 || m_dimension == 0 || p_data.Length() != p_num * m_inputVectorSize)
    {
        return false;
    }
    return (SPTAG::ErrorCode::Success == m_index->BuildIndex(p_data.Data(), p_num, m_dimension));
}


bool
AnnIndex::BuildWithMetaData(ByteArray p_data, ByteArray p_meta, SizeType p_num)
{
    if (nullptr == m_index)
    {
        m_index = SPTAG::VectorIndex::CreateInstance(m_algoType, m_inputValueType);
    }
    if (nullptr == m_index || p_num == 0 || m_dimension == 0 || p_data.Length() != p_num * m_inputVectorSize)
    {
        return false;
    }

    std::shared_ptr<SPTAG::VectorSet> vectors(new SPTAG::BasicVectorSet(p_data,
        m_inputValueType,
        static_cast<SPTAG::SizeType>(m_dimension),
        static_cast<SPTAG::SizeType>(p_num)));

    std::uint64_t* offsets = new std::uint64_t[p_num + 1]{ 0 };
    int current = 1;
    for (size_t i = 0; i < p_meta.Length(); i++) {
        if (((char)p_meta.Data()[i]) == '\n')
            offsets[current++] = (std::uint64_t)(i + 1);
    }
    std::shared_ptr<SPTAG::MetadataSet> meta(new SPTAG::MemMetadataSet(p_meta, ByteArray((std::uint8_t*)offsets, (p_num + 1) * sizeof(std::uint64_t), true), p_num));
    return (SPTAG::ErrorCode::Success == m_index->BuildIndex(vectors, meta));
}


void
AnnIndex::SetBuildParam(const char* p_name, const char* p_value)
{
    if (nullptr == m_index) 
    {
        if (SPTAG::IndexAlgoType::Undefined == m_algoType || 
            SPTAG::VectorValueType::Undefined == m_inputValueType)
        {
            return;    
        }
        m_index = SPTAG::VectorIndex::CreateInstance(m_algoType, m_inputValueType);

    }
    m_index->SetParameter(p_name, p_value);
}


void
AnnIndex::SetSearchParam(const char* p_name, const char* p_value)
{
    if (nullptr != m_index) m_index->SetParameter(p_name, p_value);
}


std::shared_ptr<QueryResult>
AnnIndex::Search(ByteArray p_data, SizeType p_resultNum)
{
    std::shared_ptr<QueryResult> results = std::make_shared<QueryResult>(p_data.Data(), p_resultNum, false);

    if (nullptr != m_index && p_data.Length() == m_inputVectorSize)
    {
        m_index->SearchIndex(*results);
    }
    return std::move(results);
}

std::shared_ptr<QueryResult>
AnnIndex::SearchWithMetaData(ByteArray p_data, SizeType p_resultNum)
{
    std::shared_ptr<QueryResult> results = std::make_shared<QueryResult>(p_data.Data(), p_resultNum, true);

    if (nullptr != m_index && p_data.Length() == m_inputVectorSize)
    {
        m_index->SearchIndex(*results);
    }
    return std::move(results);
}

bool
AnnIndex::ReadyToServe() const
{
    return m_index != nullptr;
}


bool
AnnIndex::Save(const char* p_savefile) const
{
    return SPTAG::ErrorCode::Success == m_index->SaveIndex(p_savefile);
}


AnnIndex
AnnIndex::Load(const char* p_loaderFile)
{
    std::shared_ptr<SPTAG::VectorIndex> vecIndex;
    auto ret = SPTAG::VectorIndex::LoadIndex(p_loaderFile, vecIndex);
    if (SPTAG::ErrorCode::Success != ret || nullptr == vecIndex)
    {
        return AnnIndex(0);
    }

    return AnnIndex(vecIndex);
}


bool 
AnnIndex::Add(ByteArray p_data, SizeType p_num)
{
    if (nullptr == m_index)
    {
        m_index = SPTAG::VectorIndex::CreateInstance(m_algoType, m_inputValueType);
    }
    if (nullptr == m_index || p_num == 0 || m_dimension == 0 || p_data.Length() != p_num * m_inputVectorSize)
    {
        return false;
    }
    return (SPTAG::ErrorCode::Success == m_index->AddIndex(p_data.Data(), p_num, m_dimension));
}


bool
AnnIndex::AddWithMetaData(ByteArray p_data, ByteArray p_meta, SizeType p_num)
{
    if (nullptr == m_index)
    {
        m_index = SPTAG::VectorIndex::CreateInstance(m_algoType, m_inputValueType);
    }
    if (nullptr == m_index || p_num == 0 || m_dimension == 0 || p_data.Length() != p_num * m_inputVectorSize)
    {
        return false;
    }

    std::shared_ptr<SPTAG::VectorSet> vectors(new SPTAG::BasicVectorSet(p_data,
        m_inputValueType,
        static_cast<SPTAG::SizeType>(m_dimension),
        static_cast<SPTAG::SizeType>(p_num)));

    std::uint64_t* offsets = new std::uint64_t[p_num + 1]{ 0 };
    int current = 1;
    for (size_t i = 0; i < p_meta.Length(); i++) {
        if (((char)p_meta.Data()[i]) == '\n')
            offsets[current++] = (std::uint64_t)(i + 1);
    }
    std::shared_ptr<SPTAG::MetadataSet> meta(new SPTAG::MemMetadataSet(p_meta, ByteArray((std::uint8_t*)offsets, (p_num + 1) * sizeof(std::uint64_t), true), p_num));
    return (SPTAG::ErrorCode::Success == m_index->AddIndex(vectors, meta));
}


bool
AnnIndex::Delete(ByteArray p_data, SizeType p_num)
{
    if (nullptr != m_index && p_num > 0)
    {
        return (SPTAG::ErrorCode::Success == m_index->DeleteIndex(p_data.Data(), p_num));
    }
    return false;
}

bool 
AnnIndex::Refine(const char* p_loaderFile)
{
    return (SPTAG::ErrorCode::Success == m_index->RefineIndex(std::string(p_loaderFile)));
}
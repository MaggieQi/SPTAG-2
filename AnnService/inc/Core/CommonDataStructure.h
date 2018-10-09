#ifndef _SPACEV_COMMONDATASTRUCTURE_H_
#define _SPACEV_COMMONDATASTRUCTURE_H_

#include "Common.h"

#include <memory>

namespace SpaceV
{

class ByteArray
{
public:
    ByteArray();

    ByteArray(ByteArray&& p_right);

    ByteArray(std::uint8_t* p_array, std::size_t p_length, bool p_transferOnwership);

    ByteArray(std::uint8_t* p_array, std::size_t p_length, std::shared_ptr<std::uint8_t> p_dataHolder);

    ByteArray(const ByteArray& p_right);

    ByteArray& operator= (const ByteArray& p_right);

    ByteArray& operator= (ByteArray&& p_right);

    ~ByteArray();

    static ByteArray Alloc(std::size_t p_length);

    std::uint8_t* Data() const;

    std::size_t Length() const;

    std::shared_ptr<std::uint8_t> DataHolder() const;

    void Clear();

    const static ByteArray c_empty;

private:
    std::uint8_t* m_data;

    std::size_t m_length;

    // Notice this is holding an array. Set correct deleter for this.
    std::shared_ptr<std::uint8_t> m_dataHolder;
};

} // namespace SpaceV

#endif // _SPACEV_COMMONDATASTRUCTURE_H_
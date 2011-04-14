
#ifndef _ICOMPRESSOR_H
#define _ICOMPRESSOR_H

#include "ByteBuffer.h"


class ICompressor : public ByteBuffer
{
public:
    typedef int (*ProgressCallback)(void *, uint64 , uint64);

    ICompressor(): _iscompressed(false), _real_size(0) {}
    virtual ~ICompressor() {}
    virtual void Compress(uint32 level = 1, ProgressCallback pcb = NULL) {}
    virtual void Decompress(void) {}


    bool Compressed(void) const { return _iscompressed; }
    void Compressed(bool b) { _iscompressed = b; }
    uint32 RealSize(void) const { return _iscompressed ? _real_size : size(); }
    void RealSize(uint32 realsize) { _real_size = realsize; }
    void clear(void) // not required to be strictly virtual; be careful not to mess up static types!
    {
        ByteBuffer::clear();
        _real_size = 0;
        _iscompressed = false;
    }

protected:
    bool _iscompressed;
    uint64 _real_size;
};


#endif

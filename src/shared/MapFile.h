#ifndef MAPFILE_H
#define MAPFILE_H

class LayerMgr;
class ByteBuffer;
class Engine;
class VFSFile;

class MapFile
{
public:
    static void Save(ByteBuffer* bufptr, LayerMgr *mgr);
    static bool SaveAsFileDirect(const char *fn, LayerMgr *mgr);
    static LayerMgr *LoadUnsafe(ByteBuffer *bufptr, Engine *engine, LayerMgr *target); // may throw ByteBufferException if file corrupt
    static LayerMgr *Load(ByteBuffer *bufptr, Engine *engine, LayerMgr *target = NULL);
    static LayerMgr *Load(memblock* mem,  Engine *engine, LayerMgr *target = NULL);


};


#endif

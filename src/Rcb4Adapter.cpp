#include "Rcb4Adapter.hpp"

namespace Roki
{
    bool Rcb4Adapter::synchronize(byte txBuf[], byte txLen, byte rxBuf[], byte rxLen)
    {
        RequestData = txBuf;
        ResponceData = rxBuf;
        RequestSize = txLen;
        ResponceSize = rxLen;
        return true;
    }

    uint8_t *Rcb4Adapter::GetRequestData() const { return RequestData; }
    uint8_t *Rcb4Adapter::GetResponceData() const { return ResponceData; }

    uint8_t Rcb4Adapter::GetRequestSize() const { return RequestSize; }
    uint8_t Rcb4Adapter::GetResponceSize() const { return ResponceSize; }
}
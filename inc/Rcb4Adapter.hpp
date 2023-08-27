#pragma once

#include "Rcb4BaseClass.hpp"

namespace Roki
{
    class Rcb4Adapter : public Rcb4BaseClass
    {
    private:
        uint8_t *RequestData;
        uint8_t *ResponceData;
        uint8_t RequestSize;
        uint8_t ResponceSize;

    public:
        virtual bool synchronize(byte txBuf[], byte txLen, byte rxBuf[], byte rxLen) override;

    public:
        uint8_t *GetRequestData() const;
        uint8_t *GetResponceData() const;
        uint8_t GetRequestSize() const;
        uint8_t GetResponceSize() const;
    };
}
#pragma once

#include "Rcb4BaseClass.hpp"
#include "BodyInterface.hpp"

namespace Roki {
    class Rcb4Adapter : public Rcb4BaseClass {
        private:
            uint8_t* RequestData;
            uint8_t RequestSize;
            uint8_t ResponceSize;
        public:

        virtual bool synchronize(byte txBuf[],byte txLen,byte rxBuf[],byte rxLen) { 
            RequestData = txBuf;
            RequestSize = txLen;
            ResponceSize = rxLen;
            return true;
        }

        public:
            uint8_t* GetRequestData() const {return RequestData; }
            uint8_t GetRequestSize() const {return RequestSize; }
            uint8_t GetResponceSize() const {return ResponceSize; }
    };
}
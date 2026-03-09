#pragma once
#include "../utils/sample/sample.h"
#include "LWEGContext.h"

class LWEG {
    public:
        LWEG(const PolyRing& ring, const LWEGContext& lwegContext);

        void initialize(std::vector<uint8_t>& d);

        void prng(std::vector<uint8_t>& bytes);
        
    private:
    	const PolyRing& ring;
        const LWEGContext& lwegContext;

        std::vector<PolyMatrix> vectorA;
        PolyVector s;
        PolyVector e;
};

#include "tests.h"

void perfomanceTest(LWEGContext &context, PolyRing& ring) {
    printf("\n-----------------PERFOMANCE TEST RESULT-----------------\n");
    printf(
        "LWEG context:\n\tm = %d\n\tqS = %d\n\tmartixRank = %d\n\tnoise = %d\n",
        context.m,
        context.qS,
        context.martixRank,
        context.noise
    );
    printf(
        "Ring:\n\tq = %d\n\tn = %d\n",
        ring.q,
        ring.n
    );

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<uint8_t> d(32);

    for (int i = 0; i < 32; i++) {
        d[i] = gen();
    }

    LWEG lweg(ring, context);
    lweg.initialize(d);

    std::vector<uint8_t> buffer(102400);

    long totalTime = 0;
    long testsCount = 1000;
    
    for(int i = 0; i < testsCount; i++) {
        auto startTime = std::chrono::high_resolution_clock::now();
        lweg.prng(buffer);
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        totalTime += duration.count();
    }

    float meanTime = (float)totalTime / testsCount;

    printf("Average time of generation 100Kb = %f ms\n", meanTime);
    printf("-----------------THE END-----------------\n");
}

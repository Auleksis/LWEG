#pragma once
struct LWEGContext {
    int m; // matricies A, vectors s count
    int qS; // modulo, a multiple of 256 for secure mapping Rq -> R256
    int polySize; // size of vectors and rank of matricies
    int noise; // noise for generation vetors s and e
};

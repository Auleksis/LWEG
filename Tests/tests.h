#pragma once

#include <stdlib.h>
#include <random>
#include <chrono>
#include "../LWEG/LWEG.h"

/**
 * Computes average time of generation 100 Kb using 1000 tests
 */
void perfomanceTest(LWEGContext& context, PolyRing& ring);

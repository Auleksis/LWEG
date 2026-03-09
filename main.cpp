#include "LWEG/LWEG.h"
#include <random>
#include <fstream>

int main() {
    PolyRing kyberRing(3329, 256);

    kyberRing.print();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<uint8_t> d(32);

    for (int i = 0; i < 32; i++) {
        d[i] = gen();
    }

    LWEGContext lwegContext = { 16, 3072, 4, 2 };
    LWEG lweg(kyberRing, lwegContext);
    lweg.initialize(d);

    std::vector<uint8_t> buffer(1024 * 1024 * 100);

    lweg.prng(buffer);

    std::ofstream file("data.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.close();
        printf("Файл сохранён, размер: %d байт\n", (int)buffer.size());
    } else {
        printf("Ошибка открытия файла\n");
    }

    return 0;
}

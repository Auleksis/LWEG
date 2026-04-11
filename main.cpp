#include <random>
#include <fstream>
#include <getopt.h>
#include "Tests/tests.h"

void printHelp() {
    std::printf(
        "Использование: ./lweg [опции] outputBytesLength\n"
        "Обязательный аргумент (не нужен при использовании опции -t):\n"
        "  outputBytesLength             размер выходных данных в байтах (целое число)\n"
        "Опции:\n"
        "  -s, --seed <число>            зерно для генератора (по умолчанию, случайное число из 32 байт)\n"
        "  -o, --output <файл>           имя выходного файла (по умолчанию, data.bin)\n"
        "  -t, --test <test_number>      Переход в режим тестирования. Игнорируются другие аргументы\n"
        "\tДоступные тесты:\n"
        "\t\t 1) Perfomance Test (1000 tests, 100 Kb)\n"
        "  -h, --help                    показать справку\n"
    );
}

int main(int argc, char** argv) {
    int opt;
    int option_index = 0;
    long seed = 0;
    bool seedProvided = false;
    std::string outputFilename = "data.bin";
    int testNumber = 0;

    PolyRing kyberRing(3329, 256);
    LWEGContext lwegContext1 = { 16, 3072, 2, 3 };
    LWEGContext lwegContext2 = { 16, 3072, 3, 2 };
    LWEGContext lwegContext3 = { 16, 3072, 4, 2 };

    // Описываем длинные опции
    static struct option long_options[] = {
        {"seed",   required_argument, 0, 's'},
        {"output", required_argument, 0, 'o'},
        {"test",   required_argument, 0, 't'},
        {"help",   no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "s:o:t:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 's':
                seed = std::atol(optarg);
                seedProvided = true;
                break;
            case 'o':
                outputFilename = optarg;
                break;
            case 't':
                testNumber = std::atoi(optarg);
                break;
            case 'h':
                printHelp();
                return 0;
            default:
                printHelp();
                return -1;
        }
    }

    if (testNumber != 0) {
        printf("Переход в режим тестирования.\n");
        switch (testNumber)
        {
        case 1:
            perfomanceTest(lwegContext1, kyberRing);
            perfomanceTest(lwegContext2, kyberRing);
            perfomanceTest(lwegContext3, kyberRing);
            return 0;
        
        default:
            printf("Ошибка: неизвестный тест\n");
            return -1;
        }
    }

    if (optind >= argc) {
        printf("Ошибка: не указан обязательный аргумент outputSize.\n");
        printHelp();
        return -1;
    }

    char* endptr;
    long outputSize = std::strtol(argv[optind], &endptr, 10);
    if (*endptr != '\0' || outputSize <= 0) {
        printf("Ошибка: outputSize должно быть положительным целым числом.\n");
        return -1;
    }

    // Проверяем, нет ли лишних аргументов после обязательного
    if (optind + 1 < argc) {
        printf("Предупреждение: игнорируются лишние аргументы.\n");
    }

    std::vector<uint8_t> d(32);

    if (!seedProvided) {
        std::random_device rd;
        std::mt19937 gen(rd());
        for (int i = 0; i < 32; i++) {
            d[i] = gen();
        }
    } else {
        memcpy(d.data(), &seed, sizeof(long));
    }

    LWEG lweg(kyberRing, lwegContext1);
    lweg.initialize(d);

    std::vector<uint8_t> buffer(outputSize);

    lweg.prng(buffer);

    std::ofstream file(outputFilename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.close();
        printf("Файл сохранён, размер: %d байт\n", (int)buffer.size());
    } else {
        printf("Ошибка открытия файла\n");
    }

    return 0;
}

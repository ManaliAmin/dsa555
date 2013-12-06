#define TEST_RECURSION 0    // 1 tests recursion component, 0 does not
#define TEST_LIST 1         // 1 tests linked list component, 0 does not

#include <cstdint>
#include <array>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include "tester.hpp"

#if TEST_RECURSION
#include "recurse.hpp"

namespace Testing {
    struct SumTest {
        uint32_t lowerBound;
        uint32_t upperBound;
        int row;
        int col;
        uint32_t result;

        SumTest(uint32_t lb = 0, uint32_t ub = 0, int r = 0, int c = 0, uint32_t res = 0) :
            lowerBound(lb), upperBound(ub), row(r), col(c), result(res)
        {}
    };

    struct CountTest {
        uint32_t lowerBound;
        uint32_t upperBound;
        uint32_t result;

        CountTest(uint32_t lb = 0, uint32_t ub = 0, uint32_t res = 0) :
            lowerBound(lb), upperBound(ub), result(res)
        {}
    };

    struct RecurseTestPackage {
        const uint32_t* data;
        int dataLen;
        int width;

        std::vector<SumTest> sumTests;
        std::vector<CountTest> countTests;

        template <size_t SZ_DTR, size_t SZ_DTC, size_t SZ_ST, size_t SZ_CT>
        RecurseTestPackage(
            const uint32_t (&dataArray)[SZ_DTR][SZ_DTC],
            const SumTest (&sumTests)[SZ_ST],
            const CountTest (&countTests)[SZ_CT]
        ) : data((const uint32_t*)dataArray), dataLen(SZ_DTR * SZ_DTC), width(SZ_DTC),
            sumTests(&sumTests[0], &sumTests[0] + SZ_ST),
            countTests(&countTests[0], &countTests[0] + SZ_CT)
        {}
    };

    static uint32_t recurseData0[][40] = {
        //     0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9
        /*0*/{ 0, 0, 0, 5, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
        /*1*/{ 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4 },
        /*2*/{ 5, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, 4, 5, 4, 5, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0 },
        /*3*/{ 2, 0, 2, 7, 9, 0, 6, 0, 5, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4, 5, 3, 5, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 },
        /*4*/{ 0, 0, 0, 0, 2, 0, 3, 0, 8, 7, 0, 1, 6, 0, 0, 8, 0, 0, 0, 0, 0, 5, 6, 7, 6, 0, 0, 0, 3, 8, 0, 0, 3, 0, 0, 7, 5, 2, 0, 0 },
        /*5*/{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 6, 2, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 7, 1, 4 },
        /*6*/{ 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4 },
        /*7*/{ 2, 0, 2, 7, 9, 0, 6, 0, 5, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4, 5, 3, 5, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 }


        // Count Test 0: A B C D E F G H I J K L M N O P Q R S = 19
        // { 0, 0, 0, A, A, 0, 0, A, A, 0, A, 0, 0, 0, 0, 0, 0, I, 0, 0, 0, 0, K, K, 9, 0, L, 0, L, 0, 0, 0, 0, M, 0, 0, 0, 0, N, 0 },
        // { 0, 0, A, A, A, A, A, A, A, 0, A, A, 0, 0, 0, 0, I, I, I, I, 0, 0, 0, K, 9, L, L, L, L, 0, M, M, 0, M, M, 0, 0, N, N, N },
        // { A, A, A, A, A, 0, A, 0, A, A, A, A, A, 0, 0, H, 0, I, I, I, I, I, 0, 0, 9, 0, L, L, 0, 0, 9, M, M, M, 9, 0, P, 0, N, 0 },
        // { A, 0, A, A, 9, 0, A, 0, A, 0, 0, 0, 0, 0, F, 9, 0, 0, 0, I, I, I, I, I, 0, 0, L, 0, S, 0, 9, M, M, 0, P, P, P, 0, 9, O },
        // { 0, 0, 0, 0, B, 0, A, 0, A, A, 0, E, E, 0, 0, G, 0, 0, 0, 0, 0, I, I, I, I, 0, 0, 0, S, S, 0, 0, M, 0, 0, P, P, P, 0, 0 },
        // { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, E, E, E, E, 9, 0, 0, 0, 0, 0, 0, 0, I, 0, 0, 0, S, S, S, 0, 0, 0, 0, 0, 0, 0, P, P, P },
        // { 0, 0, D, D, D, D, D, D, D, 0, E, E, 0, 0, 0, 0, I, I, I, I, 0, 0, 0, I, 9, S, S, S, S, 0, R, R, 0, Q, Q, 0, 0, P, P, P },
        // { C, 0, D, D, 9, 0, D, 0, D, 0, 0, 0, 0, 0, J, 9, 0, 0, 0, I, I, I, I, I, 0, 0, S, 0, S, 0, 9, R, R, 0, Q, Q, Q, 0, 9, P }

        // Count Test 1: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m = 39
        // { 0, 0, 0, C, 2, 0, 0, 2, J, 0, L, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, W, 3, 9, 0, 2, 0, Y, 0, 0, 0, 0, 1, 0, 0, 0, 0, m, 0 },
        // { 0, 0, B, 3, D, 2, 1, 1, J, 0, 2, M, 0, 0, 0, 0, 3, 2, 3, T, 0, 0, 0, X, 9, 1, 2, Y, Y, 0, c, c, 0, 3, e, 0, 0, 3, 1, k },
        // { A, A, 1, D, D, 0, 3, 0, J, 3, N, 3, O, 0, 0, 1, 0, 3, T, T, T, T, 0, 0, 9, 0, 2, Y, 0, 0, 9, 2, 1, d, 9, 0, 1, 0, l, 0 },
        // { 2, 0, 2, D, 9, 0, I, 0, J, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, T, T, 3, V, V, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, f, f, 2, 0, 9, j },
        // { 0, 0, 0, 0, 2, 0, 3, 0, J, J, 0, 1, P, 0, 0, S, 0, 0, 0, 0, 0, V, V, V, V, 0, 0, 0, 3, a, 0, 0, 3, 0, 0, f, f, 2, 0, 0 },
        // { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, R, 2, 9, 0, 0, 0, 0, 0, 0, 0, V, 0, 0, 0, 1, 2, a, 0, 0, 0, 0, 0, 0, 0, h, 1, i },
        // { 0, 0, E, 3, G, 2, 1, 1, K, 0, 2, Q, 0, 0, 0, 0, 3, 2, 3, U, 0, 0, 0, V, 9, 1, 2, Z, Z, 0, b, b, 0, 3, g, 0, 0, 3, 1, i },
        // { 2, 0, 2, F, 9, 0, H, 0, K, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, U, U, 3, V, V, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, g, g, 2, 0, 9, i }

        // Count Test 2: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h = 34
        // { 0, 0, 0, F, F, 0, 0, G, G, 0, 7, 0, 0, 0, 0, 0, 0, U, 0, 0, 0, 0, 8, W, 9, 0, X, 0, 7, 0, 0, 0, 0, c, 0, 0, 0, 0, 6, 0 },
        // { 0, 0, 6, F, 6, G, G, G, G, 0, M, 6, 0, 0, 0, 0, U, U, U, U, 0, 0, 0, 8, 9, X, X, X, X, 0, 6, Z, 0, c, 8, 0, 0, e, e, e },
        // { A, 6, C, 6, 7, 0, G, 0, 8, L, 6, N, 8, 0, 0, T, 0, U, U, U, U, U, 0, 0, 9, 0, X, 6, 0, 0, 9, Z, Z, 8, 9, 0, d, 0, 8, 0 },
        // { A, 0, C, 7, 9, 0, 6, 0, K, 0, 0, 0, 0, 0, Q, 9, 0, 0, 0, U, U, U, U, 6, 0, 0, X, 0, Y, 0, 9, Z, Z, 0, 6, 7, d, 0, 9, f },
        // { 0, 0, 0, 0, H, 0, I, 0, 8, 7, 0, P, 6, 0, 0, 8, 0, 0, 0, 0, 0, U, 6, 7, 6, 0, 0, 0, Y, 8, 0, 0, Z, 0, 0, 7, d, d, 0, 0 },
        // { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P, P, 6, R, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, Y, Y, 8, 0, 0, 0, 0, 0, 0, 0, 7, g, g },
        // { 0, 0, 6, E, 6, J, J, J, J, 0, O, 6, 0, 0, 0, 0, V, V, V, V, 0, 0, 0, 8, 9, Y, Y, Y, Y, 0, 6, a, 0, b, 8, 0, 0, g, g, g },
        // { B, 0, D, 7, 9, 0, 6, 0, J, 0, 0, 0, 0, 0, S, 9, 0, 0, 0, V, V, V, V, 6, 0, 0, Y, 0, Y, 0, 9, a, a, 0, 6, 7, h, 0, 9, g }

        // Count Test 3: A B C D E F G H I J K L M N O P Q R = 18
        // { 0, 0, 0, B, 2, 0, 0, 2, C, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
        // { 0, 0, 6, 3, 6, 2, 1, 1, C, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, F, 0, 0, 0, 8, 9, 1, 2, K, K, 0, 6, M, 0, 3, 8, 0, 0, 3, 1, P },
        // { A, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, F, F, F, F, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0 },
        // { 2, 0, 2, 7, 9, 0, 6, 0, D, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, F, F, 3, H, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, Q },
        // { 0, 0, 0, 0, 2, 0, 3, 0, 8, 7, 0, 1, 6, 0, 0, 8, 0, 0, 0, 0, 0, G, 6, 7, 6, 0, 0, 0, 3, 8, 0, 0, 3, 0, 0, 7, O, 2, 0, 0 },
        // { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 6, 2, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 7, 1, R },
        // { 0, 0, 6, 3, 6, 2, 1, 1, E, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, I, 0, 0, 0, 8, 9, 1, 2, L, L, 0, 6, N, 0, 3, 8, 0, 0, 3, 1, R },
        // { 2, 0, 2, 7, 9, 0, 6, 0, E, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, I, I, 3, J, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, R }

        // Count Test 4: A B C D E F G H I J K L = 12
        // { 0, 0, 0, B, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
        // { 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4 },
        // { A, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, 4, E, 4, H, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0 },
        // { 2, 0, 2, 7, 9, 0, 6, 0, C, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4, F, 3, I, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 },
        // { 0, 0, 0, 0, 2, 0, 3, 0, 8, 7, 0, 1, 6, 0, 0, 8, 0, 0, 0, 0, 0, G, 6, 7, 6, 0, 0, 0, 3, 8, 0, 0, 3, 0, 0, 7, L, 2, 0, 0 },
        // { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 6, 2, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 7, 1, 4 },
        // { 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4 },
        // { 2, 0, 2, 7, 9, 0, 6, 0, D, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4, J, 3, K, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 }
    };
    static SumTest recurseSumTests0[] = {
        SumTest(3, 7, 3, 20, 31),
        SumTest(0, 10, 6, 2, 52),
        SumTest(3, 9, 6, 18, 0),
        SumTest(2, 9, 6, 18, 119),
        SumTest(1, 10, 2, 24, 46),
    };
    static CountTest recurseCountTests0[] = {
        CountTest(0, 9, 19),
        CountTest(3, 9, 39),
        CountTest(0, 6, 34),
        CountTest(3, 6, 18),
        CountTest(4, 6, 12),
    };
    static RecurseTestPackage recurseTestPackage0(recurseData0, recurseSumTests0, recurseCountTests0);

    static uint32_t recurseData1[][20] = {
        //     0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9
        /*0*/{ 0, 0, 0, 5, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0 },
        /*1*/{ 5, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, 4, 5 },
        /*2*/{ 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4 },
        /*3*/{ 5, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, 4, 5 },
        /*4*/{ 2, 0, 2, 7, 9, 0, 6, 0, 5, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4 },
        /*5*/{ 0, 0, 0, 0, 2, 0, 3, 0, 8, 7, 0, 1, 6, 0, 0, 8, 0, 0, 0, 0 },
        /*6*/{ 0, 0, 0, 5, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0 },
        /*7*/{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 6, 2, 9, 0, 0, 0, 0 },
        /*8*/{ 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
        /*9*/{ 4, 5, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0 },
        /*0*/{ 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4 },
        /*1*/{ 4, 5, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0 },
        /*2*/{ 5, 3, 5, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 },
        /*3*/{ 0, 5, 6, 7, 6, 0, 0, 0, 3, 8, 0, 0, 3, 0, 0, 7, 5, 2, 0, 0 },
        /*4*/{ 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
        /*5*/{ 0, 0, 0, 6, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 7, 1, 4 },
    };
    static SumTest recurseSumTests1[] = {
        SumTest(2, 9, 3, 9, 101),
        SumTest(0, 2, 11, 12, 1),
        SumTest(3, 9, 9, 18, 14),
        SumTest(0, 5, 7, 11, 3),
        SumTest(1, 9, 6, 8, 107),
    };
    static CountTest recurseCountTests1[] = {
        CountTest(0, 9, 25),
        CountTest(5, 8, 23),
        CountTest(0, 2, 18),
        CountTest(3, 7, 32),
        CountTest(4, 7, 29),
    };
    static RecurseTestPackage recurseTestPackage1(recurseData1, recurseSumTests1, recurseCountTests1);

    static uint32_t recurseData2[][80] = {
        //     0                             10                            20                            30                            40                            50                            60                            70
        //     0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9
        /*0*/{ 0, 0, 0, 5, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0, 2, 0, 2, 7, 9, 0, 6, 0, 5, 0, 0, 0, 0, 0, 2, 9, 0, 0, 0, 4, 5, 3, 5, 6, 0, 0, 1, 0, 3, 0, 9, 2, 2, 0, 6, 7, 2, 0, 9, 4 },
        /*1*/{ 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4, 0, 0, 0, 0, 2, 0, 3, 0, 8, 7, 0, 1, 6, 0, 0, 8, 0, 0, 0, 0, 0, 5, 6, 7, 6, 0, 0, 0, 3, 8, 0, 0, 3, 0, 0, 7, 5, 2, 0, 0 },
        /*2*/{ 5, 6, 1, 6, 7, 0, 3, 0, 8, 3, 6, 3, 8, 0, 0, 1, 0, 3, 4, 5, 4, 5, 0, 0, 9, 0, 2, 6, 0, 0, 9, 2, 1, 8, 9, 0, 1, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 6, 2, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 7, 1, 4 },
        /*3*/{ 0, 0, 6, 3, 6, 2, 1, 1, 4, 0, 2, 6, 0, 0, 0, 0, 3, 2, 3, 4, 0, 0, 0, 8, 9, 1, 2, 4, 4, 0, 6, 4, 0, 3, 8, 0, 0, 3, 1, 4, 0, 0, 0, 5, 2, 0, 0, 2, 4, 0, 7, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 8, 3, 9, 0, 2, 0, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
    };
    static SumTest recurseSumTests2[] = {
        SumTest(4, 9, 1, 49, 20),
        SumTest(0, 6, 2, 63, 0),
        SumTest(3, 9, 1, 39, 4),
        SumTest(1, 9, 2, 53, 8),
        SumTest(0, 5, 3, 28, 26),
    };
    static CountTest recurseCountTests2[] = {
        CountTest(0, 9, 29),
        CountTest(4, 9, 41),
        CountTest(1, 4, 36),
        CountTest(2, 6, 35),
        CountTest(3, 9, 44),
    };
    static RecurseTestPackage recurseTestPackage2(recurseData2, recurseSumTests2, recurseCountTests2);
}
#endif
#if TEST_LIST
#include "list.h"
#endif


namespace Testing {
    template <typename ITERATOR>
    void bubbleSort(ITERATOR begin, size_t numElems){
        for (size_t i = 0; i < numElems - 1; ++i){
            ITERATOR b = begin;
            bool didStuff = false;
            for (size_t j = 0; j < numElems - 1; ++j){
                ITERATOR a = b++;

                if (*b < *a) {
                    didStuff = true;
                    auto temp = *a;
                    *a = *b;
                    *b = temp;
                }

            }
            if (!didStuff) {
                break;
            }
        }
    }

    template <typename ITERATOR>
    void llMergeSections(
        ITERATOR a, ITERATOR b, ITERATOR buff, size_t an, size_t bn){
        auto dest = a;
        auto temp = buff;

        size_t ai = 0, bi = 0;
        while (ai < an && bi < bn) {
            const auto& av = *a;
            const auto& bv = *b;
            if (av < bv) {
                *(buff++) = av;
                a++;
                ai++;
            }
            else {
                *(buff++) = bv;
                b++;
                bi++;
            }
        }

        for (; ai < an; *(buff++) = *(a++), ai++);
        for (; bi < bn; *(buff++) = *(b++), bi++);
        for (size_t i = 0, end = an + bn; i < end; ++i) {
            *(dest++) = *(temp++);
        }
    }

    template <typename ITERATOR>
    void llMergeSort(ITERATOR begin, size_t numElems, ITERATOR buff){
        if (numElems <= 1) {
            return;
        }
        else if (numElems == 2) {
            auto b = begin;
            auto a = b++;
            auto& av = *a;
            auto& bv = *b;
            if (bv < av) {
                auto temp = av;
                av = bv;
                bv = temp;
            }
            return;
        }
        else {
            auto beginStart = begin;
            auto buffStart = buff;
            size_t half = numElems / 2;
            llMergeSort(begin, half, buff);

            for (size_t i = 0; i < half; ++i, ++begin, ++buff);
            llMergeSort(begin, numElems - half, buff);

            llMergeSections(beginStart, begin, buffStart, half, numElems - half);
        }
    }
}

#if TEST_RECURSION
bool testRecursion(){
    using namespace Testing;

    msg("Entering recursion tests");

    std::array<RecurseTestPackage, 3> tests = {
        recurseTestPackage0,
        recurseTestPackage1,
        recurseTestPackage2
    };

    for (auto i = tests.begin(), end = tests.end(); i != end; ++i) {
        ss << "Test " << i - tests.begin() << ": "
            << i->dataLen / i->width << " rows, "
            << i->width << " columns";
        h2w();

        msg("Constructing solver...");
        RecursiveSolver solver(i->data, i->dataLen, i->width);

        ss << "Performing " << i->countTests.size() << " count tests";
        h3w();
        for (auto j = i->countTests.begin(), jend = i->countTests.end(); j != jend; ++j) {
            auto solverAnswer = solver.count(j->lowerBound, j->upperBound);
            ASSERT_EQ(solverAnswer, j->result,
                "count(lb:" << j->lowerBound << " ub:" << j->upperBound << ") == " << j->result);
        }

        ss << "Performing " << i->sumTests.size() << " sum tests";
        h3w();
        for (auto j = i->sumTests.begin(), jend = i->sumTests.end(); j != jend; ++j) {
            auto solverAnswer = solver.sum(j->lowerBound, j->upperBound, j->row, j->col);
            ASSERT_EQ(solverAnswer, j->result,
                "sum(lb:" << j->lowerBound << ", ub:" << j->upperBound << ", r:" << j->row << ", c:" << j->col << ") == " << j->result);
        }
    }

    h2("Recursion tests complete");

    return true;
}
#endif

#if TEST_LIST
static bool testLinkedList(){
    using namespace Testing;

    h2("Entering linked list basic tests");
    LinkedList<int> l1;
    ASSERT(l1.size() == 0, "array has size 0 at construction time");
    ASSERT(l1.empty(), "empty() is true after construction");
    ASSERT(l1.begin() == l1.end(), "begin == end after construction");
    l1.push_back(1);
    ASSERT(!l1.empty(), "empty() is false after a push back");
    ASSERT(l1.begin() != l1.end(), "begin != end after a push back");
    l1.push_back(50);
    ASSERT(l1.size() == 2, "array has size 2 after 2 push backs");
    ASSERT(l1.front() == 1, "front element has value 1");
    ASSERT(l1.back() == 50, "back element has value 50");
    l1.push_front(-3);
    l1.push_front(6);
    ASSERT(l1.size() == 4, "array has size 4 after 2 more push backs");
    ASSERT(l1.front() == 6, "first element is now 6");
    ASSERT(l1.back() == 50, "last element still 50");
    ASSERT(*++l1.begin() == -3, "element *++begin() is now -3, accessed through iterator");
    l1.erase(++l1.begin());
    ASSERT(l1.size() == 3, "size is 3 after erasing second element");
    ASSERT(*++l1.begin() == 1, "second element is now 1");
    l1.erase(----l1.end());
    ASSERT(l1.size() == 2, "size is 2 after erasing second element");
    ASSERT(*++l1.begin() == 50, "second element is now 50");
    l1.push_front(9);
    l1.push_back(10);
    ASSERT(l1.size() == 4, "size is 4 after push front, push back");
    ASSERT(l1.front() == 9, "first element is now 9");
    ASSERT(l1.back() == 10, "last element is now 10");
    l1.erase(l1.begin());
    ASSERT(l1.size() == 3, "size is 3 after erase first element");
    ASSERT(l1.front() == 6, "first element is now 6");
    l1.erase(--l1.end());
    ASSERT(l1.size() == 2, "size is 2 after erase last element");
    ASSERT(l1.back() == 50, "last element is now 50");
    Iterator<int> i = l1.begin();
    ASSERT(*i == 6, "deref iterator after assignment from begin iterator is 6");
    l1.insert(i, 7);
    ASSERT(l1.size() == 3, "size is 3 after insert at iterator pointing at begin");
    ASSERT(l1.front() == 7, "first element is now 7");
    ASSERT(*i == 6, "deref iterator still gives 6");
    i = l1.end();
    l1.insert(i, 8);
    ASSERT(l1.size() == 4, "size is 4 after insert at iterator pointing at end");
    ASSERT(l1.back() == 8, "last element is now 8");
    ASSERT(*--i == 8, "deref element through iterator returns 8");
    Iterator<int> j = i;
    ASSERT(*j++ == 8, "deref through copied iterator postfix incremented returns 8");
    ASSERT(j-- == l1.end(), "equality test iterator postfix decremented returns list end");
    ASSERT(j != l1.end(), "inequality test iterator shouldn't equal end anymore");
    ASSERT(!(++j != l1.end()), "not-inequality test iterator should equal end after prefix increment");

    l1.insert(i, 11);
    ASSERT(l1.size() == 5, "size is 5 after insert at iterator pointing at element before end");
    ASSERT(l1.back() == 8, "last element still 8");
    ASSERT(*i == 8, "deref element through iterator still 8");
    ASSERT(*--i == 11, "deref element before last through iterator is 11");
    ASSERT(!l1.empty(), "ensure empty still false");
    l1.clear();
    ASSERT(l1.empty(), "empty after clear");
    ASSERT(l1.size() == 0, "0 size after clear");
    ASSERT(l1.begin() == l1.end(), "begin == end after clear");

    h2("String tests");
    LinkedList<std::string> llstr;
    llstr.push_back("hello");
    llstr.push_back("there");
    llstr.push_back("boy");
    Iterator<std::string> k = llstr.begin();
    ASSERT(k->size() == 5, "iterator call through arrow operator first elem");
    ASSERT((++k)->size() == 5, "iterator call through arrow operator second elem");
    ASSERT((++k)->size() == 3, "iterator call through arrow operator third elem");
    ASSERT(*k-- == "boy", "iterator deref comparison third elem");
    ASSERT(*k-- == "there", "iterator deref comparison second elem");
    ASSERT(*k == "hello", "iterator deref comparison first elem");

    h2("Linked list basic tests complete");

    // Make list of ints then sort
    h2("Linked list sort tests");
#define NUM_INTS 10
    int ints[NUM_INTS] = {
        10, 2, 6, -5, 15, 11, 22, 3, 8, 2
    };

    // Construct list of ints then sort baseline array
    msg("Pushing values into list...");
    LinkedList<int> testList, buffer, toMergeSortList;
    for (int i = 0; i < NUM_INTS; ++i){
        testList.push_back(ints[i]);
    }
    msg("Copying lists via assignment operator...");
    toMergeSortList = testList;
    buffer = testList;

    h2("Testing contents of lists after assignment");
    {
        auto tm = toMergeSortList.begin();
        auto tl = testList.begin();
        auto b = buffer.begin();
        for (int i = 0; i < NUM_INTS; ++i) {
            ss << "Elem " << i;
            h3w();
            ASSERT(*tm++ == ints[i], "*(ListA iterator) == " << ints[i]);
            ASSERT(*tl++ == ints[i], "*(ListB iterator) == " << ints[i]);
            ASSERT(*b++ == ints[i], "*(ListC iterator) == " << ints[i]);
        }
    }

    std::vector<int> sortedInts(ints, ints + NUM_INTS);

    // Bubble sort both lists
    bubbleSort(sortedInts.begin(), sortedInts.size());

    msg("Bubble sorting linked list via iterators...");
    bubbleSort(testList.begin(), testList.size());
    h2("Testing Bubble Sort Through Iterators");
    {
        Iterator<int> begin = testList.begin(), end = testList.end();
        auto vb = sortedInts.begin();
        for (int i = 0; begin != end; ++begin, ++vb, ++i){
            ASSERT(*vb == *begin, "element " << i << ", expected " << *vb << " got " << *begin);
        }
    }
    h2("Bubble sort tests complete");

    msg("Merge sorting linked list via iterators...");
    llMergeSort(toMergeSortList.begin(), toMergeSortList.size(), buffer.begin());
    h2("Merge Sort Through Iterators");
    {
        Iterator<int> begin = toMergeSortList.begin(), end = toMergeSortList.end();
        auto vb = sortedInts.begin();
        for (int i = 0; begin != end; ++begin, ++vb, ++i){
            ASSERT(*vb == *begin, "element " << i << ", expected " << *vb << " got " << *begin);
        }
    }
    h2("Merge sort tests complete");

    return true;
}
#endif

int main(){
    using namespace Testing;
    bool overallSuccess = true;
#if TEST_RECURSION
    success = true;
    h1("Recursion Tests");

    testRecursion();

    if (success) {
        h1("Finished recursion tests with no errors! Success!");
    }
    else {
        h1("Finished recursion tests with errors... fix them and try again");
        overallSuccess = false;
    }
#endif

#if TEST_RECURSION && TEST_LIST
    std::cout << std::endl;
#endif

#if TEST_LIST
    success = true;
    h1("Linked list Tests");

    testLinkedList();

    if (success) {
        h1("Finished linked list tests with no errors! Success!");
    }
    else {
        h1("Finished linked list tests with errors... fix them and try again");
        overallSuccess = false;
    }
#endif

    if (overallSuccess) {
        h1("Finished all enabled tests with no errors! Success!");
    }
    else {
        h1("Finished some enabled tests with errors... fix them and try again");
    }

    std::cout << "Done. Type anything to continue..." << std::endl;
    std::string s;
    std::cin >> s;

    return 0;
}

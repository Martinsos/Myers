//
// This performance test is the analogous of performanceTest_Generic
// for the original char-based edlib
//
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <iostream>
#include <fstream>

#include "edlib.h"
#include "SimpleEditDistance_Char.h"

using namespace std;

void runRandomTests(int numTests, EdlibAlignMode mode, bool findAlignment,
                    int alphabetLength, int queryLength, int targetLength,
                    ofstream& file);

int main(int argc, char* argv[]) {
    // This program has optional first parameter, which is number of random tests to run
    // per each algorithm.
    int numRandomTests = 10;
    if (argc > 1) {
        numRandomTests = static_cast<int>(strtol(argv[1], NULL, 10));
    }

    std::ofstream ofs;
    ofs.open ("performanceChar_alphabet.txt", std::ofstream::out);
    ofs << "modeStr" << "\t" << "alphaLen" << "\t" << "queryLen" << "\t";
    ofs << "targetLen" << "\t" << "findAlignment" << "\t" << "mTime" << "\n";

    srand(42);
    for(int queryLength = 100; queryLength <= 1000; queryLength *= 10) {
        int targetLength = queryLength;
        for (int alphabetLength = 2; alphabetLength < 128; alphabetLength += 10) {

            printf("query and target length = %d \n", queryLength);
            printf("alphabet length = %d \n", alphabetLength);
            printf("######################### \n");

            printf("Testing HW with alignment...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_HW, true,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");

            printf("Testing HW...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_HW, false,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");

            printf("Testing NW with alignment...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_NW, true,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");

            printf("Testing NW...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_NW, false,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");

            printf("Testing SHW with alignment...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_SHW, true,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");

            printf("Testing SHW...\n");
            runRandomTests(numRandomTests, EDLIB_MODE_SHW, false,
                           alphabetLength, queryLength, targetLength, ofs);
            printf("\n");
        }
    }
    ofs.close();
    return 0;
}

void fillRandomly(char* seq, int seqLength, int alphabetLength) {
    for (int i = 0; i < seqLength; i++)
        seq[i] = static_cast<char>(rand() % alphabetLength);
}

// Returns true if all tests passed, false otherwise.
void runRandomTests(int numTests, EdlibAlignMode mode, bool findAlignment,
                    int alphabetLength, int queryLength, int targetLength,
                    ofstream& file){
    clock_t start;
    double timeEdlib = 0;

    for (int i = 0; i < numTests; i++) {
        char *query = static_cast<char *>(malloc(sizeof(char) * queryLength));
        char *target = static_cast<char *>(malloc(sizeof(char) * targetLength));
        fillRandomly(query, queryLength, alphabetLength);
        fillRandomly(target, targetLength, alphabetLength);

        start = clock();
        EdlibAlignResult result = edlibAlign(
                query, queryLength, target, targetLength,
                edlibNewAlignConfig(-1, mode, findAlignment ? EDLIB_TASK_PATH : EDLIB_TASK_DISTANCE, NULL, 0));
        timeEdlib += clock() - start;
        edlibFreeAlignResult(result);
    }
    string modeString = mode == EDLIB_MODE_HW ? "HW" : mode == EDLIB_MODE_SHW ? "SHW" : "NW";
    double mTime = static_cast<double>(timeEdlib)/CLOCKS_PER_SEC;
    printf("Time Edlib: %lf\n", mTime);

    //writing on output file for visualization
    file << modeString << "\t" << alphabetLength << "\t" << queryLength << "\t";
    file << targetLength << "\t" << findAlignment << "\t" << mTime << "\n";

}


#include "kernel.cuh"

#define SEED_RANGE_MAX 10000

__device__ int64_t traverseMap(Mapping **mapsCuds, int *numMaps, int64_t s, int map){

    int nMaps = *(numMaps + map);

    for(int i = 0; i < nMaps; i++){
        Mapping m = *(*(mapsCuds + map) + i);
        if (s >= m.from && s < m.from + m.range) { return m.to + (s - m.from); }
    }

    return s;
}

__global__ void run(int numSeeds, int64_t *seeds, Mapping **mapsCuda, int *numMaps, int64_t *mins){
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    int64_t min = INT64_MAX;
    for(int i = index; i < numSeeds; i+=stride){
        int64_t startSeed = *(seeds + 2 * i);
        int64_t range = *(seeds + 2 * i + 1);
        for(int64_t j = 0; j < range; j++){
            int64_t outSeed = traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, traverseMap(mapsCuda, numMaps, startSeed + j, 0), 1), 2), 3), 4), 5), 6);
            
            if(outSeed < min){ min = outSeed; }

        }
        *(mins + i) = min;
    }
}

int64_t pt2(std::vector<int64_t>& seedsIn, const std::vector<std::vector<Mapping>>& maps){
    
    std::vector<int64_t> seeds;
    // Split seed ranges up so less than SEED_RANGE_MAX
    for(int i = 0; i < seedsIn.size() / 2; i++){
        while(seedsIn[2 * i + 1] > SEED_RANGE_MAX){
            seeds.push_back(seedsIn[2 * i]);
            seeds.push_back(SEED_RANGE_MAX);
            seedsIn[2 * i] += SEED_RANGE_MAX;
            seedsIn[2 * i + 1] -= SEED_RANGE_MAX;
        }
        seeds.push_back(seedsIn[2 * i]);
        seeds.push_back(seedsIn[2 * i + 1]);
    }
    
    int N = seeds.size() / 2;
    int64_t *seedsCUDA;
    cudaMallocManaged(&seedsCUDA, 2 * N * sizeof(int64_t));
    for(int i = 0; i < seeds.size(); i++){
        seedsCUDA[i] = seeds[i];
    }

    Mapping **mapsCUDA;
    cudaMallocManaged(&mapsCUDA, maps.size() * sizeof(Mapping *));
    int *numMaps;
    cudaMallocManaged(&numMaps, 7 * sizeof(int));
    for(int i = 1; i < 8; i++){
        cudaMallocManaged(&mapsCUDA[i - 1], maps[i].size() * sizeof(Mapping));
        numMaps[i - 1] = maps[i].size();
    
        for(int j = 0; j < maps[i].size(); j++){
            mapsCUDA[i - 1][j] = maps[i][j];
        }
    }

    int64_t *mins;
    cudaMallocManaged(&mins, N * sizeof(int64_t));

    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;
    run<<<numBlocks, blockSize>>>(N, seedsCUDA, mapsCUDA, numMaps, mins);
    cudaDeviceSynchronize();

    int64_t minLoc = INT64_MAX;
    for(int i = 0; i < N; i++){
        if(*(mins + i) < minLoc){ minLoc = *(mins + i); }
    }

    cudaFree(seedsCUDA);
    for(int i = 0; i < 7; i++){
        cudaFree(mapsCUDA[i]);
    }
    cudaFree(mapsCUDA);
    cudaFree(numMaps);
    cudaFree(mins);

    return minLoc;
}

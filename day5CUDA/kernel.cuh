#ifndef KERNEL_H
#define KERNEL_h

#include <iostream>
#include <vector>

#include <cuda_runtime.h>

#include "mapping.h"

int64_t pt2(std::vector<int64_t>& seeds, const std::vector<std::vector<Mapping>>& maps);


#endif
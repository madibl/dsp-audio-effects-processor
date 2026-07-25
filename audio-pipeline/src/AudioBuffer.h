#pragma once

#include <vector>
#include <cstdint>

struct AudioChunk {
    std::vector<float> samples;
    bool is_last = false; // flag to mark the final chunk so we know when we stop
};
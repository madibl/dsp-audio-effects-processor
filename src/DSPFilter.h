#pragma once

#include <vector>
#include "AudioBuffer.h"

class FIRLowPass {
public:
    /**
     * a simple 5-tap moving average-style filter
     */
    FIRLowPass() : coefficients_({0.2f, 0.2f, 0.2f, 0.2f, 0.2f}) {
        history_.resize(coefficients_.size(), 0.0f);
    }

    /**
     * Filters one chunk. Maintains history across calls so the 
     * filter is continuous across chunk boundaries
     */
    AudioChunk apply(const AudioChunk& input) {
        AudioChunk output;
        output.samples.resize(input.samples.size());
        output.is_last = input.is_last;

        for (size_t i = 0; i < input.samples.size(); ++i) {
            // for each input, shift history and insert new sample
            // TODO: maybe replace this with a more efficient history filler - circular buffer?
            for (size_t j = coefficients_.size() - 1; j > 0; --j) {
                history_[j] = history_[j-1];
            }
            history_[0] = input.samples[i];

            float sum  = 0.0f;
            for (size_t k = 0; k < coefficients_.size(); ++k) {
                sum += coefficients_[k] * history_[k];
            }
            output.samples[i] = sum;
        }
        
        return output;
    }


private:
    std::vector<float> coefficients_;
    std::vector<float> history_;
};
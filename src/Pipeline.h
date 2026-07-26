#pragma once

#include <thread>
#include "CircularBuffer.h"
#include "WavReader.h"
#include "WavWriter.h"
#include "DSPFilter.h"

class Pipeline {
public:
    Pipeline(const std::string& inputPath, const std::string& outputPath,
              size_t chunkSize = 4096, size_t queueCapacity = 8)
        : reader_(inputPath),
          writer_(outputPath, reader_.sampleRate(), reader_.channels()),
          chunkSize_(chunkSize),
          readQueue_(queueCapacity),
          writeQueue_(queueCapacity) {}

    void run() {
        std::thread readerThread(&Pipeline::readLoop, this);
        std::thread writerThread(&Pipeline::writeLoop, this);
        std::thread filterThread(&Pipeline::filterLoop, this);

        readerThread.join();
        writerThread.join();
        filterThread.join();
    }

private:
    void readLoop() {
        bool done = false;
        while (!done) {
            AudioChunk chunk = reader_.readChunk(chunkSize_);
            done = chunk.is_last;
            readQueue_.push(std::move(chunk));
        }
        readQueue_.close();
    }
    
    void writeLoop() {
        AudioChunk chunk;
        while (writeQueue_.pop(chunk)) {
            writer_.writeChunk(chunk);
        }
    } 

    void filterLoop() {
        AudioChunk chunk;
        while (readQueue_.pop(chunk)) {
            AudioChunk filtered = filter_.apply(chunk);
            bool wasLast = filtered.is_last;
            writeQueue_.push(std::move(filtered));
            if (wasLast) break;
        }
        writeQueue_.close();
    }

    WavReader reader_;
    WavWriter writer_;
    FIRLowPass filter_;
    size_t chunkSize_;
    CircularBuffer<AudioChunk> readQueue_;
    CircularBuffer<AudioChunk> writeQueue_;
};
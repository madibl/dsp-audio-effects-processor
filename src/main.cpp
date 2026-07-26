#include <iostream>
#include "Pipeline.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.wav> <output.wav>\n";
        return 1;
    }

    try {
        Pipeline pipeline(argv[1], argv[2]);
        pipeline.run();
        std::cout << "Done. Wrote to " << argv[2] << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
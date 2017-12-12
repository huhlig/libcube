#include "cube.hpp"
#include <vector>

const uint8_t PIN_DATA = 26; // Serial/Data
const uint8_t PIN_OE = 19; // Output Enable
const uint8_t PIN_LATCH = 13; // Latch
const uint8_t PIN_CLOCK = 22; // Clock
const uint8_t PIN_MR = 17; // Master Reset


int main(int argc, char **argv) {
    MonochromeCube<4, 4, 4> cube(PIN_DATA, PIN_CLOCK, PIN_LATCH);
    std::vector<MonochromeCube<4, 4, 4>::Frame> frames;
    MonochromeCube<4, 4, 4>::Buffer buffer = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
    };
    frames.push_back(MonochromeCube<4, 4, 4>::Frame(buffer));
    while(true) {
        for(int f = 0; f < frames.size(); f++ ) {
            cube.displayFrame(frames[f]);
        }
    }
}
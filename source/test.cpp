#include <iostream>
#include "cube.hpp"
#include "patterns.hpp"

const uint8_t PIN_DATA = 26; // Serial/Data
const uint8_t PIN_OE = 19; // Output Enable
const uint8_t PIN_LATCH = 13; // Latch
const uint8_t PIN_CLOCK = 22; // Clock
const uint8_t PIN_MR = 17; // Master Reset

const uint8_t SPEED = 50; // Times to repeat each frame

int main(int argc, char **argv) {
    std::cout << "Starting up" << std::endl;
    MonochromeCube<4, 4, 4> cube(PIN_DATA, PIN_CLOCK, PIN_LATCH, PIN_OE, PIN_MR);
    std::vector<MonochromeCube<4, 4, 4>::Frame> *pattern = &pattern_1;

    while (true) {
        for (int f = 0; f < pattern->size(); f++) {
            for (int t = SPEED; t > 0; t--) {
                cube.displayFrame((*pattern)[f]);
            }
            std::cout << "Frame Displayed " << (uint8_t) SPEED << " Times" << std::endl;
        }
    }
}
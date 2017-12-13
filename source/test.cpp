#include <iostream>
#include <cstdlib>
#include <ctime>
#include "cube.hpp"
#include "patterns.hpp"

const uint8_t PIN_DATA = 26; // Serial/Data
const uint8_t PIN_OE = 19; // Output Enable
const uint8_t PIN_LATCH = 13; // Latch
const uint8_t PIN_CLOCK = 22; // Clock
const uint8_t PIN_MR = 17; // Master Reset
const uint32_t SPEED = 15; // Duration of each frame
const uint8_t L0 = 0xFF, L1 = 0xAA, L2 = 0x88, L3 = 0x80;

int main(int argc, char **argv) {
    std::cout << "Starting up" << std::endl;
    MonochromeCube<4, 4, 4> cube(PIN_DATA, PIN_CLOCK, PIN_LATCH, PIN_MR, PIN_OE);

    srand(time(NULL));

    uint8_t x = rand() % 4, y = rand() % 4, z = rand() % 4;

    struct Position {
        uint8_t x, y, z;
    };

    Position trail[4] = {{x, y, z},
                         {x, y, z},
                         {x, y, z},
                         {x, y, z}};

    while (true) {
        MonochromeCube<4, 4, 4>::Frame frame;
        frame.setDuration(SPEED);
        bool success = false;
        uint8_t x = trail[0].x, y = trail[0].y, z = trail[0].z;
        while (!success) {
            switch (rand() % 12) {
                case 0: // +X
                    if (x + 1 < 4) {
                        x = x + 1;
                        success = true;
                    }
                    break;
                case 1: // -X
                    if (x - 1 > 0) {
                        x = x - 1;
                        success = true;
                    }
                    break;
                case 2: // +Y
                    if (y + 1 < 4) {
                        y = y + 1;
                        success = true;
                    }
                    break;
                case 3: // -Y
                    if (y - 1 > 0) {
                        y = y - 1;
                        success = true;
                    }
                    break;
                case 4: // +Z
                    if (z + 1 < 4) {
                        z = z + 1;
                        success = true;
                    }
                    break;
                case 5: // -Z
                    if (z - 1 > 0) {
                        z = z - 1;
                        success = true;
                    }
                    break;
                case 6: // No Movement
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    success = true;
                    break;


            }
        }
        trail[3] = trail[2];
        trail[2] = trail[1];
        trail[1] = trail[0];
        trail[0].x = x;
        trail[0].y = y;
        trail[0].z = z;
        frame.clear();
        frame.setPixel(trail[3].x, trail[3].y, trail[3].z, L3);
        frame.setPixel(trail[2].x, trail[2].y, trail[2].z, L2);
        frame.setPixel(trail[1].x, trail[1].y, trail[1].z, L1);
        frame.setPixel(trail[0].x, trail[0].y, trail[0].z, L0);
        cube.displayFrame(frame);
    }

    /*
    std::vector<MonochromeCube<4, 4, 4>::Frame> *pattern = &pattern_1;
    while (true) {
        for (int f = 0; f < pattern->size(); f++) {
            cube.displayFrame((*pattern)[f]);
        }
    }
    */
}

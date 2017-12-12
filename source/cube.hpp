#ifndef LEDCUBE_CUBE_HPP
#define LEDCUBE_CUBE_HPP
#include <iostream>
#include <wiringPi.h>
#include <cassert>
#include <cstdint>
#include <array>

const uint8_t DELAY = 50;

template<uint8_t WIDTH, uint8_t HEIGHT, uint8_t DEPTH>
class MonochromeCube {
public:
    using Buffer = std::array<uint8_t, WIDTH * HEIGHT * DEPTH>;

    class Frame {
        friend class MonochromeCube;

        Buffer m_buffer;
    public:
        unsigned int getWidth() const { return WIDTH; }

        unsigned int getHeight() const { return HEIGHT; }

        unsigned int getDepth() const { return DEPTH; }

        Frame &set(uint8_t x, uint8_t y, uint8_t z, uint8_t brightness) {
            assert(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT && 0 <= z && z < DEPTH);
            m_buffer[y * WIDTH * DEPTH + z * WIDTH + x] = brightness;
            return *this;
        }

        bool getState(uint8_t mask, uint8_t x, uint8_t y, uint8_t z) {
            assert(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT && 0 <= z && z < DEPTH);
            return m_buffer[y * WIDTH * DEPTH + z * WIDTH + x] & mask;
        }

        Frame() {}

        Frame(Buffer buffer) : m_buffer(buffer) {}
    };

    MonochromeCube(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t masterResetPin, uint8_t outputEnablePin) :
            m_dataPin(dataPin), m_clockPin(clockPin), m_latchPin(latchPin), m_masterResetPin(masterResetPin), m_outputEnablePin(outputEnablePin) {
        wiringPiSetupGpio();
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
        pinMode(masterResetPin, OUTPUT);
        pinMode(outputEnablePin, OUTPUT);
        digitalWrite(dataPin, LOW); delay(DELAY);
        digitalWrite(clockPin, LOW); delay(DELAY);
        digitalWrite(latchPin, LOW); delay(DELAY);
        resetOutput();
        enableOutput();
    }

    ~MonochromeCube() {}

    void enableOutput() {
        digitalWrite(m_outputEnablePin, LOW); delay(DELAY);
    }

    void disableOutput() {
        digitalWrite(m_outputEnablePin, HIGH); delay(DELAY);
    }

    void resetOutput() {
        digitalWrite(m_masterResetPin, LOW); delay(DELAY);
        digitalWrite(m_masterResetPin, HIGH); delay(DELAY);
    }

    void displayFrame(Frame frame) {
        for (uint8_t seq = 0; seq < 8; seq++) {
            const uint8_t mask = (1 << seq);
            for (uint8_t y = 0; y < HEIGHT; y++) {
                std::cout << '_';
                digitalWrite(m_latchPin, LOW); delay(DELAY);
                for (uint8_t x = 0; x < WIDTH; x++) {
                    for (uint8_t z = 0; z < DEPTH; z++) {
                        const bool bit = frame.getState(mask, x, y, z);
                        std::cout << (uint8_t)bit;
                        digitalWrite(m_dataPin, bit? HIGH: LOW); delay(DELAY);
                        digitalWrite(m_clockPin, HIGH); delay(DELAY);
                        digitalWrite(m_clockPin, LOW); delay(DELAY);
                    }
                }
                std::cout << ' ';
                for (uint8_t h = 0; h < HEIGHT; h++) {
                    const bool bit = h == y ? HIGH : LOW;
                    std::cout << (uint8_t)bit;
                    digitalWrite(m_dataPin, bit); delay(DELAY);
                    digitalWrite(m_clockPin, HIGH); delay(DELAY);
                    digitalWrite(m_clockPin, LOW); delay(DELAY);
                }
                std::cout << 'L' << std::endl;
                digitalWrite(m_latchPin, HIGH); delay(DELAY);
            }
        }
    }

private:
    const uint8_t m_dataPin;
    const uint8_t m_clockPin;
    const uint8_t m_latchPin;
    const uint8_t m_masterResetPin;
    const uint8_t m_outputEnablePin;
};

#endif//LEDCUBE_CUBE_HPP
#ifndef LEDCUBE_CUBE_HPP
#define LEDCUBE_CUBE_HPP

#include <wiringPi.h>
#include <cassert>
#include <cstdint>
#include <vector>
#include <array>

const uint8_t shiftDelay = 10;

template<uint8_t WIDTH, uint8_t HEIGHT, uint8_t DEPTH>
class MonochromeCube {
public:
    using Buffer = std::array<uint8_t, WIDTH * HEIGHT * DEPTH>;

    class Frame {
        uint8_t m_duration;
        Buffer m_buffer;
    public:
        uint8_t getWidth() const { return WIDTH; }

        uint8_t getHeight() const { return HEIGHT; }

        uint8_t getDepth() const { return DEPTH; }

        uint8_t getDuration() const { return m_duration; }

        Frame &setDuration(uint8_t duration) {
            m_duration = duration;
            return *this;
        }

        Frame &clear() { m_buffer.fill(0); }

        Frame &setPixel(uint8_t x, uint8_t y, uint8_t z, uint8_t luminosity) {
            assert(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT && 0 <= z && z < DEPTH);
            m_buffer[y * WIDTH * DEPTH + z * WIDTH + x] = luminosity;
            return *this;
        }


        bool getState(uint8_t mask, uint8_t x, uint8_t y, uint8_t z) {
            assert(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT && 0 <= z && z < DEPTH);
            return m_buffer[y * WIDTH * DEPTH + z * WIDTH + x] & mask;
        }

        Frame() {}

        Frame(Buffer buffer) : m_buffer(buffer) {}

        Frame(Buffer buffer, uint8_t duration) : m_buffer(buffer), m_duration(duration) {}
    };

    class Animation {
        /// Empty Frame
        static const Frame emptyFrame = Frame(Buffer(), 1);

        std::vector<Frame> m_frames;
        std::vector<size_t> m_indices;
        bool m_repeat;
    public:
        Animation();

        Animation(std::vector<Frame> frames)
                : m_frames(frames), m_repeat(false) {
            for (int i = 0; i < frames.size(); i++) m_indices.push_back(i);
        }

        Animation(std::vector<Frame> frames, bool repeat)
                : m_frames(frames), m_repeat(repeat) {
            for (int i = 0; i < frames.size(); i++) m_indices.push_back(i);
        }

        Animation(std::vector<Frame> frames, std::vector<size_t> indices)
                : m_frames(frames), m_indices(indices), m_repeat(false) {
        }


        Animation(std::vector<Frame> frames, std::vector<size_t> indices, bool repeat)
                : m_frames(frames), m_indices(indices), m_repeat(repeat) {
        }

        const Frame &getFrame(size_t frameNumber) {
            if (m_repeat) {
                return m_frames[m_indices[frameNumber % m_indices.size()]];
            } else if (frameNumber > m_indices.size()) {
                return emptyFrame;
            }
        }
    };

    MonochromeCube(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t masterResetPin, uint8_t outputEnablePin)
            : m_dataPin(dataPin), m_clockPin(clockPin), m_latchPin(latchPin), m_masterResetPin(masterResetPin),
              m_outputEnablePin(outputEnablePin) {
        wiringPiSetupGpio();
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
        pinMode(masterResetPin, OUTPUT);
        pinMode(outputEnablePin, OUTPUT);
        digitalWrite(dataPin, LOW);
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, LOW);
        digitalWrite(masterResetPin, LOW);
        delay(shiftDelay);
        digitalWrite(masterResetPin, HIGH);
        digitalWrite(outputEnablePin, LOW);
    }

    ~MonochromeCube() {}

    void enableOutput() {
        digitalWrite(m_outputEnablePin, LOW);
    }

    void disableOutput() {
        digitalWrite(m_outputEnablePin, HIGH);
    }

    void resetOutput() {
        digitalWrite(m_masterResetPin, LOW);
        digitalWrite(m_masterResetPin, HIGH);
    }

    void displayAnimation(Animation animation) {

    }

    void displayFrame(Frame frame) {
        for (uint8_t t = frame.getDuration(); t > 0; t--) {
            for (uint8_t seq = 0; seq < 8; seq++) {
                const uint8_t mask = (1 << seq);
                for (uint8_t y = 0; y < HEIGHT; y++) {
                    digitalWrite(m_latchPin, LOW);
                    for (uint8_t x = 0; x < WIDTH; x++) {
                        for (uint8_t z = 0; z < DEPTH; z++) {
                            const bool bit = frame.getState(mask, x, y, z);
                            digitalWrite(m_dataPin, bit ? HIGH : LOW);
                            digitalWrite(m_clockPin, HIGH);
                            delayMicroseconds(shiftDelay);
                            digitalWrite(m_clockPin, LOW);
                        }
                    }
                    for (uint8_t h = 0; h < HEIGHT; h++) {
                        const bool bit = (h == y ? HIGH : LOW);
                        digitalWrite(m_dataPin, bit ? HIGH : LOW);
                        digitalWrite(m_clockPin, HIGH);
                        delayMicroseconds(shiftDelay);
                        digitalWrite(m_clockPin, LOW);
                        digitalWrite(m_dataPin, LOW);
                    }
                    digitalWrite(m_latchPin, HIGH);
                    delayMicroseconds(shiftDelay);
                }
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

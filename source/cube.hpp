#include <wiringPi.h>
#include <cassert>
#include <cstdint>
#include <array>

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

    MonochromeCube(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin) :
            m_dataPin(dataPin), m_clockPin(clockPin), m_latchPin(latchPin) {
        wiringPiSetupGpio();
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
    }

    ~MonochromeCube() {}

    void displayFrame(Frame frame) {
        for (uint8_t seq = 0; seq < 8; seq++) {
            const uint8_t mask = (1 << seq);
            for (uint8_t y = 0; y < HEIGHT; y++) {
                digitalWrite(m_latchPin, LOW);
                for (uint8_t x = 0; x < WIDTH; x++) {
                    for (uint8_t z = 0; z < DEPTH; z++) {
                        digitalWrite(m_dataPin, frame.getState(x, y, z, mask));
                        digitalWrite(m_clockPin, HIGH);
                        digitalWrite(m_clockPin, LOW);
                    }
                }
                for (uint8_t h = 0; h < HEIGHT; h++) {
                    digitalWrite(m_dataPin, h == y ? HIGH : LOW);
                    digitalWrite(m_clockPin, HIGH);
                    digitalWrite(m_clockPin, LOW);
                }
                digitalWrite(m_latchPin, HIGH);
            }
        }
    }

private:
    const uint8_t m_dataPin;
    const uint8_t m_clockPin;
    const uint8_t m_latchPin;
};

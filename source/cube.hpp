#include <cstdint>
#include <array>

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int DEPTH, unsigned int COLORS>
class Cube {
public:
    using FrameBuffer = std::array<uint8_t, WIDTH * HEIGHT * DEPTH * COLORS>;
    Cube();
    ~Cube();
private:
    void shiftOut(uint8_t seq, FrameBuffer frame);
}

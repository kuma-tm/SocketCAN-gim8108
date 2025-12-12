#include <cstdint>
#include <cstring>
#include <socketcan_cpp.h>

using namespace scpp;

// GIM8108 用 位置指令
//  pos は「回転数 [rev]」想定（例: 1回転なら 1.0f）
bool set_position(SocketCan& can, uint16_t node_id, float pos)
{
    const uint16_t cmd_id = 0x00C;  // Set_Input_Pos
    const uint32_t can_id = (static_cast<uint32_t>(node_id) << 5) | cmd_id;

    CanFrame frame{};
    frame.id    = can_id;
    frame.len   = 8;      // GIM8108 の CAN データ長は常に 8 byte
    frame.flags = 0;

    // Byte0-3: Input_Pos (float, little-endian)
    static_assert(sizeof(float) == 4, "float must be 4 bytes");
    std::memcpy(&frame.data[0], &pos, sizeof(float));

    // Byte4-5: Vel_FF (ここでは 0 にしておく)
    uint16_t vel_ff = 0;
    std::memcpy(&frame.data[4], &vel_ff, sizeof(uint16_t));

    // Byte6-7: Torque_FF (ここでは 0 にしておく)
    uint16_t torque_ff = 0;
    std::memcpy(&frame.data[6], &torque_ff, sizeof(uint16_t));

    auto status = can.write(frame);
    return (status == STATUS_OK);
}

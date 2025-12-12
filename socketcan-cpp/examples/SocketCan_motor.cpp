#include <iostream>
#include <socketcan_cpp.h>

using namespace scpp;

bool set_position(SocketCan& can, uint16_t node_id, float pos); // 上で定義したやつ

int main()
{
    SocketCan can;
    auto status = can.open("can0", 3, MODE_CAN_MTU);  // 古典CAN, タイムアウト3ms
    if (status != STATUS_OK) {
        std::cerr << "CAN open failed: " << status << std::endl;
        return -1;
    }

    uint16_t node_id = 20;      // odrv0.axis0.config.can.node_id と一致させる
    float target_pos = 3.14f;  // 3.14 [rev] だけ回したい例

    if (!set_position(can, node_id, target_pos)) {
        std::cerr << "set_position failed" << std::endl;
        return -1;
    }

    std::cout << "Position command sent." << std::endl;
    return 0;
}
/* 1	can.open("can0", 3, MODE_CAN_MTU) → PF_CAN/SOCK?_RAWソケット開いてcan0にbind

   2	set_position(can, 5, 3.14f) → node_id = 5, pos = 3.14[rev]の位置指令フレームを送信		*/

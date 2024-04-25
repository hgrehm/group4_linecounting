#include "../include/input_packet.h"
#include <cstring>

// Ensure outData is large enough to store all the data that is intended to be serialized.
void InputPacket::serialize(const InputPacket& input, char*& outData) {
    char* temp = outData;
    
    // Serialize size of events
    size_t numEvents = input.events.size();
    memcpy(temp, &numEvents, sizeof(numEvents));
    temp += sizeof(numEvents);

    // Serialize events
    memcpy(temp, input.events.data(), sizeof(int) * numEvents);
    temp += sizeof(int) * numEvents;

    // Serialize cam_angle
    memcpy(temp, &input.cam_angle, sizeof(input.cam_angle));
    temp += sizeof(input.cam_angle);
}

void InputPacket::deserialize(const char* inData, InputPacket& input) {
    const char* temp = inData;

    // Deserialize size of events
    size_t numEvents;
    memcpy(&numEvents, temp, sizeof(numEvents));
    temp += sizeof(numEvents);

    // Deserialize events
    input.events.clear();
    input.events.resize(numEvents);
    memcpy(input.events.data(), temp, sizeof(int) * numEvents);
    temp += sizeof(int) * numEvents;

    // Deserialize cam_angle
    memcpy(&input.cam_angle, temp, sizeof(input.cam_angle));
}

#include "../include/game_state_packet.h"
#include <cstring>

size_t GameStatePacket::calculateSize() const {
    size_t totalSize = 0;

    totalSize += sizeof(size_t);  // To store the number of players
    totalSize += state.players.size() * (sizeof(float) * 16 + sizeof(int)); // Each player's size: 16 floats for world matrix, 1 int for score

    totalSize += sizeof(size_t);  // To store the number of students
    totalSize += state.students.size() * (sizeof(float) * 16); // Each student's size: 16 floats for world matrix

    // Add size of level
    totalSize += sizeof(state.level);

    return totalSize;
}

// Ensure outData is large enough to store all the data that is intended to be serialized.
void GameStatePacket::serialize(const GameStatePacket& packet, char*& outData) {
    char* temp = outData;
    
    // Serialize number of players
    size_t numPlayers = packet.state.players.size();
    memcpy(temp, &numPlayers, sizeof(numPlayers));
    temp += sizeof(numPlayers);

    // Serialize each player's state including 16 floats of world matrix, and score
    for (const PlayerState& player : packet.state.players) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                memcpy(temp, &player.world[i][j], sizeof(player.world[i][j]));
				temp += sizeof(player.world[i][j]);
            }
        }

        memcpy(temp, &player.score, sizeof(player.score));
        temp += sizeof(player.score);

    }

    // Serialize number of students
    size_t numStudents = packet.state.students.size();
    memcpy(temp, &numStudents, sizeof(numStudents));
    temp += sizeof(numStudents);

    // Serialize each student's state including 16 floats of world matrix
    for (const StudentState& student : packet.state.students) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				memcpy(temp, &student.world[i][j], sizeof(student.world[i][j]));
                temp += sizeof(student.world[i][j]);
            }
        }
    }

    // Serialize level
    memcpy(temp, &packet.state.level, sizeof(packet.state.level));
    temp += sizeof(packet.state.level);
}

void GameStatePacket::deserialize(const char* inData, GameStatePacket& packet) {
    // Deserialize number of players
    size_t numPlayers;
    memcpy(&numPlayers, inData, sizeof(numPlayers));
    inData += sizeof(numPlayers);
    packet.state.players.clear();
    packet.state.players.resize(numPlayers);

    // Deserialize each player's state including 16 floats for world matrix and score
    for (PlayerState& player : packet.state.players) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				memcpy(&player.world[i][j], inData, sizeof(float));
				inData += sizeof(float);
			}
        }

        memcpy(&player.score, inData, sizeof(player.score));
        inData += sizeof(player.score);
    }

    // Deserialize number of students
    size_t numStudents;
    memcpy(&numStudents, inData, sizeof(numStudents));
    inData += sizeof(numStudents);
    packet.state.students.clear();
    packet.state.students.resize(numStudents);

    // Deserialize each student's state including 16 floats for world matrix
    for (StudentState& student : packet.state.students) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				memcpy(&student.world[i][j], inData, sizeof(float));
				inData += sizeof(float);
			}
        }
    }

    // Deserialize level
    memcpy(&packet.state.level, inData, sizeof(packet.state.level));
    inData += sizeof(packet.state.level);
}
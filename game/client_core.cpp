#include "../include/client_core.h"
#include <iostream>

ClientCore::ClientCore() {
    this->connected = false;
    this->server_state = LOBBY;
}

ClientCore::~ClientCore()
{
    if (connected) // only shutdown if it hasn't been called already (else double-closes)
        shutdown();
}

void ClientCore::initialize()
{
    while (!client.is_connected()) {
        client.connect_to_server();
    }
    
    // recv id from server
    char* buffer = client.sock_receive();
    while (!buffer || !buffer[0]){
        buffer = client.sock_receive();
    }
    this->id = *((short*)buffer) - 1;
    connected = true;
    printf("client connected with id %d\n", this->id);

    // Initialize graphics
    window = Graphics::set_up_window(this->id);
}

bool ClientCore::is_connected() {
    return connected;
}

void ClientCore::shutdown()
{
    // Clean up all resources
    connected = false;
    Window::clean_up();
    client.close_conn();
}

void ClientCore::run()
{
    while (false) { // eventually: while this->server_state == LOBBY
        receive_updates();
    }
    while (connected) // && state == MAIN_LOOP ?
    {
        receive_updates();
        process_server_data();
        renderGameState();
        send_input(); // moving to bottom bc only send can shutdown
    }
}

void ClientCore::send_input()
{
    InputPacket packet;/*
    packet.events.push_back(0);
    packet.events.push_back(1);*/
    packet.cam_angle = Window::get_cam_angle_radians();
    // TODO: Get events and push it into packet
    std::vector<int> tmp = Window::get_input_actions();
    for (int event : tmp)
        packet.events.push_back(event);

    // TODO: Get cam_angle and push it to packet - Camera controls need to be implemented first
 
    if (glfwWindowShouldClose(window)) {
        shutdown();
        return;
    }

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];

    InputPacket::serialize(packet, buffer);
    if (!client.sock_send((int)bufferSize, buffer)) {
        shutdown();
    }

    delete[] buffer;
}

void ClientCore::receive_updates() {
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10;

    char * received_data = NULL;
    ServerHeartbeatPacket hb;
    GameStatePacket packet;

    FD_SET(client.conn_sock, &readFdSet);
    while (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        received_data = client.sock_receive();
        if (received_data == NULL) {
            printf("receive failed. exiting from client\n");
            shutdown();
            return;
        }
        if (received_data && received_data[0]){
            PacketType type = Packet::get_packet_type(received_data);
            switch(type) {
                case SERVER_HEARTBEAT:
                    ServerHeartbeatPacket::deserialize(received_data, hb);
                    this->server_state = hb.state;
                    break;
                case GAME_STATE:
                    GameStatePacket::deserialize(received_data, packet);
                    this->world_state = packet.state;
                    break;
                default: // shouldn't reach this
                    printf("Error: unexpected receipt of packet type %d\n", type);
                    shutdown(); // not ideal but ehhh
            }

            //printf("client got \"%s\" from server\n", received_data);
        }
        timeout.tv_sec = 0;
        timeout.tv_usec = 10;
    }
}
void ClientCore::process_server_data() {
    // Processed in Window
    Window::update_state(world_state);
}

void ClientCore::renderGameState()
{
    printf("\n\n");
  //  std::cout << "Level: " << clientState.level << std::endl;
  //  std::cout << "Players:" << std::endl;
  //  for (const auto &player : clientState.players)
  //  {
  //      // Print player world matrix
  //      for (int i = 0; i < 4; i++)
  //      {
  //          for (int j = 0; j < 4; j++)
  //          {
		//		std::cout << player.world[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}
  //      std::cout << "  Score: " << player.score << std::endl;
  //  }

    // Don't need students rn...
    //std::cout << "Students:" << std::endl;
    //for (const auto &student : clientState.students)
    //{
    //    // Print student world matrix
    //    for (int i = 0; i < 4; i++)
    //    {
    //        for (int j = 0; j < 4; j++)
    //        {
    //            std::cout << student.world[i][j] << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}
    printf("\n\n");



    // Render
    Window::display_callback(window);
    Window::idle_callback();
}
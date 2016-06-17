#include "jacktelop/listener_handler.hpp"

Handler::~Handler() {}

Handler::Handler(geometry_msgs::Twist *msg) {
    this->msg = msg;
}

void Handler::handleMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
        const jacktelop::Twist_t *msg) {
    this->msg->linear.x = msg->linear.x;
    this->msg->angular.z = msg->angular.z;
    std::cout << "linear: "
              << "[" << msg->linear.x << "," << msg->linear.y << ","
              << msg->linear.z << "]" << std::endl;
    std::cout << "angular: "
              << "[" << msg->angular.x << "," << msg->angular.y << ","
              << msg->angular.z << "]" << std::endl;
    std::cout << "---------------------------------" << std::endl; 
 }



#include "ros/ros.h"
#include "jacktelop/listener_handler.hpp"

int main(int argc, char **argv) {
  //set up LCM
  lcm::LCM *lcm = new lcm::LCM("udpm://239.255.76.67:7667?ttl=1");
  if (!lcm->good())
    return 1;

  //set up ROS
  ros::init(argc, argv, "lcm_ros_bridge");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/navi", 100);

  ros::Rate loop_rate(10);

  geometry_msgs::Twist twist;

  Handler *handlerObject = new Handler(&twist);
  lcm->subscribe("/cmd_vel", &Handler::handleMessage, handlerObject);

  int count = 0;
  while (0 == lcm->handle() && ros::ok()) {
    pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
    // zero
    twist.linear.x = 0;
    twist.angular.z = 0;
  }
  delete lcm;
  return 0;
}

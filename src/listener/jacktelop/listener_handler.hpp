#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "jacktelop/Twist_t.hpp"
#include "jacktelop/Vector3_t.hpp"
#include "geometry_msgs/Twist.h"

class Handler {
public:
   geometry_msgs::Twist *msg;
   Handler(geometry_msgs::Twist *msg);
  ~Handler();

  void handleMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                     const jacktelop::Twist_t *msg);
};



#include "spin_sol.h"
#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
using std::placeholders::_1;
int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

//your code here
SpinSolution::SpinSolution() : Node("spinsolution") {
  RCLCPP_INFO(this->get_logger(), "Remove this statement from spin_sol.cpp");
  // your code here
  estimate_publisher = this->create_publisher<ArrayMsg>("predictedpos",10);
  pos_sub = this->create_subscription<ArrayMsg>(
    "measuredpos", 10, std::bind(&SpinSolution::pos_callback, this, _1)
  );
  vel_sub = this->create_subscription<ArrayMsg>(
    "measuredvel", 10, std::bind(&SpinSolution::vel_callback, this, _1)
  );

  timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500), std::bind(&SpinSolution::est_pub, this));
  
}

void SpinSolution::pos_callback(const ArrayMsg::SharedPtr msg){
  RCLCPP_INFO(this->get_logger(),"Position: %f",msg->data[0]);
  cur_pos = msg->data;
}

void SpinSolution::vel_callback(const ArrayMsg::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(),"Velocity: %f",msg->data[0]);
  cur_vel = msg->data;
}

void SpinSolution::est_pub()
{
  for(std::size_t  i = 0; i<cur_pos.size(); i++){
    cur_pos[i] += cur_vel[i]/3;
  }
  ArrayMsg msg_out;
  msg_out.data = cur_pos;
  estimate_publisher -> publish(msg_out);
  RCLCPP_INFO(this->get_logger(),"Pub %f | %f",cur_pos[0],cur_pos[1]);
}


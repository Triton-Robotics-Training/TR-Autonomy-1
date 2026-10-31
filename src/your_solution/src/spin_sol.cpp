#include "spin_sol.h"
#include <chrono>
using namespace std::chrono_literals;
int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

//your code here
SpinSolution::SpinSolution() : Node("spinsolution") {
  last_time_ = this->now();
  pred_pub_= this->create_publisher<ArrayMsg>("predictedpos", 10);
  pos_sub_= this->create_subscription<ArrayMsg>(
	"measuredpos", 10, 
	std::bind(&SpinSolution::pos_callback, this, std::placeholders::_1));
  vel_sub_ = this->create_subscription<ArrayMsg>(
	"measuredvel", 10, 
	std::bind(&SpinSolution::vel_callback, this, std::placeholders::_1));
  timer_ =this->create_wall_timer(
	1ms, std::bind(&SpinSolution::timer_callback, this)); 
}
void SpinSolution::pos_callback(const ArrayMsg::SharedPtr msg){
  if(msg->data.size() < 2){ return;}
  x_ = msg->data[0];
  y_ = msg->data[1];
  last_time_ = this->now();
  have_pos_ = true;
}

void SpinSolution::vel_callback(const ArrayMsg:: SharedPtr msg){
  if(msg->data.size() < 2){return;}
  vx_ = msg->data[0];
  vy_ = msg->data[1];
}

void SpinSolution::timer_callback(){
  if(!have_pos_){return;}
  double dt = (this->now() - last_time_).seconds();
  ArrayMsg out;
  out.data = {x_ + dt*vx_, y_+ dt*vy_};
  pred_pub_->publish(out);
}

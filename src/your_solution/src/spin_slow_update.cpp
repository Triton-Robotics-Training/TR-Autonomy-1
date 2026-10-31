#include "spin_slow_update.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

SlowSolution::SlowSolution() : Node("slowsolution") {
  pred_pub_ = this->create_publisher<ArrayMsg>("predictedpos", 10);
  pos_sub_= this->create_subscription<ArrayMsg>("measuredpos", 10, std::bind(&SlowSolution::pos_callback, this, std::placeholders::_1));
}

// your code here
void SlowSolution::pos_callback(const ArrayMsg::SharedPtr msg){
   pred_pub_->publish(*msg);
}

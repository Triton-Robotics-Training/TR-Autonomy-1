#include "spin_slow_update.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

SlowSolution::SlowSolution() : Node("slowsolution") {
  sub_ = this->create_subscription<ArrayMsg>(
  	"/measuredpos", 10,
  	std::bind(&SlowSolution::sub_cb, this, std::placeholders::_1));
  	
  pub_ = this->create_publisher<ArrayMsg>("/predictedpos",10);
  	
  timer_ = this->create_wall_timer(
  	std::chrono::milliseconds(500),
  	std::bind(&SlowSolution::timer_cb, this));
}

void SlowSolution::sub_cb(const ArrayMsg::SharedPtr msg) {
	latest_msg_ = *msg;
}

void SlowSolution::timer_cb() {
  if (!latest_msg_.data.empty()) {
	pub_->publish(latest_msg_);
  }
}

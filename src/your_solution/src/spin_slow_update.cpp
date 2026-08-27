#include "spin_slow_update.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

SlowSolution::SlowSolution() : Node("slowsolution") {
  // your code here
  publisher_ = this->create_publisher<ArrayMsg>("predictedpos", 10);
  subscriberPos_ = this->create_subscription<ArrayMsg>("measuredpos", 10, std::bind(&SlowSolution::topicP_callback, this, std::placeholders::_1));
  subscriberVel_ = this->create_subscription<ArrayMsg>("measuredvel", 10, std::bind(&SlowSolution::topicV_callback, this, std::placeholders::_1));
  timer_ = this->create_wall_timer(REFRESH,
		std::bind(&SlowSolution::timer_callback, this));
}

// your code here
void SlowSolution::topicP_callback(const std::shared_ptr<const ArrayMsg> & pos) {
	x_ = pos->data[0];
	y_ = pos->data[1];
}


void SlowSolution::topicV_callback(const std::shared_ptr<const ArrayMsg> & vel) {
	vx_ = vel->data[0];
	vy_ = vel->data[1];
}

void SlowSolution::timer_callback() {
	auto message = ArrayMsg();
	double t = REFRESH.count() / 1000000000.0;
	message.data = { x_ + vx_ * t, y_ + vy_ * t };
	publisher_->publish(message);
}

#include "spin_sol.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}


SpinSolution::SpinSolution() : Node("spinsolution") {
  // your code here
  publisher_ = this->create_publisher<ArrayMsg>("predictedpos", 10);
  subscriberPos_ = this->create_subscription<ArrayMsg>("measuredpos", 10, std::bind(&SpinSolution::topicP_callback, this, std::placeholders::_1));
  subscriberVel_ = this->create_subscription<ArrayMsg>("measuredvel", 10, std::bind(&SpinSolution::topicV_callback, this, std::placeholders::_1));
  timer_ = this->create_wall_timer(REFRESH,
                std::bind(&SpinSolution::timer_callback, this));
}

// your code here
void SpinSolution::topicP_callback(const std::shared_ptr<const ArrayMsg> & pos) {
        x_ = pos->data[0];
        y_ = pos->data[1];
	last_time = clock.now();
}


void SpinSolution::topicV_callback(const std::shared_ptr<const ArrayMsg> & vel) {
        vx_ = vel->data[0];
        vy_ = vel->data[1];
	last_time = clock.now();
}

void SpinSolution::timer_callback() {
        auto now = clock.now();
	
        double t = (now - last_time).seconds();
	x_ += vx_ * t;
	y_ += vy_ * t;

	last_time = now;
	
	auto message = ArrayMsg();
        message.data = { x_ , y_ };
        publisher_->publish(message);
}


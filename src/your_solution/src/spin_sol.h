#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include <memory>
#include <functional>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;
using namespace std::chrono_literals;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
    // your code here
  void timer_callback();
  void topicP_callback(const std::shared_ptr<const ArrayMsg> & pos);
  void topicV_callback(const std::shared_ptr<const ArrayMsg> & vel);
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr publisher_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr subscriberPos_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr subscriberVel_;

  std::chrono::nanoseconds REFRESH = 5ns;
  double x_, y_, vx_, vy_;
  rclcpp::Clock clock;
  rclcpp::Time last_time = clock.now();
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_

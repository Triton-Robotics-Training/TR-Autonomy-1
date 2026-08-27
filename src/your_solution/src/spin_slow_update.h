#ifndef YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
#define YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_

#include <memory>
#include <functional>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;
using namespace std::chrono_literals;

class SlowSolution : public rclcpp::Node {
 public:
  SlowSolution();
 private:
  // your code here
  void timer_callback();
  void topicP_callback(const std::shared_ptr<const ArrayMsg> & pos);
  void topicV_callback(const std::shared_ptr<const ArrayMsg> & vel);
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr publisher_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr subscriberPos_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr subscriberVel_;
  
  std::chrono::nanoseconds REFRESH = 500000ns;
  double x_, y_, vx_, vy_;
};

#endif //YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_

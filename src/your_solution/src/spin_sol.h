#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
    SpinSolution();
 private:
    // your code here
    rclcpp::Subscription<ArrayMsg>::SharedPtr pos_sub;
    rclcpp::Subscription<ArrayMsg>::SharedPtr vel_sub;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<ArrayMsg>::SharedPtr estimate_publisher;
    std::vector<double> cur_pos = {0.0,0.0};
    std::vector<double> cur_vel = {0.0,0.0};
    void pos_callback(const ArrayMsg::SharedPtr msg);
    void vel_callback(const ArrayMsg::SharedPtr msg);
    void est_pub();
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_

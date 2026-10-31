#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
    void pos_cb(const ArrayMsg::SharedPtr msg);
    void vel_cb(const ArrayMsg::SharedPtr msg);
    void timer_cb();
    
    rclcpp::Subscription<ArrayMsg>::SharedPtr pos_sub_;
    rclcpp::Subscription<ArrayMsg>::SharedPtr vel_sub_;
    rclcpp::Publisher<ArrayMsg>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    
    ArrayMsg latest_pos_;
    ArrayMsg latest_vel_;
    rclcpp:: Time last_msg_time_;
    bool received_data_ = false;
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_

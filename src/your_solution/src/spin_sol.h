#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
public:
    SpinSolution();
private:
    void timer_callback() {
        std::array<double, 2> predicted_pos = predict_position(current_pos_, current_vel_);

        ArrayMsg msg;
        msg.data = {predicted_pos[0], predicted_pos[1]};
        publisher_->publish(msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Time last_measurement_time_;

    rclcpp::Publisher<ArrayMsg>::SharedPtr publisher_;
    rclcpp::Subscription<ArrayMsg>::SharedPtr pos_subscription_;
    rclcpp::Subscription<ArrayMsg>::SharedPtr vel_subscription_;
    std::array<double, 2> current_pos_{0.0, 0.0};
    std::array<double, 2> current_vel_{0.0, 0.0};

    void measured_pos_callback(const ArrayMsg::SharedPtr msg);
    void measured_vel_callback(const ArrayMsg::SharedPtr msg);

    std::array<double, 2> predict_position(std::array<double, 2> current_pos, std::array<double, 2> current_vel);
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_

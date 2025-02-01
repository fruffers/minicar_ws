#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/point.hpp>
#include "json_reader.cpp"

class TrackVisualizer : public rclcpp::Node
{
public:
    TrackVisualizer() : Node("track_visualizer")
    {
        marker_pub = this->create_publisher<visualization_msgs::msg::MarkerArray>(
            "visualization_marker_array", rclcpp::QoS(10).reliable().transient_local()
        );

        // Create a timer to publish markers every second
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&TrackVisualizer::publishMarkers, this)
        );

        // Load the cones once at startup
        cones_ = parseTrackLayout("/home/louise-bilous/Coding/minicar_ws/src/groundtruthcones_visual_pkg/json/dirtfish.json");
    }

private:
    struct Cone {
        double x, y;
        std::string colour;
        bool is_big;  // Distinguish between regular and big orange cones
    };

    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub;
    rclcpp::TimerBase::SharedPtr timer_;
    std::vector<Cone> cones_;

    std::vector<Cone> parseTrackLayout(const std::string& filename) {
        // Parse the JSON file and extract all cone types
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Could not open JSON file: " + filename);
        }

        nlohmann::json json_data;
        file >> json_data;

        std::vector<Cone> cones;

        // Parse blue cones
        for (const auto& point : json_data["blue"]) {
            cones.push_back({point[0], point[1], "blue", false});
        }

        // Parse yellow cones
        for (const auto& point : json_data["yellow"]) {
            cones.push_back({point[0], point[1], "yellow", false});
        }

        // Parse regular orange cones
        for (const auto& point : json_data["orange"]) {
            cones.push_back({point[0], point[1], "orange", false});
        }

        // Parse big orange cones
        for (const auto& point : json_data["big"]) {
            cones.push_back({point[0], point[1], "orange", true});
        }

        return cones;
    }

    void publishMarkers()
    {
        auto markers = visualization_msgs::msg::MarkerArray();

        int id = 0;
        for (const auto& cone : cones_)
        {
            visualization_msgs::msg::Marker marker;
            marker.header.frame_id = "map";
            marker.header.stamp = this->now();
            marker.ns = "track_cones";
            marker.id = id++;
            marker.type = visualization_msgs::msg::Marker::CYLINDER;
            marker.action = visualization_msgs::msg::Marker::ADD;

            // Set position
            marker.pose.position.x = cone.x;
            marker.pose.position.y = cone.y;
            marker.pose.position.z = 0.0;

            // Set scale based on cone type
            if (cone.is_big) {
                marker.scale.x = 0.6;  // Larger for big orange cones
                marker.scale.y = 0.6;
                marker.scale.z = 1.0;
            } else {
                marker.scale.x = 0.3;
                marker.scale.y = 0.3;
                marker.scale.z = 0.5;
            }

            // Set color based on cone color
            if (cone.colour == "blue") {
                marker.color.r = 0.0;
                marker.color.g = 0.0;
                marker.color.b = 1.0;
            } else if (cone.colour == "yellow") {
                marker.color.r = 1.0;
                marker.color.g = 1.0;
                marker.color.b = 0.0;
            } else if (cone.colour == "orange") {
                marker.color.r = 1.0;
                marker.color.g = 0.65;  // Orange color
                marker.color.b = 0.0;
            }

            marker.color.a = 1.0;

            // Add marker to array
            markers.markers.push_back(marker);
        }

        // Publish markers
        marker_pub->publish(markers);
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TrackVisualizer>());
    rclcpp::shutdown();
    return 0;
}

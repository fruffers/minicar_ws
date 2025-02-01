// #include "publish_cone_markers.cpp"


// visualization_msgs::msg::Marker path_marker;
// path_marker.header.frame_id = "map";
// path_marker.ns = "planned_path";
// path_marker.id = 0;
// path_marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
// path_marker.action = visualization_msgs::msg::Marker::ADD;

// path_marker.scale.x = 0.1;
// path_marker.color.r = 0.0;
// path_marker.color.g = 1.0;
// path_marker.color.b = 0.0;
// path_marker.color.a = 1.0;

// // Example: add planned path points (replace with your actual path output)
// for (const auto& point : planned_path_points) {
//     geometry_msgs::msg::Point p;
//     p.x = point.x;
//     p.y = point.y;
//     p.z = 0.0;
//     path_marker.points.push_back(p);
// }

// marker_pub_->publish(path_marker);

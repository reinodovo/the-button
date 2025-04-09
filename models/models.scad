include <../../ktane-3d-models/variables.scad>;
use <../../ktane-3d-models/bomb_base.scad>;

$fn = $preview ? 10 : 50;

tolerance = 0.2;
display_height = 27;
display_width = 25;
display_support_pad_size = 4.3;

display_position_y = 6;

button_radius = 19;
button_top_radius = 23.5;
button_top_thickness = 6;
button_height_above_module = 19;
button_position_y = 56;
button_corner_radius = 2;
button_pillar_radius = 5;
button_hole_height = 11;
button_wall_thickness = 3;
button_bottom_thickness = 3;

distance_button_led_strip = 45;

led_strip_height = 56;
led_strip_width = 7.5;
led_strip_position_y = 21;
led_strip_position_x = 8.5;

module top() {
    button_position_x = size - led_strip_position_x - distance_button_led_strip;
    union() {
        difference() {
            bomb_module_top(height_above_pcb = 14, module_type = 1);
            translate([button_position_x, size - button_position_y]) cylinder(10, button_radius + tolerance / 2, button_radius + tolerance / 2, center = true);
            translate([button_position_x + distance_button_led_strip, size - led_strip_position_y - (led_strip_height + tolerance) / 2]) cube([led_strip_width + tolerance, led_strip_height + tolerance, 10], center = true);
            translate([button_position_x, size - display_position_y - (display_height + tolerance) / 2]) cube([display_width + tolerance, display_height + tolerance, 10], center = true);
        }
        translate([button_position_x, size - display_position_y - (display_height + tolerance) / 2, - 1 - wall_thickness]) {
            for (i = [-1:2:1]) {
                for (j = [-1:2:1]) {
                    translate([i * (display_width + tolerance) / 2, j * (display_height + tolerance - display_support_pad_size + 1) / 2]) cube([2 * display_support_pad_size, display_support_pad_size + 1, 2], center = true);
                }
            }
        }
    }
}

module bottom() {
    bomb_module_bottom(height_above_pcb = 14);
}

module button_top() {
    union() {
        difference() {
            union() {
                cylinder(button_height_above_module + wall_thickness, button_radius - tolerance, button_radius - tolerance);
                translate([0, 0, button_corner_radius]) minkowski() {
                    cylinder(button_top_thickness - 2 * button_corner_radius, button_top_radius - button_corner_radius, button_top_radius - button_corner_radius);
                    sphere(button_corner_radius);
                }
            }
            translate([0, 0, button_height_above_module + wall_thickness - button_hole_height]) cylinder(button_hole_height, button_radius - button_wall_thickness, button_radius - button_wall_thickness);
        }
        translate([0, 0, button_height_above_module + wall_thickness]) rotate([180, 0]) difference() {
            cylinder(button_hole_height, button_pillar_radius, button_pillar_radius);
            screw_insert_hole();
        }
    }
}

module button_bottom() {
    connector_tolerance = 0.15;
    module screw_hole() {
        hull() {
            translate([0, 0, 0.01 + button_bottom_thickness - (screw_head_hole_radius - screw_body_hole_radius)]) cylinder(screw_head_hole_radius - screw_body_hole_radius, screw_head_hole_radius, screw_body_hole_radius);
            cylinder(1, screw_head_hole_radius, screw_head_hole_radius);
        }
    }

    union() {
        difference() {
            cylinder(button_bottom_thickness, button_radius + button_wall_thickness, button_radius + button_wall_thickness);
            cylinder(button_bottom_thickness, button_radius - button_wall_thickness, button_radius - button_wall_thickness);
        }
        difference() {
            translate([0, 0, button_bottom_thickness / 2]) cube([2 * button_radius, 2 * button_pillar_radius, button_bottom_thickness], center = true);
            screw_hole();
        }
    }
}

*top();
*bottom();
*button_top();
button_bottom();
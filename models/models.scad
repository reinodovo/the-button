include <../../ktane-3d-models/variables.scad>;
use <../../ktane-3d-models/bomb_base.scad>;

$fn = $preview ? 10 : 50;

tolerance = 0.3;

module top() {
    difference() {
        bomb_module_top(height_above_pcb = 14, module_type = 1);
    }
}

module bottom() {
    bomb_module_bottom(height_above_pcb = 14);
}

top();
bottom();

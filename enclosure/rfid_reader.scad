
// Designed for the Parallax RFID Reader 28140
// A rather too expensive module

// 62.2 x 82.5 x 5.57 mm
// Use 3mm walls
// 3.5mm screw holes
// 54.5mm bolt center-to-center distance
// 75mm bolt center-to-center distance y
// 1.6mm 

// #6 machine screw holes

width = 62.3;
height = 82.6;
side_space = 1;
board_depth = 1.6;
standoff = 2;
electronics_depth = 4;
electronics_height = 15;
electronics_width = 47;
bolt_diameter = 9.5;
bolt_thickness = 3;
bolt_hole_diameter = 3.7;
plate_thickness = 3;
bolt_length = 18;

// connector hole 2.54 x 10.12
connector_width = 11;
connector_depth = 3;
connector_offset = 8;

wall_thickness = 2.5;

// to prevent rounding problems
del = 0.1;
front_thickness = max(wall_thickness, bolt_thickness + 1);
depth = bolt_length - plate_thickness;
bolt_offset = bolt_hole_diameter / 2 + 2.25;
bolt_hole_locations = [
  [bolt_offset, bolt_offset, 0],
  [width - bolt_offset, bolt_offset, 0],
  [bolt_offset, height - bolt_offset, 0],
  [width - bolt_offset, height - bolt_offset, 0]];

module rfid_reader_enclosure() {

  difference() {
    translate([-wall_thickness - side_space, -wall_thickness - side_space])
      cube([width + 2 * (wall_thickness + side_space), height + 2 * (wall_thickness + side_space), depth]);

    // void for the board and electronics
    difference() {
      translate([-side_space, -side_space, front_thickness])
        cube([2 * side_space + width, 2 * side_space + height, depth - front_thickness + del]);
      // standoffs for the board
      for(location = bolt_hole_locations) {
        translate(location)
          cylinder(d = bolt_diameter, h = front_thickness + standoff);
      }
    }

    // void for the connectors
    translate([connector_offset, -side_space - wall_thickness - del, front_thickness + board_depth + standoff - 0.25])
      cube([connector_width, wall_thickness + 2 * del, connector_depth]);

    // Bolt holes
    for(location = bolt_hole_locations) {
      translate(location)
      translate([0, 0, -del]) {
        cylinder(d = bolt_hole_diameter, h = depth, $fn = 20);
        cylinder(d = bolt_diameter, h = bolt_thickness, $fn = 6);
      }
    }

    // essentially placed manually
    translate([0, 0, -del])
    linear_extrude(height = min(0.5, wall_thickness / 2)) {
      translate([55, electronics_height, 0])
      scale(0.5)
      mirror([1, 0, 0])
      translate([-26, -18, 0])
        import("Hackrobot.dxf");
    }

  }
}

module rfid_spacer() {
  spacer_height = depth - front_thickness - standoff - board_depth;
  difference() {
    cylinder(d = bolt_diameter, h = spacer_height, $fn = 20);
    translate([0, 0, -del])
    cylinder(d = bolt_hole_diameter, h = spacer_height + 2 * del, $fn = 20);
  }
}

*rfid_reader_enclosure();

for(location = bolt_hole_locations) {
  translate([0, 0, front_thickness + standoff + board_depth])
  translate(location)
    rfid_spacer();
}


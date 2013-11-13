////////////////////////////////////////////////////////////////////////////////
// Rounded cube things
////////////////////////////////////////////////////////////////////////////////

DISABLE_ROUND_CUBE = false;

module round_cube(s, rs) {
	if (DISABLE_ROUND_CUBE) {
		cube(s);
	} else {
		hull() {
			for (x = [rs[0], s[0]-rs[0]]) {
				for (y = [rs[1], s[1]-rs[1]]) {
					for (z = [rs[2], s[2]-rs[1]]) {
						translate([x,y,z])
							scale([rs[0]/5, rs[1]/5, rs[2]/5])
								sphere(r = 5);
					}
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// Halves of a hollow container
////////////////////////////////////////////////////////////////////////////////

// size: The size of the box [w,h,t]
// wall_thickness: Thicknesses of the walls of the box
// joint_size: The size of the joint
// joint_tollerance: Size of a gap added as extra around the join.
// female: Is this the female part of the box?
// radius: The radius of the rounding of the corners
module box_half(size, wall_thickness, joint_size, joint_tollerance, female, radius) {
	// Half is in the top, half in the bottom
	joint_tollerance = joint_tollerance / 2;
	
	union() {
		// The box itself
		difference() {
			round_cube([size[0], size[1], size[2]+radius], [radius,radius, radius]);
			
			// Chop the rounded corners off
			translate([0,0,size[2]])
				cube([size[0], size[1], radius]);
			
			
			// Hollow out the inside
			translate([wall_thickness, wall_thickness, wall_thickness])
				cube([ size[0] - (wall_thickness*2)
				     , size[1] - (wall_thickness*2)
				     , size[2]
				     ]);
		}
		
		// Create the joint
		if (!female) {
			difference() {
				translate([wall_thickness/3 + joint_tollerance/4, wall_thickness/3 + joint_tollerance/4, size[2]])
					cube([ size[0] - (2*(wall_thickness/3)) - joint_tollerance/2
					     , size[1] - (2*(wall_thickness/3)) - joint_tollerance/2
					     , joint_size
					     ]);
				
				translate([2*(wall_thickness/3) - joint_tollerance/4, 2*(wall_thickness/3) - joint_tollerance/4, size[2] - 1])
					cube([ size[0] - (4*(wall_thickness/3)) + joint_tollerance/2
					     , size[1] - (4*(wall_thickness/3)) + joint_tollerance/2
					     , joint_size + 2
					     ]);
			}
		} else {
			union() {
				// Inner lip
				difference() {
					translate([2*(wall_thickness/3) + joint_tollerance/4, 2*(wall_thickness/3) + joint_tollerance/4, size[2]])
						cube([ size[0] - (4*(wall_thickness/3)) - joint_tollerance/2
						     , size[1] - (4*(wall_thickness/3)) - joint_tollerance/2
						     , joint_size
						     ]);
					
					translate([wall_thickness, wall_thickness, size[2] - 1])
						cube([ size[0] - 2*wall_thickness
						     , size[1] - 2*wall_thickness
						     , joint_size + 2
						     ]);
					
				}
				
				// Outer Lip
				difference() {
					intersection() {
						translate([0,0,size[2]-radius])
							round_cube([size[0],size[1],joint_size+(radius*2)], [radius,radius,radius]);
						translate([0,0,size[2]])
							cube([size[0],size[1],joint_size]);
					}
					
					translate([wall_thickness/3 - joint_tollerance/4, wall_thickness/3 - joint_tollerance/4, size[2]-1])
						cube([ size[0] - (2*(wall_thickness/3))  + joint_tollerance/2
						     , size[1] - (2*(wall_thickness/3))  + joint_tollerance/2
						     , joint_size + 2
						     ]);
				}
			}
		}
	}
}

//translate([0,0,0])
//	box_half(size = [30,30,10], wall_thickness = 5, joint_size = 4, joint_tollerance = 0.5, female = false, radius = 3);
//translate([40,0,0])
//	box_half(size = [30,30,10], wall_thickness = 5, joint_size = 4, joint_tollerance = 0.5, female = true, radius = 3);



////////////////////////////////////////////////////////////////////////////////
// Clips and their cutout
////////////////////////////////////////////////////////////////////////////////

// width: The width of the clip
// thickness: The thickness of the shaft
// shaft_length: The length of the straight section of the clip
// clip_length: The length of the angled clip section (additional to the shaft_length)
// clip_protrusion: The distance the point of the clip sticks out from the shaft.
// clip_tollerance: The additional space added around a clip to account for manufacturing errors.
module clip(width, thickness, shaft_length, clip_length, clip_protrusion, clip_tollerance) {
	width = width + clip_tollerance;
	translate([-clip_tollerance/2, 0,0])
		union() {
			// Shaft
			cube([width, thickness + clip_tollerance, shaft_length - clip_tollerance]);
			
			// Clip
			translate([0,0,shaft_length - clip_tollerance]) {
				difference() {
					cube([width, thickness + clip_protrusion + clip_tollerance, clip_length + 2*clip_tollerance]);
				
					translate([-1,0,clip_length + clip_tollerance*2])
						rotate(a=[-atan((clip_length+2*clip_tollerance)/(thickness + clip_protrusion + clip_tollerance)),0,0])
							cube([width+2, sqrt(pow(thickness + clip_protrusion + clip_tollerance, 2) + pow(clip_length + 2*clip_tollerance,2)), clip_length]);
				}
			}
		}
}


size = [30,30,10];
wall_thickness = 7;
joint_size = 4;
joint_tollerance = 0.5;
radius = 3;
clip_thickness = (wall_thickness/3) - (joint_tollerance/4);
shaft_length = 3.5;
clip_length = 5;
clip_protrusion = 1.5;
clip_width = 7;
pos_clip_tollerance = 0.0;
neg_clip_tollerance = 0.5;

translate([0,0,0]) {
	difference() {
		box_half( size = size, wall_thickness = wall_thickness
		        , joint_size = joint_size, joint_tollerance = joint_tollerance
		        , female = false, radius = radius
		        );
		// Left clip
		translate([(size[0]/2) - (clip_width/2),0, size[2]])
			mirror([0,0,1])
				clip( width = clip_width, thickness = clip_thickness
				    , shaft_length = shaft_length, clip_length = clip_length
				    , clip_protrusion = clip_protrusion, clip_tollerance = neg_clip_tollerance);
		// Right clip
		translate([(size[0]/2) - (clip_width/2), size[1], size[2]])
			mirror([0,0,1])
			mirror([0,1,0])
				clip( width = clip_width, thickness = clip_thickness
				    , shaft_length = shaft_length, clip_length = clip_length
				    , clip_protrusion = clip_protrusion, clip_tollerance = neg_clip_tollerance);
	}
}

translate([40,0,0]) {
	union() {
		box_half( size = size, wall_thickness = wall_thickness
		        , joint_size = joint_size, joint_tollerance = joint_tollerance
		        , female = true, radius = radius
		        );
		// Left clip
		translate([(size[0]/2) - (clip_width/2),0, size[2] + joint_size])
			clip( width = clip_width, thickness = clip_thickness
			    , shaft_length = shaft_length, clip_length = clip_length
			    , clip_protrusion = clip_protrusion, clip_tollerance = pos_clip_tollerance);
		// Right clip
		translate([(size[0]/2) - (clip_width/2), size[1], size[2] + joint_size])
			mirror([0,1,0])
				clip( width = clip_width, thickness = clip_thickness
				    , shaft_length = shaft_length, clip_length = clip_length
				    , clip_protrusion = clip_protrusion, clip_tollerance = pos_clip_tollerance);
	}
}


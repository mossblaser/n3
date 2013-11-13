////////////////////////////////////////////////////////////////////////////////
// Compartment Cover
////////////////////////////////////////////////////////////////////////////////

// size: the size of the cover itself
// tab_size: the size of the tabs on the bottom corners of the cover
// clip_spring_size: the 2D size of the cut-out spring gap at the top of the cover
// clip_size: the size of the clip tab at the top of the cover
module cover(size, tab_size, clip_spring_size, clip_size) {
	difference() {
		// Main body of the cover
		union () {
			// Cover
			cube(size);
			
			// Tabs
			translate([0,-tab_size[1],0])
				cube(tab_size);
			translate([size[0] - tab_size[0],-tab_size[1],0])
				cube(tab_size);
			
			// The clip
			translate([size[0]/2 - clip_size[0]/2, size[1], 0])
				cube(clip_size);
		}
		
		// The cutout for the spring
		translate([size[0]/2 - clip_spring_size[0]/2, size[1] - 1.5*clip_spring_size[1], -1])
			cube([clip_spring_size[0], clip_spring_size[1], size[2] + 2]);
	}
}


// size: the size of the cover itself
// tab_size: the size of the tabs on the bottom corners of the cover
// clip_size: the size of the clip tab at the top of the cover
// tollerance: Added to all internal dimensions
module cover_cut_away(size, tab_size, clip_size, tollerance) {
	size = [ size[0] + tollerance
	       , size[1] + tollerance
	       , size[2] + tollerance/2
	       ];
	tab_size = [ tab_size[0] + tollerance
	           , tab_size[1] + tollerance
	           , tab_size[2] + tollerance
	           ];
	clip_size = [ clip_size[0] + tollerance
	            , clip_size[1] + tollerance
	            , clip_size[2] + tollerance
	            ];
	translate([-tollerance/2, -tollerance/2, 0])
		union () {
			// Cover
			cube(size);
			
			// Tabs
			translate([0,-tab_size[1],0])
				cube(tab_size);
			translate([size[0] - tab_size[0],-tab_size[1],0])
				cube(tab_size);
			
			// The clip
			translate([size[0]/2 - clip_size[0]/2, size[1], 0])
				cube(clip_size);
		}
}


////////////////////////////////////////////////////////////////////////////////
// Experimentation
////////////////////////////////////////////////////////////////////////////////

cover_size = [20,20,2];
cover_tab_size = [5,3,0.6];
cover_clip_spring_size = [15,2];
cover_clip_size = [5,1.5,0.6];
cover_tollerance = 2.0;

//cover( size = cover_size, tab_size = cover_tab_size
//     , clip_spring_size = cover_clip_spring_size
//     , clip_size = cover_clip_size
//     );
//
translate([30,0,0])
	translate([8,8,3])
		difference() {
			translate([-8,-8,-3])
				cube([ cover_size[0] + 16
				     , cover_size[1] + 16
				     , cover_size[2] + 4
				     ]);
			
			cover_cut_away( size = cover_size, tab_size = cover_tab_size
			              , clip_size = cover_clip_size
			              , tollerance = cover_tollerance
			              );
			translate([-cover_tollerance/2,-cover_tollerance/2,-cover_size[2]])
				cube([cover_size[0]+cover_tollerance,cover_size[1]+cover_tollerance,cover_size[2]+10]);
		}

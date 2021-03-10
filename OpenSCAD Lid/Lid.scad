include <Chamfer.scad>;

$fn=15;
generator_color=100;

top_size=110; //110
bottom_outer_size=79.5;
bottom_outer_height=1;

bottom_inner_size=79.5;
bottom_inner_hole_size=65.0;
bottom_inner_height=2.5;

edge_radius=2;

stl_offset=0.1;
stl_files=5;
stl_name=str("PathCow"); //_NUMBER.svg will be automatically added
stl_colors=["black","red","blue","green","yellow","gray"];
stl_overlap=0.005;


upper_cover=0.35;
upper_min_height=5;
upper_start_height=upper_min_height - stl_files*stl_offset-stl_offset;
upper_filler= upper_min_height - upper_start_height;

screw_hole_lid_radius=2/2;
screw_hole_radius=1.6/2;
screw_hole_height=10;


if(generator_color == -1) {
 get_inner();
}
else if(generator_color < 100) {
 get_inner_part(generator_color);
}

if(generator_color == -1 || generator_color == 100){
 difference(){  
  get_body_upper();
  get_screw_hole(-5,screw_hole_radius);
 }
}

if(generator_color == -1 || generator_color == 101){
 difference(){  
  get_middle_lower_body();
  get_screw_hole(-5,screw_hole_radius);
 }
}

if(generator_color == -1 || generator_color == 102){
 difference(){  
  get_lower_lid();
  get_lower_lid_hole();
 }
 
}

module get_body_upper() {
 get_upper_body();
}

module get_middle_lower_body() {
 get_middle_body();
 get_lower_body();  
}

module get_inner() {
 for (i=[0:stl_files-1])
 get_inner_part(i);
}

module get_inner_part(i) {
 act_height = upper_start_height+stl_offset*i-stl_overlap/2;
 stl_instance = str(stl_name, "_", i, ".svg");
 echo(stl_instance);
 color(stl_colors[i])
  translate([0, 0, act_height])
   linear_extrude(height = stl_offset+stl_overlap)
    import(file = stl_instance, center = false, dpi = 96);
}


module get_upper_body() {
 difference() {
  translate([-top_size/2, -top_size/2, upper_start_height])
   roundedcube(top_size,top_size,stl_offset*stl_files+upper_cover,edge_radius);
  get_inner();
 }
}

module get_middle_body() {
  translate([-top_size/2, -top_size/2, 0])
   roundedcube(top_size,top_size,upper_start_height,edge_radius);
}

module get_lower_body(){
 translate([-bottom_outer_size/2, -bottom_outer_size/2, -bottom_outer_height])
 roundedcube(bottom_outer_size,bottom_outer_size,bottom_outer_height,edge_radius);
}

module get_lower_lid(){ 
 difference(){  
  translate([-bottom_inner_size/2, -bottom_inner_size/2, -bottom_outer_height-bottom_inner_height])
  roundedcube(bottom_inner_size,bottom_inner_size,bottom_inner_height,edge_radius); 

  get_screw_hole(-5,screw_hole_lid_radius);
 }
}

module get_lower_lid_hole(){    
 translate([-bottom_inner_hole_size/2, -bottom_inner_hole_size/2, -bottom_outer_height-bottom_inner_height-0.5])
  cube([bottom_inner_hole_size,bottom_inner_hole_size,bottom_inner_height+1]); 
}

module get_screw_hole(height, radius){
mirror([0,0,0])
 translate([bottom_inner_size/2+height, bottom_inner_size/2+height, +height])
  chamferCylinder(r=radius, r2=radius, h=screw_hole_height, ch=-2.5, ch2=2);

mirror([1,0,0])
 translate([bottom_inner_size/2+height, bottom_inner_size/2+height, +height])
  chamferCylinder(r=radius, r2=radius, h=screw_hole_height, ch=-2.5, ch2=2);

mirror([0,1,0])
 translate([bottom_inner_size/2+height, bottom_inner_size/2+height, +height])
  chamferCylinder(r=radius, r2=radius, h=screw_hole_height, ch=-2.5, ch2=2);

mirror([1,1,0])
 translate([bottom_inner_size/2+height, bottom_inner_size/2+height, +height])
  chamferCylinder(r=radius, r2=radius, h=screw_hole_height, ch=-2.5, ch2=2);

}


//by Chris Bate https://www.youtube.com/watch?v=gKOkJWiTgAY
module roundedcube(xdim ,ydim ,zdim,rdim){
hull(){
translate([rdim,rdim,0])cylinder(h=zdim,r=rdim);
translate([xdim-rdim,rdim,0])cylinder(h=zdim,r=rdim);

translate([rdim,ydim-rdim,0])cylinder(h=zdim,r=rdim);
translate([xdim-rdim,ydim-rdim,0])cylinder(h=zdim,r=rdim);
}
}

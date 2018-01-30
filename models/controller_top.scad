$fn=30;

thickness=2;
widthSCD=77.7;
depthSCD=45.7;

spacingLeft=3;
spacingRight=2;
spacingDown=3;
spacingUp=4;

spacingPiLeft=0.5;
spacingPiUp=4;

thicknessBoard=1.5;


heightFitting=4;
heightEncasing=21;

heightBoltHolder=8;
radiusBoltHolder=2.5;

lengthAirVent=30;
widthAirVent=2;
distanceAirVents=2;


module topPlate(height,width,depth){
    difference(){
        cube([depth,width,height]);
        translate([thickness,thickness,thickness]){
            cube([depth-2*thickness,width-2*thickness,height]);
        }
         
        //fitting cutout
        translate([0,0,heightEncasing+thickness-heightFitting]){
             difference(){
                translate([-2,-2,0]){
                 cube([depth+4,width+4,10]);   
                }
                
                  
                translate([3*thickness/4,3*thickness/4,0]){
                    cube([depth-(3/2)*thickness,width-(3/2)*thickness,10+1]);
                }
            }   
        }
        
        //mini usb cutout
        translate([depth-2*thickness,thickness+spacingPiUp+6,         heightBoltHolder+thickness-3.5]){
          cube([thickness*4,9,3+1]);
        }
        
        translate([depth-2*thickness,thickness+spacingPiUp+19,heightBoltHolder+thickness-3.5]){
          cube([thickness*4,9,3+1]);
        }
        
        //air vent cutout
        
        airVent();
    
    }
}

module boltHolder(height,outer_radius,inner_radius,depth){
     translate([0,0,height/2]){
        difference(){
            cylinder(h=height,r=outer_radius,center=true);
            translate([0,0,height-depth]){
                cylinder(h=height,r=inner_radius,center=true);
            }   
        }
     }
}

module airVent(){
    translate([8,42,-thickness/2]){
       translate([0,0,0]){
            cube([widthAirVent,lengthAirVent,2*thickness]);
        }
        
     translate([widthAirVent+distanceAirVents,0,0]){
            cube([widthAirVent,lengthAirVent,2*thickness]);
        }
        
     translate([2*widthAirVent+2*distanceAirVents,0,0]){
            cube([widthAirVent,lengthAirVent,2*thickness]);
        }
     
     translate([3*widthAirVent+3*distanceAirVents,0,0]){
            cube([widthAirVent,lengthAirVent,2*thickness]);
        }
        
     translate([4*widthAirVent+4*distanceAirVents,0,0]){
            cube([widthAirVent,lengthAirVent/2,2*thickness]);
        }
     
     translate([5*widthAirVent+5*distanceAirVents,0,0]){
            cube([widthAirVent,lengthAirVent/2,2*thickness]);
        } 
        
    }
    
    
    
}


union(){
  width=widthSCD+spacingDown+spacingUp+2*thickness;   //y
  depth=depthSCD+spacingLeft+spacingRight+2*thickness; //x
  
  topPlate(heightEncasing+thickness,width,depth); 
    
  translate([depth-thickness-3.4-spacingPiLeft,thickness+3.4+spacingPiUp,thickness-1]){
    boltHolder(heightBoltHolder+1,radiusBoltHolder,1,4);  
  }
   
  translate([depth-thickness-3.4-spacingPiLeft-22.75,thickness+3.4+spacingPiUp,thickness-1]){
    boltHolder(heightBoltHolder+1,radiusBoltHolder,1,4);  
  }
  
  translate([depth-thickness-3.4-spacingPiLeft,thickness+3.4+spacingPiUp+57.7,thickness-1]){
    boltHolder(heightBoltHolder+1,radiusBoltHolder,1,4);  
  }
  
  translate([depth-thickness-3.4-spacingPiLeft-22.75,thickness+3.4+spacingPiUp+57.7,thickness-1]){
    boltHolder(heightBoltHolder+1,radiusBoltHolder,1,4);  
  }
   
}







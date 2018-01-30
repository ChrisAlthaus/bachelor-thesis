thickness=2;
widthSCD=77.7;
depthSCD=45.7;

spacingLeft=3;
spacingRight=2;
spacingDown=3;
spacingUp=4;

heightBottom=24+6;

thicknessBoard=1.6;
heightBoltHolder=5;

heightConnectorHole=9+0.5;
widthConnectorHole=33+0.5;

widthPowerSupplyHole=9+1;
heightPowerSupplyHole=11+1;

heightFitting=4;

spacingPiLeft=0.5;
spacingPiUp=4;

heightBoltHolder=5;
radiusBoltHolder=2.5;

heightPi=23.4;

width=widthSCD+spacingDown+spacingUp+2*thickness;   //y
depth=depthSCD+spacingLeft+spacingRight+2*thickness; //x


module bottomPlate(height,width,depth){
    difference(){
        cube([depth,width,height]);
        translate([thickness,thickness,thickness]){
            cube([depth-2*thickness,width-2*thickness,height]);
        }
        
        //10x2 connector hole
        translate([depth-thickness-widthConnectorHole-1,width-thickness-       spacingUp,thickness+heightBoltHolder+thicknessBoard]){
            cube([widthConnectorHole,2*thickness + spacingUp,                   heightConnectorHole]);
        }
        
        //power supply hole
        translate([depth-spacingRight-2*thickness,thickness+        spacingDown+20,thickness+heightBoltHolder+              thicknessBoard]){
            cube([4*thickness,widthPowerSupplyHole,                        heightPowerSupplyHole]);
            }
        
        //fitting cutout
        translate([thickness/2,thickness/2,heightBottom-heightFitting]){
            cube([depth-thickness,width-thickness,heightFitting+1]);
        }
        
        //raspberry pi mini usb 
        translate([depth-2*thickness,width-thickness-spacingPiUp-6.5,
            thickness+thicknessBoard+heightBoltHolder+heightPi]){
          cube([thickness*4,7.5,3+1]);
        }
        
        translate([depth-2*thickness,width-thickness-spacingPiUp-6.5-7.5-4.2,         thickness+thicknessBoard+heightBoltHolder+heightPi]){
          cube([thickness*4,7.5,3+1]);
        }
        
        //switch on/off
         translate([-1,thickness+spacingDown+42,thickness+          heightBoltHolder+thicknessBoard+5.5]){
            cube([thickness*4,12,6]);
        }
        
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


union(){
    
    
    //bolt holder for mounting the board
    bottomPlate(heightBottom,width,depth);
    translate([depth-thickness-spacingRight-35.5,thickness+spacingDown+5.7,thickness-1]){
       boltHolder(heightBoltHolder+1,radiusBoltHolder,1,3); 
    }   
    
    translate([depth-thickness-spacingRight-5,thickness+spacingDown+5.5,thickness-1]){
       boltHolder(heightBoltHolder+1,radiusBoltHolder,1,3); 
    } 
    
    translate([depth-thickness-spacingRight-15,thickness+spacingDown+60.5,thickness-1]){
       boltHolder(heightBoltHolder+1,radiusBoltHolder,1,3); 
    }
}



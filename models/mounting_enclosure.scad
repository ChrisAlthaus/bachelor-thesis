widthStepperMotor=35+1.4;
depthStepperMotor=35+1.4;
heightStepperMotor=20;
sideSpace=1.8;

distanceHolesStepperMotor=26;
offsetHoles=4.5+sideSpace/2;
radiusMountingHoles=1.65; 

widthBasePane=80;
depthBasePane=80;
thicknessBasePane=4; //not greater than 7.6

thicknessMountingPlate=6.4; //1.6
thicknessScrewPlate=1.6;


thickness=4;
thicknessInner=4;
thicknessOuter=2.7;
radiusScrewHoles=5.5;

heightAttachment=44;

wireHoleHeight=34;
wireHoleWidth=12;

screwHoleSideWidth=17;
screwHoleSideHeight=10;
screwHoleSideDepth=1;

module mountingPlate(width,depth){
   
    
    
        difference(){
            union(){
                cube([width,depth,thicknessMountingPlate]);
                
                
                translate([(width-widthBasePane)/2,(depth-depthBasePane)/2,0]){
                  cube([widthBasePane-sideSpace,depthBasePane-sideSpace,thicknessBasePane]);  
                }
                
                translate([0,0,thicknessMountingPlate]){
                    cube([width,depth,thicknessScrewPlate]); 
                }  
                innerWall();
                outerWall();
                
               
            }
            
            //Screw Hole Side
            translate([thickness+widthStepperMotor/2-screwHoleSideWidth/2-1,-screwHoleSideDepth,thicknessMountingPlate+thicknessScrewPlate+heightAttachment-screwHoleSideHeight-1]){
                    cube([screwHoleSideWidth,thickness*2,2*screwHoleSideHeight]); 
                } 
            
            //WireHole
            translate([-2*thickness,thickness+widthStepperMotor/2-wireHoleWidth/2,thicknessMountingPlate+thicknessScrewPlate]){
                    cube([4*thickness,wireHoleWidth,wireHoleHeight]); 
                } 
            
         
           //Mounting holes
          translate([thickness+offsetHoles,                   thickness+offsetHoles,thicknessMountingPlate-1]){
                cylinder(h=2*thickness + thicknessMountingPlate +1,r=                       radiusMountingHoles);  
          }
          
          translate([thickness+offsetHoles+distanceHolesStepperMotor,              thickness+offsetHoles,thicknessMountingPlate-1]){
                cylinder(h=2*thickness + thicknessMountingPlate +1,r=radiusMountingHoles);  
          }
          
          translate([thickness+offsetHoles,                   thickness+offsetHoles+distanceHolesStepperMotor,thicknessMountingPlate-1]){
                cylinder(h=2*thickness + thicknessMountingPlate +1,r=                       radiusMountingHoles);  
          }
          
          translate([thickness+offsetHoles+distanceHolesStepperMotor,              thickness+offsetHoles+distanceHolesStepperMotor,thicknessMountingPlate-1]){
                cylinder(h=2*thickness + thicknessMountingPlate +1,r=radiusMountingHoles);  
          }
          
          //Screw holes  
          translate([thickness+offsetHoles,                   thickness+offsetHoles,-1]){
                cylinder(h=thicknessMountingPlate+1,r=                       radiusScrewHoles);  
          }
          
          translate([thickness+offsetHoles+distanceHolesStepperMotor,              thickness+offsetHoles,-1]){
                cylinder(h=thicknessMountingPlate +1,r=radiusScrewHoles);  
          }
          
          translate([thickness+offsetHoles,                   thickness+offsetHoles+distanceHolesStepperMotor,-1]){
                cylinder(h=thicknessMountingPlate+1,r=radiusScrewHoles);  
          }
          
           translate([thickness+offsetHoles+distanceHolesStepperMotor,              thickness+offsetHoles+distanceHolesStepperMotor,-1])
          {
                cylinder(h=thicknessMountingPlate+1,r=radiusScrewHoles);  
          }
            
    }
    
}

module innerWall(){
    //inner walls
    
    translate([0,0,thicknessMountingPlate]){
        cube([thickness,widthStepperMotor+2*thickness,heightStepperMotor +          thickness]);   
    }
    
     translate([widthStepperMotor+thickness,0,thicknessMountingPlate]){
        cube([thickness,widthStepperMotor+2*thickness,heightStepperMotor +          thickness]);   
    }
    
    
     translate([widthStepperMotor+2*thickness,depthStepperMotor+thickness,thicknessMountingPlate]){
        rotate(a=90){
             cube([thickness,widthStepperMotor+2*thickness,heightStepperMotor +          thickness]); 
         }
     }
     
     
     translate([widthStepperMotor+2*thickness,0,thicknessMountingPlate]){
        rotate(a=90){
             cube([thickness,widthStepperMotor+2*thickness,heightStepperMotor +thickness]); 
         }
     }
    
}
    
module outerWall(){
 //outer walls
    translate([-thicknessOuter+sideSpace/2,-thicknessOuter,0]){
        cube([thicknessOuter,widthStepperMotor+2*thickness+2*thicknessOuter-sideSpace,thicknessScrewPlate+thicknessMountingPlate+heightAttachment]);   
    }
    
     translate([widthStepperMotor+2*thickness-sideSpace/2,-thicknessOuter,0]){
        cube([thicknessOuter,widthStepperMotor+2*thickness+2*thicknessOuter-sideSpace,thicknessScrewPlate+thicknessMountingPlate+         heightAttachment]);   
    }
    
    
     translate([widthStepperMotor+2*thickness+thicknessOuter-sideSpace/2,-thicknessOuter,0]){
        rotate(a=90){
             cube([thicknessOuter,widthStepperMotor+2*thickness+2*thicknessOuter-sideSpace,thicknessScrewPlate+thicknessMountingPlate+heightAttachment]); 
         }
     }
     
     
     translate([widthStepperMotor+2*thickness+thicknessOuter-sideSpace/2,widthStepperMotor+2*thickness -1.4,0]){
        rotate(a=90){
             cube([thicknessOuter,widthStepperMotor+2*thickness+2*thicknessOuter-sideSpace,thicknessScrewPlate+thicknessMountingPlate+heightAttachment]); 
         }
     }
 
}   
     
     
    



union(){
    mountingPlate(widthStepperMotor+2*thickness,depthStepperMotor+2*thickness);
    
    
}

import processing.serial.*;
import processing.net.*; 


final int hdispY_pos=307;


Serial myPort;  // Create object from Serial class
float val;      // Data received from the serial port


float angle = 0;
float increment = 0.1;
float time = 1000;
float timeincrement = 0.01;
int xsize = 200;
int ysize = 200;
int posX = 2024;
int posY = 2024;
int fact = 8;
int chanel = 4;
int laser = 3;
int init_flags=1;
int displayX=250;
int displayY=50;

boolean auto_norm=true;
boolean LUT=true;
boolean invert=true;

int box0=fact-1;
final int box0_size=10;
final int boxX0=20;
final int boxY0=hdispY_pos+124;


int box1=chanel-1;
int box1_size=5;
int boxX1=20;
int boxY1=hdispY_pos+320;

int box2=laser;
int box2_size=4;
int boxX2=20;
int boxY2=boxY1+1*60;

int box3=int(invert);
int box3_size=2;
int boxX3=20;
int boxY3=boxY1+2*60;

int box4=int(LUT);
int box4_size=2;
int boxX4=80;
int boxY4=boxY1+2*60;

int box5=int(auto_norm);
int box5_size=2;
int boxX5=20;
int boxY5=boxY1+3*60;

int box6=1;
int box6_size=3;
int boxX6=150;
int boxY6=hdispY_pos+55;

int butX=180;
final int butY=hdispY_pos+240;

color  rectColor = color(0,0,0);
color  rectHighlight = color(0,180,0);
int boxSize = 21;     // Diameter of rect


int csize = 10;
float[][] data = new float[xsize+1][ysize+1];
float[] control_data_in = new float[csize];
float[] control_data_out = new float[csize];

float z_stage;
float z_stage_zero=2216; 
float focus;
float focus_zero=1205; 

int range_high;
int range_low;

float z_factor=1.1; //tics*z_factor = mm
float xy_factor=72; //rics*x_factor = um

float norm_val;


void setup() {

control_data_out[0]=xsize;
control_data_out[1]=ysize;
control_data_out[2]=posX;
control_data_out[3]=posY;
control_data_out[4]=fact;
control_data_out[5]=chanel;
control_data_out[6]=init_flags;
control_data_out[7]=laser;
 
  size(1540, 960, P3D);


  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);

while ( myPort.available() >0) {val = myPort.read();};
   
 for (int i=0; i < csize; i++){
  myPort.write(byte(control_data_out[i]/256)); 
  myPort.write(byte(control_data_out[i]%255)); 
 }
 
 init_flags=0;
 control_data_out[6]=init_flags;
 
 } //setup


int x = 0;
int y = 0;


void draw() {
  background(0);
  stroke(0);
 // fill(100,100);

    update(mouseX, mouseY);
// rotateY((float)mouseX/100);

//


if ( myPort.available() >= ((xsize*2)+(csize*2))) {

 for (int i=0; i < csize; i++){
   val = myPort.read();         // read Control Data
  val = val+myPort.read()*256;
  control_data_in[i] = val;
 }
 
for (int i=0; i < xsize; i++){

  val = myPort.read();         // read Image Data
  val = val+myPort.read()*256;

  data[xsize-i][y] = (val);
//println(val);
     }

y++;

while ( myPort.available() >0) {val = myPort.read();}; 

 control_data_out[5]=chanel;
 control_data_out[7]=laser;
 control_data_out[4]=fact;
 control_data_out[2]=posX;
 control_data_out[3]=posY;
 control_data_out[6]=init_flags;
 if (init_flags==1) init_flags=0;
 
 for (int i=0; i < csize; i++){
  myPort.write(byte(control_data_out[i]/256)); 
  myPort.write(byte(control_data_out[i]%256)); 

 }
 
if (y>ysize) {
  y=ysize;
  drawPlane(data, xsize, ysize,ysize/2);
saveFrame("dump/LaserDump "+nf(year(),4)+"-"+nf(month(),2)+"-"+nf(day(),2)+" "+nf(hour(),2)+":"+nf(minute(),2)+":"+nf(second(),2)+".png");
y=0; 
}

} //myPort Available
drawPlane(data, xsize, ysize,y-1);
  
} //draw

void drawPlane(float[][] vals, int sizeX, int sizeY,int indexY) {
  float rectsize = max(800, 800)/max(sizeX, sizeY);


      stroke(0,153,0);
      rect(displayX,displayY,sizeX*rectsize,sizeY*rectsize);
      
      line(displayX+sizeX*rectsize, 40, displayX+sizeX*rectsize-10000/xy_factor/fact*rectsize, 40); //draw 10um Scale
       fill(0, 255, 0);
        textSize(11);
      text("10 um",displayX+sizeX*rectsize-50,30); 

  range_low=1024;
  for (int i=1; i<sizeX-1; i++) 
    for (int j=0; j<y; j++) {
     range_high=max(range_high,int(vals[i][j]));
     range_low=min(range_low,int(vals[i][j])); 
    }
   
     
  //row by row
  for (int i=1; i<sizeX-1; i++) {
    for (int j=1; j<sizeY-1; j++) {

     //  stroke(255, 255, 255);
      noStroke();
      //  fill(0, (vals[i][j]-range_low)/(range_high-range_low)*255, 0);
   if (auto_norm) norm_val=(vals[i][j]-range_low)/(range_high-range_low)*1024; else norm_val=vals[i][j];
   if (invert) norm_val=1024-norm_val;
   
   if (LUT) {
     if (norm_val<255) fill(norm_val/2,0,norm_val); else
     if (norm_val<511) fill(norm_val/2,0,511-norm_val);else
     if (norm_val<767) fill(255,norm_val-511,0);else
      fill(255,255,norm_val-767);
   } else fill(0,norm_val/4,0);
     
      rect(displayX+i*rectsize,displayY+j*rectsize,rectsize,rectsize);
    }
  }
  if (indexY>0)
  {stroke(0,80,0);



  line(20,hdispY_pos,20+200,hdispY_pos);
  line(20,hdispY_pos-1024/4-1,20+200,hdispY_pos-1024/4-1);
  for (int i=1; i<200; i++) 
  {
       if (invert) line(20+i,hdispY_pos,20+i,hdispY_pos-(1024-vals[sizeX/200*i][indexY])/4); else 
       line(20+i,hdispY_pos,20+i,hdispY_pos-(vals[sizeX/200*i][indexY])/4);
  }
}
 
 textSize(11);
 fill(0, 255, 0);

z_stage=((z_stage_zero-control_data_in[2])*z_factor/1000);
focus= (control_data_in[1]-focus_zero)*z_factor/1000;

text("DVD Laser Scanner by GaudiLabs", 20, hdispY_pos-18*15); 

text("PositionX: "+posX, 20, hdispY_pos+2*15); 
text("PositionY: "+posY, 20, hdispY_pos+3*15); 
text("SizeX: "+nfc(sizeX*fact*xy_factor/1000,0)+"um ("+sizeX+")", 20, hdispY_pos+4*15); 
text("SizeY: "+nfc(sizeY*fact*xy_factor/1000,0)+"um ("+sizeY+")", 20, hdispY_pos+5*15); 
text("Factor: "+control_data_in[4], 20, hdispY_pos+7*15); 
text("Laser Intensity: "+control_data_in[0], 20, hdispY_pos+11*15); 
text("Reading Focus: "+nfc(focus,2)+" mm ("+control_data_in[1]+")", 20, hdispY_pos+12*15); 
text("Z-Stage: "+nfc(z_stage,2)+" mm ("+control_data_in[2]+")", 20, hdispY_pos+13*15); 
text("Gap: "+nfc(focus-z_stage,2)+" mm", 20,hdispY_pos+14*15); 

text("Focus Step: "+control_data_in[5], 20, hdispY_pos+16*15); 
text("Focus Shift: "+control_data_in[6], 20, hdispY_pos+17*15); 
text("reading: "+control_data_in[7], 20, hdispY_pos+18*15); 

text("Sensor: ", 20, hdispY_pos+20*15); 
text("Laser: ", 20, hdispY_pos+24*15); 
text("Colors: ", 20, hdispY_pos+28*15); 
text("Bightness: ", 20, hdispY_pos+32*15); 

textSize(11);

radioBut(box0,boxX0,boxY0,box0_size);
fill(0,230,0);
text("  1    2    3     4     5     6     7    8    9    10", boxX0+3, boxY0-5); 


radioBut(box1,boxX1,boxY1,box1_size);
fill(0,230,0);
text("A0  S1  S2   RF  DIF", boxX1+3, boxY1-5); 

radioBut(box2,boxX2,boxY2,box2_size);
fill(0,230,0);
text("OFF IR  RED  UV", boxX2+2, boxY2-5); 


radioBut(box3,boxX3,boxY3,box3_size);
fill(0,230,0);
text("NOM INV", boxX3+2, boxY3-5); 

radioBut(box4,boxX4,boxY4,box4_size);
fill(0,230,0);
text("GRE LUT", boxX4+2, boxY4-5); 

radioBut(box5,boxX5,boxY5,box5_size);
fill(0,230,0);
text("NOR AUTO", boxX5+2, boxY5-5); 

radioBut(box6,boxX6,boxY6,box6_size);
fill(0,230,0);
text("200  400  600", boxX6+2, boxY6-5); 

fact=box0+1;
chanel=box1+1;
laser=box2;
invert=boolean(box3); 
LUT=boolean(box4); 
auto_norm=boolean(box5); 
//xsize = 200+box6*200;

fill(0,230,0);
text("HOME", butX, butY-5); 
fill(rectHighlight);
   rect(butX,butY,30,30);


}

void mousePressed() {
  float rectsize = max(800, 800)/max(xsize, ysize);
   
  if (overRect(boxX0, boxY0, boxSize*box0_size, boxSize))  box0=checkRadio(boxX0,boxY0,box0_size);
  if (overRect(boxX1, boxY1, boxSize*box1_size, boxSize))  box1=checkRadio(boxX1,boxY1,box1_size);
  if (overRect(boxX2, boxY2, boxSize*box2_size, boxSize))  box2=checkRadio(boxX2,boxY2,box2_size);
  if (overRect(boxX3, boxY3, boxSize*box3_size, boxSize))  box3=checkRadio(boxX3,boxY3,box3_size);
  if (overRect(boxX4, boxY4, boxSize*box4_size, boxSize))  box4=checkRadio(boxX4,boxY4,box4_size);
  if (overRect(boxX5, boxY5, boxSize*box5_size, boxSize))  box5=checkRadio(boxX5,boxY5,box5_size);
  if (overRect(boxX6, boxY6, boxSize*box6_size, boxSize))  box6=checkRadio(boxX6,boxY6,box6_size);

  if (overRect(displayX,displayY,int(xsize*rectsize),int(ysize*rectsize)))
  {
  posX=int(posX-fact*((mouseX-displayX)/(rectsize)-xsize/2));
  posY=int(posY+fact*((mouseY-displayY)/(rectsize)-ysize/2));
  }
   if (overRect(butX,butY,30,30)) {
     y=0;
     init_flags=1;
   }
  
  
 // println((mouseX-displayX)/(rectsize))
}

int checkRadio(int boxX,int boxY, int box_size)
{int a=0;
 for (int i=0; i < box_size; i++){
    if (overRect(boxX+i*boxSize,boxY,boxSize,boxSize)) a=i ;
 }
 radioBut(a,boxX,boxY,box_size);

 return a;
}


void radioBut(int box,int boxX,int boxY, int box_size)
{
 for (int i=0; i < box_size; i++){
  
    if (i==box) fill(rectHighlight); else fill(rectColor);
   rect(boxX+i*boxSize, boxY, boxSize, boxSize);

 }
}


boolean overRect(int x, int y, int width, int height)  {
  if (mouseX >= x && mouseX <= x+width && 
      mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}

void update(int x, int y) {
 // if (overRect(100, 100, boxSize, boxSize)) {
  // println("go");
 // }
}
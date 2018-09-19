#include <bits/stdc++.h>

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cstring>
#include <string>

using namespace std;

GLfloat xRotated, yRotated, zRotated;
float size = 0.02f;
float limit = 1.0f;
map<int, map<string,float>> position;
map<int,float> vel1, vel2, vel3;
float axis_size = 2.0f;
float rel_dist = 0.1f;

// Distance

float sumx=0.0;
float sumy=0.0;
float sumz=0.0;

// Velocity

float sumv1=0.0;
float sumv2=0.0;
float sumv3=0.0;

// Acceleration

float suma1=0.0;
float suma2=0.0;
float suma3=0.0;

// Force

float force=0.0;

// Boundaries

float rad=3.0f;

// float xmin = -1.0*rad, ymin = -1.0*rad, zmin = -1.0*rad;
// float xmax =  1.0*rad, ymax =  1.0*rad, zmax =  1.0*rad;
float vr = 1.0;
int num = 0;

float energy = 0.0f;
int n = 0;

void wait ( float seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}

void print(int x, int y,int z, char *string)
{
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x,y);
    //get the length of the string to display
    int len = (int) strlen(string);

    //loop to display character by character
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
    }
};

void add_bird(){

    map<string,float> temp;

    temp["x"]=(((double) rand() / (RAND_MAX))-0.5)*limit;
    temp["y"]=(((double) rand() / (RAND_MAX))-0.5)*limit;
    temp["z"]=(((double) rand() / (RAND_MAX))-0.5)*limit;

    position[n]=temp;


    vel1[n]=(((double) rand() / (RAND_MAX))-0.5)*limit;
    vel2[n]=(((double) rand() / (RAND_MAX))-0.5)*limit;
    vel3[n]=(((double) rand() / (RAND_MAX))-0.5)*limit;

   
   n++;

}

// Boundary -- now not needed

void boundary(){

  for (int i = 1; i < n; ++i)
  {
    
    if(position[i]["x"] < -1*rad){
      vel1[i]=+0.1*vr;
    } else if(position[i]["x"] > 1*rad){
      vel1[i]=-0.1*vr;
    }

    if(position[i]["y"] < -1*rad){
      vel2[i]=+0.1*vr;
    } else if(position[i]["y"] > 1*rad){
      vel2[i]=-0.1*vr;
    }

    if(position[i]["z"] < -1*rad){
      vel3[i]=+0.1*vr;
    } else if(position[i]["z"] > 1*rad){
      vel3[i]=-0.1*vr;
    }

  }

}

// Rule 1

void attraction(){

    energy = 0.0f;
    num = n*4; // Energy factor

    for(int i=0; i<n; i++){

        energy+=(0.5*( vel1[i]*vel1[i] + vel2[i]*vel2[i] + vel3[i]*vel3[i] ));

    }

    for (int i=1; i<n; i++){

        sumx=0.0;
        sumy=0.0;
        sumz=0.0;

        for(int j=0; j<n; j++){

          float dist = sqrt( 
                    (position[i]["x"]-position[j]["x"])*(position[i]["x"]-position[j]["x"]) + 
                    (position[i]["y"]-position[j]["y"])*(position[i]["y"]-position[j]["y"]) + 
                    (position[i]["z"]-position[j]["z"])*(position[i]["z"]-position[j]["z"]) 
                    );

            if( dist<0.5 ){

              sumx+=(position[i]["x"]);
              sumy+=(position[i]["y"]);
              sumz+=(position[i]["z"]);

            }

        }

        vel1[i]+=( (sumx/n) - position[i]["x"] )/100.0;
        vel2[i]+=( (sumy/n) - position[i]["y"] )/100.0;
        vel3[i]+=( (sumz/n) - position[i]["z"] )/100.0;

        suma1+=( (sumx/n) - position[i]["x"] )/100.0;
        suma2+=( (sumy/n) - position[i]["y"] )/100.0;
        suma3+=( (sumz/n) - position[i]["z"] )/100.0;
    }

}

// Rule2

void repulsion(){

    for (int i=1; i<n; i++){
        for (int j=1; j<n; j++){

            float dist = sqrt( 
                    (position[i]["x"]-position[j]["x"])*(position[i]["x"]-position[j]["x"]) + 
                    (position[i]["y"]-position[j]["y"])*(position[i]["y"]-position[j]["y"]) + 
                    (position[i]["z"]-position[j]["z"])*(position[i]["z"]-position[j]["z"]) 
                    );

            if( dist<0.02 ){

               vel1[i] -= (position[i]["x"] - position[j]["x"])/2.0;
               vel2[i] -= (position[i]["y"] - position[j]["y"])/2.0;
               vel3[i] -= (position[i]["z"] - position[j]["z"])/2.0;

               suma1 -= (position[i]["x"] - position[j]["x"])/2.0;
               suma2 -= (position[i]["y"] - position[j]["y"])/2.0;
               suma3 -= (position[i]["z"] - position[j]["z"])/2.0;
            }
        }
    }

}

// Rule 3

void update_vel(){

    for(int i=0; i<n; i++){

        sumv1=0.0;
        sumv2=0.0;
        sumv3=0.0;

        for(int j=0; j<n; j++){

          float dist = sqrt( 
                    (position[i]["x"]-position[j]["x"])*(position[i]["x"]-position[j]["x"]) + 
                    (position[i]["y"]-position[j]["y"])*(position[i]["y"]-position[j]["y"]) + 
                    (position[i]["z"]-position[j]["z"])*(position[i]["z"]-position[j]["z"]) 
                    );

            if( dist<0.5 ){

              sumv1+=(vel1[i]);
              sumv2+=(vel2[i]);
              sumv3+=(vel3[i]);

            }
        }
        
        vel1[i]+=( (sumv1/n) - vel1[i] )/2.0;
        vel2[i]+=( (sumv2/n) - vel2[i] )/2.0;
        vel3[i]+=( (sumv3/n) - vel3[i] )/2.0;

        suma1+=( (sumv1/n) - vel1[i] )/8.0;
        suma2+=( (sumv2/n) - vel2[i] )/8.0;
        suma3+=( (sumv3/n) - vel3[i] )/8.0;


    }

}


// update

void update_pos(){

    for (int i = 1; i < n; ++i)
    {
        position[i]["x"]+=vel1[i];
        position[i]["y"]+=vel2[i];
        position[i]["z"]+=vel3[i];
    }

}


void update(){

    suma1 = 0.0;
    suma2 = 0.0;
    suma3 = 0.0;

    // boundary();
    attraction();
    repulsion();
    boundary();
    update_pos();

    force = suma1 + suma2 + suma3;

}

void idle_fn(){
    // wait(0.1);
    wait(0.1);

    glutPostRedisplay();

    // add_bird();

    update();
    
}

void init(void)
{

    glClearColor(0,0,0,0);
 
}

void display(void)
{

    glMatrixMode(GL_MODELVIEW);
    // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
        glTranslatef(0.0,0.0,-10.5);
        // glTranslatef(0.0,0.0,0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    // rotation about Y axis
    glRotatef(yRotated,0.0,1.0,0.0);
    // rotation about Z axis
    glRotatef(zRotated,0.0,0.0,1.0);
           // End Drawing The Cube

    string x1 = "# Boids = "+to_string(num);
    char* y1 = new char[x1.length() + 1];
    std::strcpy(y1, x1.c_str());

    string x2 = "Energy = "+to_string(energy*1000);
    char* y2 = new char[x2.length() + 1];
    std::strcpy(y2, x2.c_str());

    string x3 = "Avg. Force = "+to_string(force*1000/num);
    char* y3 = new char[x3.length() + 1];
    std::strcpy(y3, x3.c_str());

    string x4 = "Avg. Energy = "+to_string(energy*1000/(num));
    char* y4 = new char[x4.length() + 1];
    std::strcpy(y4, x4.c_str());

    print(-3, 3,0,y1);
    print(-3,-3,0,y2);
    print( 3, 3,0,y3);
    print( 3,-3,0,y4);



    glBegin(GL_LINES);
         glColor3f(0.0,0.0,0.0); 
         glVertex3f(position[0]["x"], position[0]["y"], position[0]["z"]);
         glVertex3f(position[0]["x"]+size, position[0]["y"]+size, position[0]["z"]+size);
    glEnd();

    glBegin(GL_LINES);
         glColor3f(0.0,0.0,0.0); 
         glVertex3f(position[0]["x"], position[0]["y"], position[0]["z"]);
         glVertex3f(position[0]["x"]-size, position[0]["y"]+size, position[0]["z"]+size);
    glEnd();



    for (int i = 1; i < n; ++i)
   {
      glBegin(GL_LINES);
         glColor3f(1.0,1.0,1.0); 
         glVertex3f(position[i]["x"], position[i]["y"], position[i]["z"]);
         glVertex3f(position[i]["x"]+size, position[i]["y"]+size, position[i]["z"]+size);
      glEnd();

      glBegin(GL_LINES);
         glColor3f(1.0,1.0,1.0); 
         glVertex3f(position[i]["x"], position[i]["y"], position[i]["z"]);
         glVertex3f(position[i]["x"]-size, position[i]["y"]+size, position[i]["z"]+size);
      glEnd();


      glBegin(GL_LINES);
         glColor3f(1.0,0.0,0.0); 
         glVertex3f(0.0f, -0.75f+0.0f, 0.0f);
         glVertex3f(axis_size/2.0, -0.75f+0.0f, 0.0f);
      glEnd();

      glBegin(GL_LINES);
         glColor3f(0.0,1.0,0.0); 
         glVertex3f(0.0f, -0.75f+0.0f, 0.0f);
         glVertex3f(0.0f, -0.75f+axis_size/2.0, 0.0f);
      glEnd();

      glBegin(GL_LINES);
         glColor3f(0.0,0.0,1.0); 
         glVertex3f(0.0f, -0.75f+0.0f, 0.0f);
         glVertex3f(0.0f, -0.75f+0.0f, axis_size/2.0);
      glEnd();


      // glBegin(GL_QUADS);        // Draw The Cube Using quads
      //   glColor3f(0.8f,0.8f,0.8f);    // Color Blue
      //   glVertex3f( 2.0f,-0.1f,  2.0f);    // Top Right Of The Quad (Top)
      //   glVertex3f(-2.0f,-0.1f,  2.0f);    // Top Left Of The Quad (Top)
      //   glVertex3f(-2.0f,-0.1f, -2.0f);    // Bottom Left Of The Quad (Top)
      //   glVertex3f( 2.0f,-0.1f, -2.0f);
      // glEnd();


   }

glFlush();
}


void animation(void)
{
 
     // yRotated += 0.01;
     // xRotated += 0.02;
    display();
}


void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
     
    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}

void Special_Keys (int key, int x, int y)
{
    switch (key) {
 
       case GLUT_KEY_LEFT :  yRotated -=1;  break;
       case GLUT_KEY_RIGHT:  yRotated +=1;  break;
       case GLUT_KEY_UP   :  xRotated -=2;  break;
       case GLUT_KEY_DOWN :  xRotated +=2;  break;
       case GLUT_KEY_HOME : add_bird(); break;
       case GLUT_KEY_PAGE_UP : rad+=0.02; break;
       case GLUT_KEY_PAGE_DOWN : rad-=0.02; break;

    }

    display();
 
    glutPostRedisplay();
}

int main(int argc, char** argv){

glutInit(&argc, argv);
//we initizlilze the glut. functions
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowPosition(0, 0);
glutInitWindowSize(1360, 740);   // Set the window's initial width & height
glutCreateWindow("COP290 Starling");
init();

   map<string,float> temp;
   // temp["x"]=0.0; temp["y"]=0.0; temp["z"]=-5.0;
   if(n>10){
     temp["x"]=(((double) rand() / (RAND_MAX))-0.5)/10;
     temp["y"]=(((double) rand() / (RAND_MAX))-0.5)/10;
     temp["z"]=-5.0;
     cout << "x : " << temp["x"] << ", y : " << temp["y"] << endl;
   } else {
    temp["x"]=-0.5;
    temp["y"]=0.5;
    temp["z"]=-5.0;
   }
   position[n]=temp;
   vel1[n]=1, vel2[n]=1, vel3[n]=1;
   n++;


glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutSpecialFunc  (Special_Keys);
//Set the function for the animation.
// glutIdleFunc(animation);
glutIdleFunc(idle_fn);
glutMainLoop();
return 0;



}
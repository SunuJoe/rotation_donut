#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define PI 3.141592
#define K2 5 

int main(int argc, char* argv[])
{
  float A = 0; // initalizing x-axis rad.
  float B = 0; // initalizing z-axis rad.
  int r1 = 1; // circle half.
  int r2 = 2; // torus half.

  float K1 = (80*K2*2)/(8*(r1+r2)); // 33
  float K1y = (22*K2*3)/(8*(r1+r2)); // 13

  char p[1760];
  float z[1760];
  printf("p size : %lu\n", sizeof(p));
  printf("z size : %lu\n", sizeof(z));

  printf("\x1b[2J");

  while (1)
  {
    memset(p, 32, sizeof(p));
    memset(z, 0, sizeof(z));
    
    for(float i=0; i<6.28; i+=0.03) // torus
    {
      for(float j=0; j<6.28; j+=0.02) // circle
      {
        //first circle x, y, z.
        float cosj = cos(j);
        float sinj = sin(j);
        float c_x = r2+(r1*cosj);
        float c_y = r1*sinj;
        //float c_z = 0; //TODO : it might be useless
       
        //second making torus by rotating circle.
        //y-axis rad.
        float cosi = cos(i);
        float sini = sin(i);
        float t_x = cosi*c_x;
        float t_y = c_y;
        float t_z = sini*c_x;

        //TODO : screen x, y position making.
        // A : x-axis rotate, B : z axis rotate.
        float cosA = cos(A);
        float sinA = sin(A);
        float cosB = cos(B);
        float sinB = sin(B);

        float r_x = (t_x*cosB)+(sinB*(t_y*cosA+t_z*sinA));
        float r_y = cosB*(t_y*cosA+t_z*sinA)-sinB*t_x;
        float r_z = K2 + t_z*cosA-sinA*t_y;
        float ooz = 1/r_z;

        //TODO :: let's find out the Z.
//        printf("r_z : %.4f ", r_z); 

        //luminance. ; inner/ dot product both nx, ny, nz and luminance source.
        //luminance source = (0, 1, -1) behind and above
        //lumin is square 2 max.
        float l_x = cosj * cosi;
        float l_y = sinj * cosA + cosj * sini * sinA;
        float l_z = cosj * sini * cosA -sinA * sinj;

        float n_x = l_x * cosB + l_y * sinB;
        float n_y = l_y * cosB -sinB * l_x;
        float n_z = l_z;

        float lumi = n_y - n_z;
        //float lumi = cosi*cosj*sinB-cosA*cosj*sini-sinA*sinj+cosB*(cosA*sinj-cosj*sinA*sini);
              
        // screen x, y plot.
        int x = 40+K1*r_x*ooz;
        int y = 12-K1y*r_y*ooz;

//        printf("x : %d\n", x);
//        printf("y : %d\n", y);
        int pos = x + 80 * y;
        //printf("pos : %d\n", pos);
        int index_lumi = 8*lumi;
        
        if(ooz > z[pos] && x > 0 && x < 80 && y > 0 && y < 22)
        {
          z[pos] = ooz;
          p[pos] = ".,-~:;=!*#$@"[index_lumi > 0 ? index_lumi : 0];
        }
        //p[pos] = 46;
      }
    }
   
    printf("\x1b[H");
    for(int k=0; k<1760; k++)
    {
      putchar(k % 80 ? p[k] : 10);
      A += 0.00004;
      B += 0.00002;
    }
    usleep(60000); 
  }
  return 0;
}




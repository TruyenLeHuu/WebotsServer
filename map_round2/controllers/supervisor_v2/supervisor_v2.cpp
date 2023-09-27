#include <webots/Supervisor.hpp>
#include <math.h>
#include <iostream>

#define TIME_STEP 8
#define OffSet 0.26 
const int numberOfCheckpoint = 5;
const int numberOfCheckline  = 30;
const int numberOfCircle     = 6;
using namespace webots;
using namespace std;
double Finish_time = 0;
double Start_time = 0;
bool Condition = false;

typedef struct {
  double x, z;
  double dx = 0;
  double dz = 0;
} Coord;


typedef struct {
  double Ia, Ib, R1, R2, x1, x2, z1, z2;
} Circle;

Circle Cir[numberOfCircle] = {{-0.5, 2.97, 0.04, 0.34, -0.69, -0.31, 2.97, 3.16},
                              {-0.12, 2.18, 0.04, 0.34, -0.31, 0.07, 2.18, 1.99},
                              {-0.5, 3.17, 0.42, 0.72, -1.07, 0.07, 3.17, 3.74},
                              {-1.26, 3.17, 0.04, 0.34, -1.45, -1.07, 3.17, 2.98},
                              {-1.26, 4.55, 0.04, 0.34, -1.45, -1.26, 4.55, 4.74},
                              {4, -0.1, 0.24, 0.54, 3.61, 4.39, -0.49, 0.29}};

Coord CP[numberOfCheckpoint ] = {{-0.69, 2.44, 0.081, 0.03 }, {-1.45, 3.91, 0.081, 0.03 }, {1.04, 3.55, 0.081, 0.03 },
                                 {3.06, -0.1, 0.03, 0.081}, {4.76, -0.1, 0.03, 0.081}};
// ngang  , 0.081, 0.03 
//   doc  , 0.03, 0.081 
Coord CL[numberOfCheckline] = {{-4.68, -0.84, 0.03, 0.081}, {-3.5, -0.84, 0.03, 0.081}, {-0.685994, 1.97412, 0.081, 0.03}, {-0.69, 2.97, 0.081, 0.03},
                             {-0.31, 2.97, 0.081, 0.03}, {-0.31, 2.18, 0.081, 0.03}, {0.07, 2.18, 0.081, 0.03}, {0.07, 3.17, 0.081, 0.03}, 
                             {-1.07, 3.17, 0.081, 0.03}, {-1.45, 3.17, 0.081, 0.03}, {-1.45, 4.55, 0.081, 0.03}, {-1.26, 4.74, 0.03, 0.081},
                             {-0.86, 4.74, 0.03, 0.081}, {-0.449882, 4.32994, 0.03, 0.081}, {-0.0539053, 4.72586, 0.03, 0.081}, {0.342073, 4.32994, 0.03, 0.081}, 
                             {1.04207, 4.32994, 0.03, 0.081}, {1.04207, 2.13994, 0.081, 0.03}, {1.88207, 2.13994, 0.03, 0.081}, {1.88207, 1.65994, 0.081, 0.03}, 
                             {2.36207, 1.65994, 0.03, 0.081}, {2.36207, 1.17994, 0.081, 0.03}, {2.47207, 0.90994, 0.03, 0.081}, {2.91207, 0.71994, 0.081, 0.03}, 
                             {2.73207, 0.26994, 0.081, 0.03}, {2.73207, -0.1, 0.081, 0.03}, {3.61, -0.1, 0.03, 0.081}, {4.39, -0.1, 0.03, 0.081},
                             {4.76, -0.1, 0.081, 0.03}, {5.01, -0.0999997, 0.03, 0.081}};
                


int LineType[numberOfCheckline + 1] = {0, 0, 0, 1, 0, 2, 0, 3, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0};

Coord MidPoint(Coord a, Coord b)
{
    Coord c = {(a.x + b.x) / 2, (a.z + b.z) / 2};
    return c;
}

Coord Rotation(const double *trans, Coord coord ,const double *rotat) {
    Coord  Result;
    if(rotat[1] < 0) {
      Result.x = trans[0]  + coord.x*cos(rotat[3]) - coord.z*sin(rotat[3]);
      Result.z = trans[2]  + coord.x*sin(rotat[3]) + coord.z*cos(rotat[3]);
    }
    else {
      Result.x = trans[0]  + coord.x*cos(-rotat[3]) - coord.z*sin(-rotat[3]);
      Result.z = trans[2]  + coord.x*sin(-rotat[3]) + coord.z*cos(-rotat[3]);
    }
    
    return Result;
}


//Duong thang
bool StraightLine(int index, Coord L, Coord R, double d) {
      
     double a = CL[index-1].z - CL[index].z;
     double b = CL[index].x - CL[index-1].x;
     double c = -CL[index-1].x * a - CL[index-1].z * b;
     
     if ((abs(a*L.x + b*L.z + c) / sqrt(a*a + b*b)) > d || (abs(a*R.x + b*R.z + c) / sqrt(a*a + b*b)) > d)
          return true;
     return false;
}

//Duong cong
bool CurveLine(int index, Coord L, Coord R, Circle o) {
        if ( (((L.x - o.Ia) * (L.x - o.Ia) + (L.z - o.Ib) * (L.z - o.Ib)) > (o.R1*o.R1) && 
              ((L.x - o.Ia) * (L.x - o.Ia) + (L.z - o.Ib) * (L.z - o.Ib)) < (o.R2*o.R2))
             &&
             (((R.x - o.Ia) * (R.x - o.Ia) + (R.z - o.Ib) * (R.z - o.Ib)) > (o.R1*o.R1) &&
              ((R.x - o.Ia) * (R.x - o.Ia) + (R.z - o.Ib) * (R.z - o.Ib)) < (o.R2*o.R2))
             &&
             ((o.x1 - OffSet < L.x) && (L.x < o.x2 + OffSet) && (o.z1 - OffSet < L.z) && (L.z < o.z2 + OffSet))
             &&
             ((o.x1 - OffSet < R.x) && (R.x < o.x2 + OffSet) && (o.z1 - OffSet < R.z) && (R.z < o.z2 + OffSet))
             ) {
             //cout<< "CheckCurver:  " << ((L.x - Ia) * (L.x - Ia) + (L.z - Ib) * (L.z - Ib)) << "    " << ((R.x - Ia) * (R.x - Ia) + (R.z - Ib) * (R.z - Ib)) << "\n";
             return false;
             }
        return true;
}

bool CheckOutLine(Coord LP, Coord RP, int numLine) {
    cout << "CHECK";
    if (LineType[numLine-1]) 
      return CurveLine(numLine, LP, RP, Cir[LineType[numLine-1] - 1]);
 
     return StraightLine(numLine, LP, RP, 0.15);
}

bool ForwardLine(Coord trans, Coord point) {
  if ((trans.x >= (point.x - point.dx)) && (trans.x <= (point.x + point.dx))  && (trans.z >= (point.z - point.dz)) && (trans.z <= (point.z + point.dz)))
    return true;
  return false;
} 

Coord CreatePoint(double x, double z, double dx = 0, double dz = 0)
{
    Coord a = {x, z, dx, dz};
    return a;
}


int main()

{
   
     
    Supervisor *supervisor = new Supervisor();
    
    int Next_CP = 1;
    int Next_Line = 1;
    // int Next_CP = 15;
    // int Next_Line = 52;
    Node *IRCar = supervisor->getFromDef("Speed_Line_Follower_Robot_V4_A");
    Field *IRCar_trans_field = IRCar->getField("translation");
    Field *IRCar_rotat_field = IRCar->getField("rotation");

    
    Coord LPoint = {-0.04, -0.14};
    Coord RPoint = {0.04, -0.14};
    Coord pos1 = {0, 0}; // left point
    Coord pos2 = {0, 0}; // right point
    Coord pos = {0, 0}; // midpoint
    
    while (supervisor->step(TIME_STEP) != -1) 
    {
        
        
        pos1 = Rotation(IRCar_trans_field->getSFVec3f(), LPoint, IRCar_rotat_field->getSFRotation());
        pos2 = Rotation(IRCar_trans_field->getSFVec3f(), RPoint, IRCar_rotat_field->getSFRotation());
        pos = MidPoint(pos1, pos2);
       
        //cout << pos.x << " " << pos.z << endl;
        supervisor->wwiSendText("T|" + to_string(supervisor->getTime()));
        
        // CHECK LINE
        if(ForwardLine(pos, CL[Next_Line]))
        {
            cout <<"\nCheckLine " << Next_Line << "\n\n";
            Next_Line++;  
        }
        
        if (Next_Line == numberOfCheckline)
        {          
            cout << "\nL\t" << pos1.x << " : " << pos1.z << "\t\t\t" << "R\t" << pos2.x << " : " << pos2.z << "\t\t\t" << "C\t" << pos.x << " : " << pos.z << "\n";
            supervisor->simulationSetMode(webots::Supervisor::SimulationMode::SIMULATION_MODE_PAUSE);  
        }
         

        int i = Next_CP - 1;
       
        if(CheckOutLine(pos1, pos2, Next_Line)) 
          {
             cout << "\nL\t" << pos1.x << " : " << pos1.z << "\t\t\t" << "R\t" << pos2.x << " : " << pos2.z << "\t\t\t" << "C\t" << pos.x << " : " << pos.z << "\n";
             cout << i << " here: " << CL[Next_Line-1].x << " : " << CL[Next_Line-1].z << endl;
             supervisor->simulationSetMode(webots::Supervisor::SimulationMode::SIMULATION_MODE_PAUSE);  
          }  
                      
        if (ForwardLine(pos, CP[i])) 
          {
            Finish_time = supervisor->getTime();
            supervisor->wwiSendText("C.A.0" + to_string(Next_CP) + "|" + to_string(Finish_time));
            
            cout <<"CheckPoint\t" << "C.0" + to_string(Next_CP) + "." + to_string(Finish_time) << "\n" ;
            //cout <<"Check Point " << i+1 <<" || " << Finish_time << endl;
            Next_CP++;
          }
      if (i == numberOfCheckpoint)
        {              
        
          if (ForwardLine(CreatePoint(IRCar_trans_field->getSFVec3f()[0], IRCar_trans_field->getSFVec3f()[2]), CP[i -1]) || Condition)
            {
              if (!Condition)
                Start_time = supervisor->getTime();
              Condition = true;    
              if  (supervisor->getTime() - Start_time > 5)
              {
                cout << "FINISH ! || " << Finish_time << endl;
                supervisor->simulationSetMode(webots::Supervisor::SimulationMode::SIMULATION_MODE_PAUSE);  
              }
              // cout << CreatePoint(Master_trans[0], Master_trans[2] - 0.02, 0.3, 0.01).dz << " "<< CP[Next_CP - 2].z << endl;
              // cout << Master_trans[0] << " " << Master_trans[2] - 0.02 << " " << CP[Next_CP - 2].x << " " << CP[Next_CP - 2].z << endl;
             
            }  
            
            //cout << "\nL\t" << Pre_time_of_CP << " " << pos1.x << " : " << pos1.z << "\t\t\t" << "R\t" << pos2.x << " : " << pos2.z << "\t\t\t" << "C\t" << pos.x << " : " << pos.z << "\n";
        }
    }
    delete supervisor;
    return 0;
}
#include <webots/Supervisor.hpp>
#include <math.h>
#include <iostream>

#define TIME_STEP 8
#define OffSet 0.26 
const int numberOfCheckpoint = 15;
const int numberOfCheckline  = 55;
using namespace webots;
using namespace std;
double Finish_time = 0;
double Start_time = 0;
double time_outline = 0;
bool Condition = false;

typedef struct {
  double x, z;
  double dx = 0;
  double dz = 0;
} Coord;

Coord CP[numberOfCheckpoint ] = {{-3.0325, -3.9273, 0.081, 0.03}, {-3.03, -1.24,  0.03, 0.081}, {-3.02, 1.43, 0.081, 0.03}, {-2.27, 4.1,  0.03, 0.081}, 
                {-0.01, 3.48,  0.03, 0.081}, {2.8, 3.48, 0.081, 0.03}, {2.14, -3.51,  0.03, 0.081}, {1.29, -2.04,  0.03, 0.081}, 
                {0.17, -3.51,  0.03, 0.081}, {-1.81, -0.75, 0.081, 0.03}, {-1.81, 1.38, 0.081, 0.03}, {0.72, 2.27,  0.03, 0.081}, 
                {1.8, 1.31, 0.081, 0.03}, {1.88, -0.01, 0.081, 0.03}, {1.21713, -0.886712, 0.081, 0.15} };
    
Coord CL[numberOfCheckline] = {{-4.42, -4.74,  0.03, 0.081}, {-3.52, -4.74,  0.03, 0.081}, {-3.03, -4.25, 0.081, 0.03}, {-3.03, -3.27, 0.081, 0.03},
                {-3.34, -2.96, 0.081, 0.03}, {-3.03, -2.64,  0.03, 0.081}, {-3.03, -2.26,  0.03, 0.081}, {-3.03, -1.88,  0.03, 0.081}, 
                {-3.03, -1.24,  0.03, 0.081}, {-3.03, -0.6,  0.03, 0.081}, {-3.03, -0.22,  0.03, 0.081}, {-3.03, 0.16,  0.03, 0.081},
                {-2.71, 0.48, 0.081, 0.03}, {-3.02, 0.79, 0.081, 0.03}, {-3.02, 4.1, 0.081, 0.03}, {-1.87, 4.1,  0.03, 0.081}, 
                {-1.24707, 3.47293,  0.03, 0.081}, {-0.624803, 4.09518,  0.03, 0.081}, {-0.0025543, 3.47295,  0.03, 0.081},  {0.619701, 4.09522,  0.03, 0.081},
                {1.24196, 3.47297,  0.03, 0.081}, {1.86421, 4.09522,  0.03, 0.081}, {2.31421, 4.09522,  0.03, 0.081}, {2.79421, 3.61522, 0.081, 0.03}, 
                {2.79421, -3.05478, 0.081, 0.03}, {2.33421, -3.50478,  0.03, 0.081}, {1.88421, -3.50478,  0.03, 0.081}, {1.70421, -3.06478, 0.081, 0.03},
                {1.70421, -2.54478, 0.081, 0.03}, {1.78421, -2.31478, 0.081, 0.03}, {1.45421, -1.96478,  0.03, 0.081}, {1.08421, -1.96478,  0.03, 0.081}, 
                {0.75421, -2.30478, 0.081, 0.03}, {0.82421, -2.52478, 0.081, 0.03}, {0.82421, -3.50478, 0.081, 0.081}, {-0.81579, -3.50478,0.081, 0.081}, 
                {-0.81579, -1.63478, 0.081, 0.03}, {-1.11579, -1.34478,  0.03, 0.081}, {-1.81579, -1.34478,  0.03, 0.081}, {-1.81579, -0.08478, 0.081, 0.081}, 
                {-0.93579, -0.08478,  0.081, 0.081}, {-0.93579, 0.77522, 0.081, 0.081}, {-1.81579, 0.77522,  0.081, 0.081}, {-1.81579, 2.26522, 0.081, 0.03},
                {-0.85579, 2.26522,  0.03, 0.081}, {0.11421, 2.26522,  0.03, 0.081}, {1.21421, 2.26522,  0.03, 0.081}, {1.51421, 1.98522, 0.081, 0.03},
                {1.51421, 1.59522, 0.081, 0.03}, {2.13646, 0.972971, 0.081, 0.03}, {1.51421, 0.350721, 0.081, 0.03}, {2.12936, -0.264461, 0.081, 0.03},
                {1.50713, -0.886712, 0.081, 0.03}, {1.21713, -0.886712, 0.081, 0.03}, {0.90713, -0.886712, 0.01, 0.15}};
                
int LineType[numberOfCheckline + 1] = {0, 1, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 11, 0, 0,
                    0, 12, 13, 14, 15, 16, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 18, 0, 19, 0, 0, 0, 0, 0, 0, 0};

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
bool CurveLine(int index, Coord L, Coord R, double Ia, double Ib, double R1, double R2, double x1, double x2, double z1, double z2 ) {
        if ( (((L.x - Ia) * (L.x - Ia) + (L.z - Ib) * (L.z - Ib)) > (R1*R1) && 
              ((L.x - Ia) * (L.x - Ia) + (L.z - Ib) * (L.z - Ib)) < (R2*R2))
             &&
             (((R.x - Ia) * (R.x - Ia) + (R.z - Ib) * (R.z - Ib)) > (R1*R1) &&
              ((R.x - Ia) * (R.x - Ia) + (R.z - Ib) * (R.z - Ib)) < (R2*R2))
             &&
             ((x1 - OffSet < L.x) && (L.x < x2 + OffSet) && (z1 - OffSet < L.z) && (L.z < z2 + OffSet))
             &&
             ((x1 - OffSet < R.x) && (R.x < x2 + OffSet) && (z1 - OffSet < R.z) && (R.z < z2 + OffSet))
             ) {
             //cout<< "CheckCurver:  " << ((L.x - Ia) * (L.x - Ia) + (L.z - Ib) * (L.z - Ib)) << "    " << ((R.x - Ia) * (R.x - Ia) + (R.z - Ib) * (R.z - Ib)) << "\n";
             return false;
             }
        return true;
}

bool CheckOutLine(Coord LP, Coord RP, int numLine) {
    switch (LineType[numLine-1]) {
    case 0: 
      return StraightLine(numLine, LP, RP, 0.16);
      
   case 1:
      return CurveLine(numLine, LP, RP, -3.54, -4.23, 0.37, 0.66, -3.54, -3.03, -4.74, -4.23);
   case 2:
      return CurveLine(numLine, LP, RP, -3.03, -2.95, 0.18, 0.47, -3.34, -3.03, -2.95, -2.64);
   case 3:
      return CurveLine(numLine, LP, RP, -3.03, -2.45, 0.02, 0.32, -3.03, -2.84, -2.64, -2.26);
   case 4:
      return CurveLine(numLine, LP, RP, -3.02, -2.07, 0.04, 0.34, -3.21, -3.02, -2.26, -1.88);
   case 5:
      return CurveLine(numLine, LP, RP, -3.03, -1.56, 0.17, 0.46, -3.03, -2.71, -1.88, -1.24);
   case 6:
      return CurveLine(numLine, LP, RP, -3.03, -0.92, 0.17, 0.50, -3.35, -3.03, -1.24, -0.6);
   case 7: 
      return CurveLine(numLine, LP, RP, -3.04, -0.41, 0.04, 0.34, -3.04, -2.85, -0.6, -0.22);
   case 8: 
      return CurveLine(numLine, LP, RP, -3.03, -0.03, 0.04, 0.34, -3.22, -3.03, -0.22, 0.16);
   case 9: 
      return CurveLine(numLine, LP, RP, -3.02, 0.47, 0.15, 0.44, -3.02, -2.71, 0.16, 0.47);
   case 10: 
      return CurveLine(numLine, LP, RP, 2.33, 3.63, 0.32, 0.62, 2.33, 2.8, 3.63, 4.1);
   case 11: 
      return CurveLine(numLine, LP, RP, 2.33, -3.04, 0.32, 0.62, 2.33, 2.8, -3.51, -3.04);
   case 12: 
      return CurveLine(numLine, LP, RP, 1.94, -2.5, 0.09, 0.37, 1.58, 1.94, -2.5, -2.26);
   case 13: 
      return CurveLine(numLine, LP, RP, 1.61, -2.15, 0.09, 0.39, 1.37, 1.85, -2.39, -1.91);
   case 14: 
      return CurveLine(numLine, LP, RP, 1.27, -1.81, 0.09, 38, 1.08, 1.45, -2.05, -1.96);
   case 15: 
      return CurveLine(numLine, LP, RP, 0.93, -2.14, 0.09, 0.39, 0.69, 1.17, -2.38, -1.9);
   case 16: 
      return CurveLine(numLine, LP, RP, 0.59, -2.48, 0.09, 0.39, 0.59, 0.94, -2.48, -2.24);
   case 17: 
      return CurveLine(numLine, LP, RP, -0.81, -1.34, 0.15, 0.45, -1.11, -0.51, -1.64, -1.04);   
   case 18: 
      return CurveLine(numLine, LP, RP, -0.37, 2.27, 0.34, 0.64, -0.86, 0.12, 1.78, 2.76);      
   default: return false;     
  }
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
    //int Next_CP = 14;
    //int Next_Line = 55;
    Node *IRCar = supervisor->getFromDef("Speed_Line_Follower_Robot_V4");
    Field *IRCar_trans_field = IRCar->getField("translation");
    Field *IRCar_rotat_field = IRCar->getField("rotation");
    Field *IRCar_controller_field = IRCar->getField("controller");

    string FirstPosition;
    IRCar->saveState(FirstPosition);
     
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
       
         string message = supervisor->wwiReceiveText();
   
		
	if (message.length() != 0) {
                       //cout << message << endl;
		if (message == "reload") {
                          IRCar->loadState(FirstPosition);
  		    for (int delay = 0; delay < 100; delay++);
  		    IRCar_controller_field->setSFString(IRCar_controller_field->getSFString());
    		    for (int delay = 0; delay < 100; delay++);
    		    supervisor->worldSave("/home/ceec/Tung/Map_Round1_SS2 (3)/worlds/temp_save.wbt");
    		    for (int delay = 0; delay < 5000; delay++);
    		    supervisor->simulationReset();
		    
		}
		else if (message == "start") {
  		    //cout << "start" << endl;
  		    IRCar->loadState(FirstPosition);
  		    for (int delay = 0; delay < 100; delay++);
  		    // IRCar_B->loadState(FirstPosition_B);
  		    for (int delay = 0; delay < 100; delay++);
  		    IRCar_controller_field->setSFString(IRCar_controller_field->getSFString());
    		    for (int delay = 0; delay < 100; delay++);
    		    // supervisor->worldSave("C:/Users/UIT CAR RACING/Desktop/Map_thi/LFR_Test_Map/LFR_Test_Map/worlds/LFR_Test_Map.wbt");
    		    for (int delay = 0; delay < 5000; delay++);
    		    supervisor->simulationReset();
    		    //supervisor->worldReload();
    		    //supervisor->worldLoad("C:/Users/UIT CAR RACING/Desktop/Map_thi/LFR_Test_Map/LFR_Test_Map/worlds/LFR_Test_Map.wbt");
		 }
		else 
		{
            	  //cout << message << endl;
                        IRCar_controller_field->setSFString(message);
		}
                	
    }
        
        
        //cout << pos.x << " " << pos.z << endl;
        supervisor->wwiSendText("T|" + to_string(supervisor->getTime()));
        
        // CHECK LINE
        if(ForwardLine(pos, CL[Next_Line]))
        {
            //cout <<"\nCheckLine " << Next_Line << "\n\n";
            Next_Line++;  
        }
        
        if (Next_Line == numberOfCheckline)
        {        
            //cout << "outtline" << endl;
            supervisor->simulationSetMode(webots::Supervisor::SimulationMode::SIMULATION_MODE_PAUSE);  
        }
         

        int i = Next_CP - 1;
       
        if(CheckOutLine(pos1, pos2, Next_Line)) 
          {
           
             cout << "outtline" << endl;
             supervisor->simulationSetMode(webots::Supervisor::SimulationMode::SIMULATION_MODE_PAUSE);
          }  
                      
        if (ForwardLine(pos, CP[i])) 
          {
            Finish_time = supervisor->getTime();
            if (i < 14)
              supervisor->wwiSendText("C.0" + to_string(Next_CP) + "|" + to_string(Finish_time));
            Next_CP++;
          }
      if (i == numberOfCheckpoint)
        {              
        
          if (ForwardLine(CreatePoint(IRCar_trans_field->getSFVec3f()[0], IRCar_trans_field->getSFVec3f()[2]), CP[i -1]) || Condition)
            {
              if (!Condition)
              {
                Start_time = supervisor->getTime();
                Condition = true;    
              }
              if  (supervisor->getTime() - Start_time > 3)
              {
                cout << "FINISH ! || " << Finish_time << endl;
                supervisor->wwiSendText("C.0" + to_string(--Next_CP) + "|" + to_string(Finish_time));
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

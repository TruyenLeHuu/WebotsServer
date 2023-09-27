#include <webots/Supervisor.hpp>
#include <math.h>
#include <iostream>
#include <string.h>

using namespace webots;
using namespace std;



int main()

{
  
    Supervisor *supervisor = new Supervisor();
    string Name; 
   
   
    for (int i = 1; i <= 32; i++)
    {
      Name = "CL2_";
      Name.insert(4, to_string(i));
      Node *Node = supervisor->getFromDef(Name);
      Field *Node_field = Node->getField("translation");
      const double *Node_trans = Node_field->getSFVec3f();
      cout << "{" << Node_trans[0] << ", " << Node_trans[2] << "\n";
          
      //cout << Name << endl;
    } 
    cout<<endl;

    delete supervisor;
    return 0;
}

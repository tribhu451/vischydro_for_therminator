#include "read_ic.h"

using namespace std;

read_ic::read_ic(idb *_IDB)
{
IDB = _IDB;
}



read_ic::~read_ic(){}



void read_ic::set_ic(grid* f, EoS* eos)
{
  cout<<"*** [IC] ***"<<endl;
  cout<<"[Info] reading external file ...\n"<<endl;
  

  double eps, dummy1, dummy;
  double xf, yf ; // x and y from file.
  double ux, uy, utau;


  cell* c;
  fstream ic_file;
  ic_file.open(IDB->init_file_name.c_str());
  if (!ic_file) 
    {cout<<"couldn't find ic file."<<endl; exit(1);}
 
  ic_file.getline(buff,200);
 
  while(!ic_file.eof())
    { // while start
      ic_file.getline(buff,200);
      iss = new istringstream(buff);
      *iss >> dummy1 >> xf >> yf 
                     >> eps >> utau >> ux >> uy
                     >> dummy  >> dummy  >> dummy  >> dummy;
      delete iss;
      
      for(int i=0; i<IDB->nx; i++)
	{ // i loop
	  double x_ = IDB->xmin + i*IDB->dx;
	  if (abs(xf-x_)<0.0001)
	    { // if x
	      for(int j=0; j<IDB->ny; j++)
		{
	          double y_ = IDB->ymin + j*IDB->dy;
		  if(abs(yf - y_)<0.0001)
		    { //if y
                      for(int k=0; k<IDB->neta; k++)
                         {
                           double eta = IDB->etamin + k*IDB->deta;
		           double vx = 0.0;
                           double vy = 0.0;
		           c = f->get_cell(i,j,k); 
                           double eps1 = eps*exp(((-(( abs(eta) - IDB->eta_platue)*
                                           ( abs(eta)-IDB->eta_platue))/(2*pow( IDB->eta_fall,2.0))))*
                                                  theta(abs(eta)-IDB->eta_platue));        
		           double vz= 0.0; double nb= 0; double ns=0; double nq=0;
                           c->set_prim_var(eos, IDB->tau0, eps1, nb, nq, ns, vx, vy, vz); 
                         } // eta bracket    		      
		    } // if y
		  else
		    {
		      continue ; 
		    }
		  
		} 
	    } // if x
          else
	    {
	      continue ;
	    }
	} // i loop
    }//while end
 ic_file.close();
  cout<<"\n";
}





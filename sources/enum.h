//#
//# FILE: enum.h -- Interface to Enum solution module
//#
//# $Id$
//#

#ifndef ENUM_H
#define ENUM_H

#include "normal.h"
#include "rational.h"
#include "glist.h"
#include "gstatus.h"
#include "tableau.h"
#include "gtableau.h"

class EnumParams     {
public:
  int plev, nequilib;
  gOutput *outfile, *errfile;
  gStatus &status;

	EnumParams(gStatus &status_=gstatus);
};

//
// The general-purpose interface to the Module routines
//

template <class T> class EnumModule  {
private:
  BFS_List List;
  DumbTableau<T> tab;
  EnumParams params;
  int rows,cols,level,players;
  long count,npivots;
  gRational time;
  
//  void AddSolution(const gPVector<T> &s);
  
public:
  EnumModule(const NormalForm<T> &N, const EnumParams &p); 
  
  int Enum(void);
  void SubSolve(int pr, int pcl, Tableau<T> &B1, gBlock<int> &targ1);
  
  int NumPivots(void) const;
  gRational Time(void) const;
  
  EnumParams &Parameters(void);

      //  Ted -- I like 2nd form better, but does this ever get 
      //         de-allocated?  

//  void GetSolutions(gList<gPVector<T> > &solutions) const;
  gList<gPVector<T> > &GetSolutions(void) const;

      //  Ted -- Valuable information (the basis) is lost in this 
      //         module by converting to gPVectors. Can 
      //         we find a better way to do this.  
};

//
// Convenience functions for "one-shot" evaluations
//
template <class T> int Enum(const NormalForm<T> &N, const EnumParams &p,
			    gList<gPVector<T> > &solutions,
			    int &npivots, gRational &time);

#endif    // ENUM_H





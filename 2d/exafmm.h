#ifndef exafmm_h
#define exafmm_h
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "vec.h"

namespace exafmm {
  // Basic type definitions
  typedef double real_t;                                        //!< Floating point type is double precision
  typedef std::complex<real_t> complex_t;                       //!< Complex type
  typedef vec<2,real_t> vec2;                                   //!< Vector of 2 real_t types

  //! Structure of bodies
  struct Body {
    vec2 X;                                                     //!< Position
    real_t q;                                                   //!< Charge
    real_t p;                                                   //!< Potential
    vec2 F;                                                     //!< Force
  };
  typedef std::vector<Body> Bodies;                             //!< Vector of bodies

  //! Structure of cells
  struct Cell {
    int NCHILD;                                                 //!< Number of child cells
    int NBODY;                                                  //!< Number of descendant bodies
    Cell * CHILD;                                               //!< Pointer to first child cell
    Body * BODY;                                                //!< Pointer to first body
    vec2 X;                                                     //!< Cell center
    real_t R;                                                   //!< Cell radius
#if EXAFMM_LAZY
    std::vector<Cell*> listM2L;                                 //!< M2L interaction list
    std::vector<Cell*> listP2P;                                 //!< P2P interaction list
#endif
    std::vector<complex_t> M;                                   //!< Multipole expansion coefficients
    std::vector<complex_t> L;                                   //!< Local expansion coefficients
  };
  typedef std::vector<Cell> Cells;                              //!< Vector of cells

  // Global variables
  int P;                                                        //!< Order of expansions
  int ncrit;                                                    //!< Number of bodies per leaf cell
  real_t theta;                                                 //!< Multipole acceptance criterion
  vec2 dX;                                                      //!< Distance array
#pragma omp threadprivate(dX)                                   // Make global variables private

  //! L2 norm of array X
  /*!
    \param X a two-element array.
    \return L2 norm of X
  */
  inline real_t norm(const real_t * X) {
    return X[0] * X[0] + X[1] * X[1];
  }
}
#endif

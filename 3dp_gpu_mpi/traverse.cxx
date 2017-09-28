#include "mpi_utils.h"
#include "args.h"
#include "build_tree.h"
#include <cassert>
#include "dataset.h"
#include "partition.h"
#include "test.h"
using namespace exafmm;

int main(int argc, char ** argv) {
  Args args(argc, argv);
  startMPI();
  THETA = args.theta;
  NCRIT = args.ncrit;
  LEVEL = args.level;
  VERBOSE = args.verbose;
  const int numBodies = args.numBodies;
  const char * distribution = args.distribution;
  CYCLE = 2 * M_PI;
  IMAGES = args.images;

  Bodies bodies = initBodies(numBodies, distribution, MPIRANK, MPISIZE);
  for (size_t b=0; b<bodies.size(); b++) bodies[b].q = 1;

  partition(bodies);
  Cells cells = buildTree(bodies);
  initKernel();
  upwardPass(&cells[0]);
  if (IMAGES == 0) {
    horizontalPass(&cells[0], &cells[0]);
  } else {
    for (IX[0]=-1; IX[0]<=1; IX[0]++) {
      for (IX[1]=-1; IX[1]<=1; IX[1]++) {
        for (IX[2]=-1; IX[2]<=1; IX[2]++) {
          horizontalPass(&cells[0], &cells[0]);
        }
      }
    }
    real_t saveCycle = CYCLE;
    periodic(&cells[0], &cells[0]);
    CYCLE = saveCycle;
  }
  downwardPass(&cells[0]);

  uint64_t imageBodies = std::pow(3,3*IMAGES) * bodies.size();
  print("numBodies", imageBodies);
  print("bodies[0].p", bodies[0].p);
  for (size_t b=0; b<bodies.size(); b++) assert(imageBodies == bodies[b].p);
  print("Assertion passed");
  stopMPI();
  return 0;
}

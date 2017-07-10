#ifndef verify_h
#define verify_h
#include <fstream>
#include <map>
#include "exafmm.h"

namespace exafmm {
  class Verify {
    typedef std::map<uint64_t,double> Record;
    typedef Record::iterator R_iter;

  private:
    const char * path;

  public:
    bool verbose;
    double average, average2;

    Verify() : path("./"), average(0), average2(0) {}
    Verify(const char * _path) : path(_path), average(0), average2(0) {}

    double getSumScalar(const Bodies & bodies) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += bodies[b].p * bodies[b].q;
      }
      return v;
    }

    double getNrmScalar(const Bodies & bodies) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs(bodies[b].p * bodies[b].p);
      }
      return v;
    }

    double getDifScalar(const Bodies & bodies, const Bodies & bodies2) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs((bodies[b].p - bodies2[b].p) *
                      (bodies[b].p - bodies2[b].p));
      }
      return v;
    }

    double getRelScalar(const Bodies & bodies, const Bodies & bodies2) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs(((bodies[b].p - bodies2[b].p) * (bodies[b].p - bodies2[b].p))
                      / (bodies2[b].p * bodies2[b].p));
      }
      return v;
    }

    double getNrmVector(const Bodies & bodies) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs(norm(bodies[b].F));
      }
      return v;
    }

    double getDifVector(const Bodies & bodies, const Bodies & bodies2) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs((bodies[b].F[0] - bodies2[b].F[0]) * (bodies[b].F[0] - bodies2[b].F[0]) +
                      (bodies[b].F[1] - bodies2[b].F[1]) * (bodies[b].F[1] - bodies2[b].F[1]));
      }
      return v;
    }

    double getRelVector(const Bodies & bodies, const Bodies & bodies2) {
      double v = 0;
      for (size_t b=0; b<bodies.size(); b++) {
        v += std::abs(((bodies[b].F[0] - bodies2[b].F[0]) * (bodies[b].F[0] - bodies2[b].F[0]) +
                       (bodies[b].F[1] - bodies2[b].F[1]) * (bodies[b].F[1] - bodies2[b].F[1]))
                      / norm(bodies2[b].F));
      }
      return v;
    }
  };
}
#endif

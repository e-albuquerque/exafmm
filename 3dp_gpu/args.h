#ifndef args_h
#define args_h
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <stdint.h>

namespace exafmm {
  static struct option long_options[] = {
    {"accuracy",     no_argument,       0, 'a'},
    {"ncrit",        required_argument, 0, 'c'},
    {"distribution", required_argument, 0, 'd'},
    {"help",         no_argument,       0, 'h'},
    {"numBodies",    required_argument, 0, 'n'},
    {"P",            required_argument, 0, 'P'},
    {"theta",        required_argument, 0, 't'},
    {"verbose",      no_argument,       0, 'v'},
    {0, 0, 0, 0}
  };

  //! Argument parser class
  /*!
    Parse and set the parameters of FMM and bodies from argv
  */
  class Args {
  public:
    int accuracy;                               //!< Regression for accuracy only
    int ncrit;                                  //!< Number of bodies per leaf cell 
    const char * distribution;                  //!< Body Distribution
    int numBodies;                              //!< Number of bodies
    int P;                                      //!< Order of expansions
    double theta;                               //!< Multipole acceptance criterion
    int verbose;                                //!< Verbose mode

  private:
    //! Print the usage of option-arguments
    void usage(char * name) {
      fprintf(stderr,
              "Usage: %s [options]\n"
              "Long option (short option)       : Description (Default value)\n"
              " --accuracy (-a)                 : Regression for accuracy only (%d)\n"
              " --ncrit (-c)                    : Number of bodies per leaf cell (%d)\n"
              " --distribution (-d) [c/l/o/p/s] : lattice, cube, sphere, octant, plummer (%s)\n"
              " --help (-h)                     : Show this help document\n"
              " --numBodies (-n)                : Number of bodies (%d)\n"
              " --P (-P) not working            : Order of expansion (%d)\n"
              " --theta (-t)                    : Multipole acceptance criterion (%f)\n"
              " --verbose (-v)                  : Print information to screen (%d)\n",
              accuracy,
              name,
              ncrit,
              distribution,
              numBodies,
              P,
              theta,
              verbose);
    }

    //! Parse body distribution from option-argument (optarg)
    /*!
      \param arg a pointer to optarg
      \return a string of distribution name
    */
    const char * parseDistribution(const char * arg) {
      switch (arg[0]) {
        case 'c': return "cube";
        case 'l': return "lattice";
        case 'o': return "octant";
        case 'p': return "plummer";
        case 's': return "sphere";
        default:
          fprintf(stderr, "invalid distribution %s\n", arg);
          abort();
      }
      return "";
    }

    //! Get the integer (value) mapped to body distribution (key)
    /*!
      \param _distribution body distribution string
      \return the associated integer
    */
    uint64_t getDistNum(const char * _distribution) {
      switch (_distribution[0]) {
        case 'c': return 0;
        case 'l': return 1;
        case 'o': return 2;
        case 'p': return 3;
        case 's': return 4;
        default:
          fprintf(stderr, "invalid distribution %s\n", _distribution);
          abort();
      }
      return 0;
    }

  public:
    //! Constructor
    /*!
      Set default values to FMM parameters and parse argv for user-defined options
    */
    Args(int argc=0, char ** argv=NULL)
      : accuracy(0),
        ncrit(64), 
        distribution("cube"),
        numBodies(1000000),
        P(10),
        theta(.4),
        verbose(0) 
    {
      while (1) {
        int option_index;
        int c = getopt_long(argc, argv, "ac:d:hn:P:t:v",
                            long_options, &option_index);
        if (c == -1) break;
        switch (c) {
          case 'a':
            accuracy = 1;
            break;
          case 'c':
            ncrit = atoi(optarg);
            break;
          case 'd':
            distribution = parseDistribution(optarg);
            break;
          case 'h':
            usage(argv[0]);
            abort();
          case 'n':
            numBodies = atoi(optarg);
            break;
          case 'P':
            P = atoi(optarg);
            break;
          case 't':
            theta = atof(optarg);
            break;
          case 'v':
            verbose = 1;
            break;
          default:
            usage(argv[0]);
            abort();
        }
      }
    }

    //! Calculate key based on arguments
    /*!
      Use bit masking to store arguments information in key, which will be
      used in regression test to identify different runs
      \return key
    */
    uint64_t getKey() {
      uint64_t key = 0;
      key |= uint64_t(round(log(ncrit)/log(2)));                // [1-4] bit
      key |= getDistNum(distribution) << 4;                     // [5-15] bit
      key |= uint64_t(round(log(numBodies)/log(10))) << 15;     // [16-19] bit
      key |= P << 19;                                           // [20-28] bit
      key |= uint64_t(theta*14) << 28;                          // [29-64] bit
      return key;
    }

    //! Print formatted output for arguments
    /*!
      \param stringLength field width for output
    */
    void print(int stringLength) {
      if (verbose) {
        std::cout << std::setw(stringLength) << std::fixed << std::left
                  << "ncrit" << " : " << ncrit << std::endl
                  << std::setw(stringLength)
                  << "distribution" << " : " << distribution << std::endl
                  << std::setw(stringLength)
                  << "numBodies" << " : " << numBodies << std::endl
                  << std::setw(stringLength)
                  << "P" << " : " << P << std::endl
                  << std::setw(stringLength)
                  << "theta" << " : " << theta << std::endl
                  << std::setw(stringLength)
                  << "verbose" << " : " << verbose << std::endl
                  << std::setw(stringLength);
      }
    }
  };
}
#endif

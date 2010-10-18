/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2008 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#include <irtkImage.h>

#include <irtkHistogram.h>

#include <irtkTransformation.h>

// Default filenames
char *output_name = NULL, *prefix_name = NULL, *suffix_name = NULL, *textfile = NULL;

#define MAX_IMAGES 10000
#define EPSILON    0.001

void usage()
{
  cerr << "Usage: atlas [output] <input1..inputN> <options>" << endl;
  cerr << "" << endl;
  cerr << "Make an atlas from a given set of input images. All input images must have the" << endl;
  cerr << "same voxel lattice. Names of input images can be given on the command line or" << endl;
  cerr << "in a file (see below)." << endl;
  cerr << "" << endl;
  cerr << "If they are named on the command line, images are given equal weighting." << endl;
  cerr << "Different weights can be given if a file with names is provided." << endl;
  cerr << "" << endl;
  cerr << "where <options> is one or more of the following:" << endl;
  cerr << "<-imagenames file>      File containing image names and values to use for weighting." << endl;
  cerr << "                        The format should be one line for each image with" << endl;
  cerr << "                          \"image_name value\"" << endl;
  cerr << "                        on each line. If a mean and sigma value are specified" << endl;
  cerr << "                        with the \'-gaussian\' flag (see below), then the values" << endl;
  cerr << "                        are converted to weights using the corresponding Gaussian" << endl;
  cerr << "                        function. Otherwise, the values in the file are used" << endl;
  cerr << "                        directly as weights." << endl;
  cerr << "<-prefix directory>     Directory in which to find the images." << endl;
  cerr << "<-gaussian mean sigma>  Use Gaussian with mean and sigma for kernel-based smoothing\n";
  cerr << "<-epsilon value>        Epsilon value for ignoring image in kernel-based smoothing " << endl;
  cerr << "                          (default = " << EPSILON << ")" << endl;
  cerr << "<-scaling value>        Scaling value for final atlas (default = 1)\n";
  cerr << "<-norm>                 Normalise intensities before atlas construction " << endl;
  cerr << "                          (default = off)\n";
  exit(1);
}

void normalise(irtkGreyImage &input, irtkRealImage &output)
{
  int j;
  double mean, std;
  irtkGreyPixel *ptr1;
  irtkRealPixel *ptr2;

  // Set up input histogram
  cerr << "Setting up input histogram..."; cout.flush();
  irtkGreyPixel min, max;
  input.GetMinMax(&min, &max);
  irtkHistogram histogram(max-min+1);
  histogram.PutMin(min-0.5);
  histogram.PutMax(max+0.5);

  ptr1 = input.GetPointerToVoxels();
  for (j = 0; j < input.GetNumberOfVoxels(); j++) {
    histogram.AddSample(*ptr1);
    ptr1++;
  }

  mean = histogram.Mean();
  std  = histogram.StandardDeviation();
  cout << "done" << endl;
  cerr << "Stats: Mean = " << mean << " SD = " << std << endl;

  ptr1 = input.GetPointerToVoxels();
  ptr2 = output.GetPointerToVoxels();
  for (j = 0; j < input.GetNumberOfVoxels(); j++) {
    *ptr2 = (*ptr1 - mean) / std;
    ptr1++;
    ptr2++;
  }
}

int main(int argc, char **argv)
{
  double scale, mean, sigma, epsilon;
  int i, j, n, padding, norm, no, ok;
  int useGaussianWeights;
  irtkGreyPixel *ptr1;
  irtkGreyImage input;
  irtkRealPixel *ptr2;
  irtkRealImage tmp, output;

  // Check command line
  if (argc < 5) {
    usage();
  }

  // Parse parameters
  output_name = argv[1];
  argc--;
  argv++;

  // Default: scaling factor
  scale = 1;

  // Default: Use weights directly. Alternatively, convert using Gaussian formula.
  useGaussianWeights = false;
  
  // Default values for kernel smoothing
  mean  = 0;
  sigma = 1;

  // Default: No intensity  normalisation
  norm  = false;

  // Default: Epsilon
  epsilon = EPSILON;

  // Default padding
  padding = MIN_GREY;

  // Parse input file names and values
  char  **input_name   = new char *[MAX_IMAGES];
  double *input_value  = new double[MAX_IMAGES];
  double *input_weight = new double[MAX_IMAGES];
  double  total_weight = 0;

  // Parse any remaining paramters
  no = 0;

  while ((argc > 1) && (argv[1][0] != '-' )) {
    input_name[no]   = argv[1];
    input_weight[no] = 1;
    total_weight    += 1;
    no++;
    argc--;
    argv++;
  }

  while (argc > 1) {
    ok = false;
    if ((ok == false) && (strcmp(argv[1], "-scaling") == 0)) {
      argc--;
      argv++;
      scale = atof(argv[1]);
      argc--;
      argv++;
      ok = true;
    }
    if ((ok == false) && (strcmp(argv[1], "-gaussian") == 0)) {
      argc--;
      argv++;
      useGaussianWeights = true;
      mean = atof(argv[1]);
      argc--;
      argv++;
      sigma = atof(argv[1]);
      argc--;
      argv++;
      ok = true;
    }
    if ((ok == false) && (strcmp(argv[1], "-norm") == 0)) {
      argc--;
      argv++;
      norm = true;
      ok = true;
    }
    if ((ok == false) && (strcmp(argv[1], "-imagenames") == 0)) {
      argc--;
      argv++;
      textfile = argv[1];
      argc--;
      argv++;
      ok = true;
    }
    if ((ok == false) && (strcmp(argv[1], "-padding") == 0)) {
      argc--;
      argv++;
      padding = atoi(argv[1]);
      argc--;
      argv++;
      ok = true;
    }
    if ((ok == false) && (strcmp(argv[1], "-prefix") == 0)) {
       argc--;
       argv++;
       prefix_name = argv[1];
       argc--;
       argv++;
       ok = true;
     }
    if ((ok == false) && (strcmp(argv[1], "-suffix") == 0)) {
       argc--;
       argv++;
       suffix_name = argv[1];
       argc--;
       argv++;
       ok = true;
     }
    if ((ok == false) && (strcmp(argv[1], "-epsilon") == 0)) {
      argc--;
      argv++;
      epsilon = atof(argv[1]);
      argc--;
      argv++;
      ok = true;
    }
    if (ok == false) {
      cerr << "Unknown argument: " << argv[1] << endl << endl;
      usage();
    }
  }

  if (textfile != NULL) {
    ifstream in(textfile);
    if (in.is_open()) {
      while (!in.eof()) {
        input_name[no] = new char[256];
        in >> input_name[no] >> input_value[no];
        if (strlen(input_name[no]) > 0) {
          
          if (useGaussianWeights == true){
            // Convert input value to weight based on Gaussian centred at above specified mean.
            input_weight[no] = 1.0 / sqrt(2.0) / sigma * exp(-pow((mean - input_value[no])/sigma, 2.0)/2);
          } else {
            // Use input value directly as a weight.
            input_weight[no] = input_value[no];
          }
          
          if (input_weight[no] > epsilon) {
            total_weight   += input_weight[no];
            cout << input_value[no] << " " << input_weight[no] << " " << total_weight << endl;
            no++;
          }
        }
      }
      in.close();
    } else {
      cout << "atlas: Unable to open file " << textfile << endl;
      exit(1);
    }
  }

  // Read and add one input image after the other
  n = 0;
  for (i = 0; i < no; i++) {
    char buffer[255];

    // Read input
    if (prefix_name != NULL) {
      sprintf(buffer, "%s%s", prefix_name, input_name[i]);
    } else {
      sprintf(buffer, "%s", input_name[i]);
    }
    input_name[i] = strdup(buffer);
    if (suffix_name != NULL) {
      sprintf(buffer, "%s%s",  input_name[i], suffix_name);
    } else {
      sprintf(buffer, "%s", input_name[i]);
    }
    input_name[i] = strdup(buffer);

    cout << "Reading input image " << input_name[i] << "... "; cout.flush();
    input.Read(input_name[i]);
    cout << "done" << endl;

    if (i == 0) {
      n = input.GetX()*input.GetY()*input.GetZ();
      output = input;
      ptr2 = output.GetPointerToVoxels();
      for (j = 0; j < n; j++) {
        *ptr2 = 0;
        ptr2++;
      }
    }

    tmp = input;
    if (norm == true) normalise(input, tmp);

    cerr << "Adding input to atlas..."; cout.flush();
    tmp *= input_weight[i];
    output += tmp;
    cerr << "done\n";

  }

  ptr1 = input.GetPointerToVoxels();
  ptr2 = output.GetPointerToVoxels();
  for (j = 0; j < n; j++) {
    *ptr1 = round(scale * *ptr2 / total_weight);
    ptr1++;
    ptr2++;
  }
  cerr << " done\n";

  // Write atlas
  cerr << "Writing atlas to " << output_name << " ... ";
  cout.flush();
  input.Write(output_name);
  cerr << "done\n";
}

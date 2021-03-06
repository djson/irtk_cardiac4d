/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2009 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#ifndef _IRTKMULTIPLEIMAGEREGISTRATION2_H

#define _IRTKMULTIPLEIMAGEREGISTRATION2_H

/**
 * Generic for image registration based on voxel similarity measures.
 *
 * This class implements a registration filter which takes two input images
 * and calculates the transformation which maps the second image (denotes as
 * source image) into the coordinate system of the first image (denotes as
 * target image).  This is the abstract base class which defines a common
 * interface for arbitrary registration filters. Each derived class has to
 * implement all abstract member functions.
 *
 */

class irtkMultipleImageRegistration2 : public irtkRegistration2
{

  /// Interface to input file stream
  friend istream& operator>> (istream&, irtkMultipleImageRegistration2*);

  /// Interface to output file stream
  friend ostream& operator<< (ostream&, const irtkMultipleImageRegistration2*);

protected:

    /** First set of input image. This image is denoted as target image and its
   *  coordinate system defines the frame of reference for the registration.
   */
  irtkRealImage **_target;

  /** Second input image. This image is denoted as source image. The goal of
   *  the registration is to find the transformation which maps the source
   *  image into the coordinate system of the target image.
   */
  irtkRealImage **_source;

#ifdef HAS_VTK
  /// Landmark input
  vtkPolyData *_ptarget;

  /// Landmark input
  vtkPolyData *_psource;

  /// Landmark gradient
  vtkPolyData *_pgradient;
#endif

  /// Number of images (must be equal for source and target images)
  int _numberOfImages;

  /** Current estimate of the source image transformed back into the target
   *  coordinate system. This is updated every time the Update function is
   *  called.
   */
  irtkGenericImage<double> *_transformedSource;

  /// Gradient of the original source
  irtkGenericImage<double> *_sourceGradient;

  /// Gradient of the transformed source
  irtkGenericImage<double> *_transformedSourceGradient;

  /// Gradient of the similarity metric
  irtkGenericImage<double> *_similarityGradient;

  /// Transformation
  irtkTransformation *_transformation;

  /// 2D histogram (this is not used for all similarity metrics)
  irtkHistogram_2D<double> **_histogram;

  /// Interpolator for source image
  irtkInterpolateImageFunction **_interpolator;

  /// Interpolator for source image gradient
  irtkInterpolateImageFunction **_interpolatorGradient;

  /// Blurring of target image (in mm)
  double _TargetBlurring[MAX_NO_RESOLUTIONS];

  /// Blurring of source image (in mm)
  double _SourceBlurring[MAX_NO_RESOLUTIONS];

  /// Minimum length of steps
  double _MinStep[MAX_NO_RESOLUTIONS];

  /// Maximum length of steps
  double _MaxStep[MAX_NO_RESOLUTIONS];

  /// Max. number of iterations per step size
  int    _NumberOfIterations[MAX_NO_RESOLUTIONS];

  /// Padding value of target image
  short  _TargetPadding;

  /// Padding value of target image
  short  _SourcePadding;

  /// Number of levels of multiresolution pyramid
  int    _NumberOfLevels;

  /// Max. number of bins for histogram
  int    _NumberOfBins;

  /// Similarity measure for registration
  irtkSimilarityMeasure  _SimilarityMeasure;

  /// Optimization method for registration
  irtkOptimizationMethod _OptimizationMethod;

  /// Interpolation mode to use during resampling and registration
  irtkInterpolationMode _InterpolationMode;

  /// Convergence parameter for optimization based on change in similarity.
  double _Epsilon;

  /// Debugging flag
  int    _DebugFlag;

  /// Current min and max voxel values
  int _target_min, _target_max;
  int _source_min, _source_max;
  int *_maxDiff;
  int _totalVoxels;
  double _totalWeight;

  /// image lambda weight
  double **_weight;

  /// Landmark regulation penalty coefficient
  double _Lregu;

  int _level;

  /// Current level in the multi-resolution pyramid
  int _CurrentLevel;

  /// Current iteration during the optimization
  int _CurrentIteration;

  /// Source image domain which can be interpolated fast
  double *_source_x1, *_source_y1, *_source_z1;
  double *_source_x2, *_source_y2, *_source_z2;

  /// Initial set up for the registration
  virtual void Initialize();

  /// Final set up for the registration
  virtual void Finalize();

  /// Initial set up for the registration at a multiresolution level
  virtual void Initialize(int);

  /// Final set up for the registration at a multiresolution level
  virtual void Finalize(int);

  /// Update state of the registration based on current transformation estimate
  virtual void Update(bool);

  /// Update state of the registration based on current transformation estimate (source image)
  virtual void UpdateSource() = 0;

  /// Update state of the registration based on current transformation estimate (source image and source image gradient)
  virtual void UpdateSourceAndGradient() = 0;

  /// Evaluate similarity measure: SSD
  virtual double EvaluateSSD();

  /// Evaluate gradient of similarity measure: SSD
  virtual void EvaluateGradientSSD();

  /// Evaluate similarity measure: NMI
  virtual double EvaluateNMI();

  /// Evaluate gradient of similarity measure: NMI
  virtual void EvaluateGradientNMI();

#ifdef HAS_VTK

  /// Calculate the landmark penalty gradient
  virtual void EvaluateGradientLandmark();

#endif

  /** Evaluates the smoothness preservation term. */
  virtual double LandMarkPenalty();

public:

  /// Constructor
  irtkMultipleImageRegistration2();

  /// Destructor
  virtual ~irtkMultipleImageRegistration2();

  /// Sets input for the registration filter
  virtual void SetInput (irtkRealImage **, irtkRealImage **, int);

  /// Sets output for the registration filter
  virtual void SetOutput(irtkTransformation *) = 0;

  /// Sets weighting for different images at different levels
  virtual void SetWeighting(double **);

  /// Runs the registration filter
  virtual void Run();

  /** Evaluates the similarity metric. This function evaluates the similarity
   *  metric of the registration by looping over the target image and
   *  interpolating the transformed source image. This function returns the
   *  value of the similarity measure.
   */
  virtual double Evaluate();

  /** Evaluates the gradient of the similarity metric. This function
   *  evaluates the gradient of the similarity metric of the registration
   *  by looping over the target image and interpolating the transformed
   *  source image while computing the gradient. The derivatives in the
   *  gradient are expressed with respect to voxel wise displacements, not
   *  with respect to the parameters of the transformation. Each subclass
   *  of this class must override this function in order to compute the
   *  gradient with respect to the transformation parameters.
   */
  virtual double EvaluateGradient(double *);

  /// Returns the name of the class
  virtual const char *NameOfClass() = 0;

  /// Prints debugging messages if debugging is enabled
  virtual void Debug(string);

#ifdef HAS_VTK

  /// Sets landmark regulation input for the registration filter
  virtual void SetLandmarks (vtkPolyData *, vtkPolyData *);

#endif

  /// Prints information about the progress of the registration
  virtual void Print() = 0;

  /// Guess parameters
  virtual void GuessParameter() = 0;

  /// Read registration parameters from file
  virtual void Read (char *);

  /// Parse parameter line
  virtual bool Read(char *, char *, int &);

  /// Write registration parameters to file
  virtual void Write(char *);

  /// Write parameters to stream
  virtual void Write(ostream &);

  // Access parameters
  virtual SetMacro(DebugFlag, int);
  virtual GetMacro(DebugFlag, int);
  virtual SetMacro(TargetPadding, int);
  virtual GetMacro(TargetPadding, int);
  virtual SetMacro(OptimizationMethod, irtkOptimizationMethod);
  virtual GetMacro(OptimizationMethod, irtkOptimizationMethod);

};

inline void irtkMultipleImageRegistration2::SetInput(irtkRealImage **target, irtkRealImage **source, int n)
{
    int i;

    _numberOfImages = n;
    _target = new irtkRealImage*[_numberOfImages];
    _source = new irtkRealImage*[_numberOfImages];
    for (i = 0; i < _numberOfImages; i++) {
        _target[i] = target[i];
        _source[i] = source[i];
    }
}

inline void irtkMultipleImageRegistration2::Debug(string message)
{
  if (_DebugFlag == true) cout << message << endl;
}

inline void irtkMultipleImageRegistration2::SetWeighting(double **weight)
{
    _weight = weight;
}

#ifdef HAS_VTK

inline void irtkMultipleImageRegistration2::SetLandmarks (vtkPolyData * target, vtkPolyData * source)
{
    _ptarget = target;
    _psource = source;
    _pgradient = vtkPolyData::New();
    _pgradient->DeepCopy(_psource);
}

#endif

#include <irtkMultipleImageRigidRegistration2.h>
#include <irtkMultipleImageFreeFormRegistration2.h>

#endif

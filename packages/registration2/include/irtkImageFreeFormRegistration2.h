/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2009 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#ifndef _IRTKIMAGEFREEFORMREGISTRATION2_H

#define _IRTKIMAGEFREEFORMREGISTRATION2_H

/**
 * Filter for non-rigid registration based on voxel similarity measures.
 *
 * This class implements a registration filter for the non-rigid registration
 * of two images. The basic algorithm is described in Rueckert et al., IEEE
 * Transactions on Medical Imaging, In press.
 *
 */

class irtkImageFreeFormRegistration2 : public irtkImageRegistration2
{

protected:

  /// Pointer to the local transformation which is currently optimized
  irtkBSplineFreeFormTransformation *_affd;

  /// Pointer to the global transformation which is constant
  irtkMultiLevelFreeFormTransformation *_mffd;

  /// Pointer to Adjugate jacobian matrix
  irtkMatrix *_adjugate;

  /// Pointer to jacobian determine
  double *_determine;

  /// Smoothness parameter for non-rigid registration
  double _Lambda1;

  /// Volume preservation parameter for non-rigid registration
  double _Lambda2;

  /// Topology preservation parameter for non-rigid registration
  double _Lambda3;

  /// Control point spacing in the x-direction
  double _DX;

  /// Control point spacing in the y-direction
  double _DY;

  /// Control point spacing in the z-direction
  double _DZ;

  /// Subdivide FFD between resolution levels
  bool _Subdivision;

  /// Registration mode
  irtkImageFreeFormRegistrationMode _Mode;

  /// Multilevel mode
  bool _MFFDMode;

  /// Initial set up for the registration
  virtual void Initialize();

  /// Initial set up for the registration
  virtual void Initialize(int);

  /// Final set up for the registration
  virtual void Finalize();

  /// Final set up for the registration
  virtual void Finalize(int);

  /// Update state of the registration based on current transformation estimate
  virtual void Update();

    /** Evaluates the smoothness preservation term. */
  virtual double SmoothnessPenalty();

  /** Evaluates the smoothness term. */
  virtual double SmoothnessPenalty(int);

  /** Evaluates the volume preservation term. */
  virtual double VolumePreservationPenalty();

  /** Evaluates the volume preservation term. */
  virtual void VolumePreservationPenalty(int,double*);

  /** Evaluates the topology preservation term. */
  virtual double TopologyPreservationPenalty();

  /** Evaluates the topology preservation term. */
  virtual double TopologyPreservationPenalty(int);

  /** Evaluates the registration. This function evaluates the registration by
   *  looping over the target image and interpolating the transformed source
   *  image while filling the joint histogram. This function returns the value
   *  of the similarity measure using Similarity().
   */
  virtual double Evaluate();

  /// Evaluate the gradient of the similarity measure for the current transformation.
  virtual double EvaluateGradient(double *);

public:

  /// Constructor
  irtkImageFreeFormRegistration2();

  /// Set output for the registration filter
  virtual void SetOutput(irtkTransformation *);

  /// Runs the registration filter
  virtual void Run();

  /// Returns the name of the class
  virtual const char *NameOfClass();

  /// Print some information
  virtual void Print();

  /// Guess parameters
  virtual void GuessParameter();

  /// Read single line of registration parameters
  virtual bool Read(char *, char *, int &);

  /// Write registration parameters to file
  virtual void Write(ostream &);

  // Access parameters for control point space
  virtual SetMacro(DX, double);
  virtual GetMacro(DX, double);
  virtual SetMacro(DY, double);
  virtual GetMacro(DY, double);
  virtual SetMacro(DZ, double);
  virtual GetMacro(DZ, double);

  // Access parameters for registration mode
  virtual SetMacro(Mode, irtkImageFreeFormRegistrationMode);
  virtual GetMacro(Mode, irtkImageFreeFormRegistrationMode);

  virtual SetMacro(MFFDMode, bool);
  virtual GetMacro(MFFDMode, bool);
};

inline void irtkImageFreeFormRegistration2::SetOutput(irtkTransformation *transformation)
{
  // Print debugging information
  this->Debug("irtkImageFreeFormRegistration::SetOutput");

  if (strcmp(transformation->NameOfClass(),
             "irtkMultiLevelFreeFormTransformation") != 0) {
    cerr << "irtkImageFreeFormRegistration::SetOutput: Transformation must be "
         << "irtkMultiLevelFreeFormTransformation" << endl;
    exit(0);
  }
  _transformation = transformation;
}

inline const char *irtkImageFreeFormRegistration2::NameOfClass()
{
  return "irtkFreeFormRegistration";
}

inline void irtkImageFreeFormRegistration2::Print()
{}

#endif

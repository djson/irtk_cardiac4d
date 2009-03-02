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

#include <irtkFileToImage.h>

irtkFileToImage::irtkFileToImage()
{
  _type  = IRTK_VOXEL_UNKNOWN;
  _slope = 1.;
  _intercept = 0.;
  _reflectX = False;
  _reflectY = False;
  _reflectZ = False;
  _debug = True;
  _start = 0;
  _imagename = NULL;

}

irtkFileToImage::~irtkFileToImage()
{
  _type  = IRTK_VOXEL_UNKNOWN;
  _slope = 1.;
  _intercept = 0.;
  _reflectX = False;
  _reflectY = False;
  _reflectZ = False;
  _debug = True;
  _start = 0;
  if (_imagename != NULL) free(_imagename);
}

irtkFileToImage *irtkFileToImage::New(const char *imagename)
{
  irtkFileToImage *reader = NULL;

  // Check format for GIPL
  if (irtkFileGIPLToImage::CheckHeader(imagename)) {
    reader = new irtkFileGIPLToImage;
    reader->SetInput(imagename);
    return reader;
  }

#ifdef HAS_NIFTI
  // Check format for NIFTI
  if (irtkFileNIFTIToImage::CheckHeader(imagename)) {
    reader = new irtkFileNIFTIToImage;
    reader->SetInput(imagename);
    return reader;
  }
#endif

  // Check format for ANALYZE
  if (irtkFileANALYZEToImage::CheckHeader(imagename)) {
    reader = new irtkFileANALYZEToImage;
    reader->SetInput(imagename);
    return reader;
  }

  // Check format for VTK
  if (irtkFileVTKToImage::CheckHeader(imagename)) {
    reader = new irtkFileVTKToImage;
    reader->SetInput(imagename);
    return reader;
  }

  // Check format for PGM
  if (irtkFilePGMToImage::CheckHeader(imagename)) {
    reader = new irtkFilePGMToImage;
    reader->SetInput(imagename);
    return reader;
  }

  // Check for error
  if (reader == NULL) {
    cerr << "irtkFileToImage::New: Unknown file format " << imagename
    << endl;
    exit(1);
  }

  return reader;
}

int irtkFileToImage::GetDebugFlag()
{
  return _debug;
}

void irtkFileToImage::PutDebugFlag(int debug)
{
  _debug = debug;
}

const char *irtkFileToImage::NameOfClass()
{
  return "irtkFileToImage";
}

void irtkFileToImage::SetInput(const char *imagename)
{
  // Close old file
  this->Close();

  // Delete old file name
  if (_imagename != NULL) free(_imagename);

  // Copy new file name
  _imagename = strdup(imagename);

  // Open new file for reading
  this->Open(_imagename);

  // Read header
  this->ReadHeader();
}

irtkImage *irtkFileToImage::GetOutput()
{
  int x, y, z, t;
  irtkImage *output = NULL;

  // Bring image to correct size
  switch (_type) {
  case IRTK_VOXEL_CHAR: {

      // Allocate image
      output = new irtkGenericImage<char>(_attr);

      // Read data
      this->ReadAsChar((char *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      char *ptr = (char *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

  case IRTK_VOXEL_UNSIGNED_CHAR: {

      // Allocate image
      output = new irtkGenericImage<unsigned char>(_attr);

      // Read data
      this->ReadAsUChar((unsigned char *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      unsigned char *ptr = (unsigned char *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

  case IRTK_VOXEL_SHORT: {

      // Allocate image
      output = new irtkGenericImage<short>(_attr);

      // Read data
      this->ReadAsShort((short *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      short *ptr = (short *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

  case IRTK_VOXEL_UNSIGNED_SHORT: {

      // Allocate image
      output = new irtkGenericImage<unsigned short>(_attr);

      // Read data
      this->ReadAsUShort((unsigned short *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      unsigned short *ptr = (unsigned short *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

  case IRTK_VOXEL_FLOAT: {

      // Allocate image
      output = new irtkGenericImage<float>(_attr);

      // Read data
      this->ReadAsFloat((float *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      float *ptr = (float *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

  case IRTK_VOXEL_DOUBLE: {

      // Allocate image
      output = new irtkGenericImage<double>(_attr);

      // Read data
      this->ReadAsDouble((double *)output->GetScalarPointer(), output->GetNumberOfVoxels(), _start);

      // Scale data
      double *ptr = (double *)output->GetScalarPointer();
      for (t = 0; t < _attr._t; t++) {
        for (z = 0; z < _attr._z; z++) {
          for (y = 0; y < _attr._y; y++) {
            for (x = 0; x < _attr._x; x++) {
              *ptr = *ptr * _slope + _intercept;
              ptr++;
            }
          }
        }
      }
    }
    break;

   default:
      cout << "irtkFileToImage::GetOutput: Unknown voxel type" << endl;
  }

  // Reflect if necessary
  if (_reflectX == True) output->ReflectX();
  if (_reflectY == True) output->ReflectY();
  if (_reflectZ == True) output->ReflectZ();

  return output;
}

void irtkFileToImage::Print()
{
  cout << "Name of class is " << this->NameOfClass() << endl;
  cout << "File name is " << _imagename << endl;
  cout << "Image dimensions are " << _attr._x << " " << _attr._y << " " << _attr._z << " " << _attr._t << endl;
  cout << "Image has " << _bytes << " bytes per voxel" << endl;
  cout << "Voxel dimensions are " << _attr._dx << " " << _attr._dy << " "
  << _attr._dz << " " << _attr._dt << endl;
  cout << "Voxel type is ";
  switch (_type) {
    case IRTK_VOXEL_CHAR:
      cout << "char" << endl;
      break;
    case IRTK_VOXEL_UNSIGNED_CHAR:
      cout << "unsigned char" << endl;
      break;
    case IRTK_VOXEL_SHORT:
      cout << "short" << endl;
      break;
    case IRTK_VOXEL_UNSIGNED_SHORT:
      cout << "unsigned short" << endl;
      break;
    case IRTK_VOXEL_INT:
      cout << "int" << endl;
      break;
    case IRTK_VOXEL_UNSIGNED_INT:
      cout << "unsigned int" << endl;
      break;
    case IRTK_VOXEL_FLOAT:
      cout << "float" << endl;
      break;
    case IRTK_VOXEL_DOUBLE:
      cout << "double" << endl;
      break;
    default:
      cout << "unknown" << endl;
  }
}

void irtkFileToImage::Debug(char *message)
{
  if (_debug) cerr << message << endl;
}


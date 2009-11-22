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

irtkHistogram_2D::irtkHistogram_2D(const irtkHistogram_2D &h)
{
  int i, j;

  _min_x   = h._min_x;
  _min_y   = h._min_y;
  _max_x   = h._max_x;
  _max_y   = h._max_y;
  _width_x = h._width_x;
  _width_y = h._width_y;
  _nbins_x = h._nbins_x;
  _nbins_y = h._nbins_y;
  _nsamp   = h._nsamp;
  if ((_nbins_x < 1) || (_nbins_y < 1)) {
    cerr << "irtkHistogram_2D::irtkHistogram_2D: Should have at least one bin";
    exit(1);
  }
  _bins  = Allocate(_bins, _nbins_x, _nbins_y);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = h._bins[j][i];
    }
  }
}

irtkHistogram_2D::irtkHistogram_2D(int nbins_x, int nbins_y)
{
  int i, j;

  if ((nbins_x < 1) || (nbins_y < 1)) {
    cerr << "irtkHistogram_2D::irtkHistogram_2D: Should have at least one bin";
    exit(1);
  }
  _min_x   = 0;
  _min_y   = 0;
  _max_x   = nbins_x;
  _max_y   = nbins_y;
  _width_x = 1;
  _width_y = 1;
  _nbins_x = nbins_x;
  _nbins_y = nbins_y;
  _nsamp   = 0;
  _bins    = Allocate(_bins, _nbins_x, _nbins_y);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = 0;
    }
  }
}

irtkHistogram_2D::irtkHistogram_2D(double min_x, double max_x, double width_x,
                                   double min_y, double max_y, double width_y)
{
  int i, j;

  _min_x   = min_x;
  _min_y   = min_y;
  _max_x   = max_x;
  _max_y   = max_y;
  _nbins_x = round((max_x - min_x) / width_x);
  _nbins_y = round((max_y - min_y) / width_y);
  _width_x = (_max_x - _min_x) / (double)_nbins_x;
  _width_y = (_max_y - _min_y) / (double)_nbins_y;
  _nsamp = 0;
  if ((_nbins_x < 1) || (_nbins_y < 1)) {
    cerr << "irtkHistogram_2D::irtkHistogram_2D: Should have at least one bin";
    exit(1);
  }
  _bins  = Allocate(_bins, _nbins_x, _nbins_y);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = 0;
    }
  }
}

irtkHistogram_2D::~irtkHistogram_2D()
{
  if ((_nbins_x > 0) && (_nbins_y > 0)) {
    Deallocate(_bins);
    _bins = NULL;
  }
  _nbins_x = 0;
  _nbins_y = 0;
  _min_x   = 0;
  _min_y   = 0;
  _max_x   = 0;
  _max_y   = 0;
  _width_x = 0;
  _width_y = 0;
  _nsamp   = 0;
}

void irtkHistogram_2D::Reset()
{
  int i, j;

  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = 0;
    }
  }
  _nsamp = 0;
}

void irtkHistogram_2D::Reset(const irtkHistogram_2D &h)
{
  int i, j;

  if ((_nbins_x != h._nbins_x) || (_nbins_y != h._nbins_y)) {
    // Deallocate old memory
    _bins = Deallocate(_bins);
    // Allocte new memory
    _bins = Allocate(_bins, h._nbins_x, h._nbins_y);
  }
  _min_x   = h._min_x;
  _min_y   = h._min_y;
  _max_x   = h._max_x;
  _max_y   = h._max_y;
  _width_x = h._width_x;
  _width_y = h._width_y;
  _nbins_x = h._nbins_x;
  _nbins_y = h._nbins_y;
  _nsamp   = h._nsamp;
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = h._bins[j][i];
    }
  }
}
void irtkHistogram_2D::PutMin(double min_x, double min_y)
{
  _min_x = min_x;
  _min_y = min_y;
  _width_x = (_max_x - _min_x) / (double)_nbins_x;
  _width_y = (_max_y - _min_y) / (double)_nbins_y;
  this->Reset();
}

void irtkHistogram_2D::GetMin(double *min_x, double *min_y) const
{
  *min_x = _min_x;
  *min_y = _min_y;
}

void irtkHistogram_2D::PutMax(double max_x, double max_y)
{
  _max_x = max_x;
  _max_y = max_y;
  _width_x = (_max_x - _min_x) / (double)_nbins_x;
  _width_y = (_max_y - _min_y) / (double)_nbins_y;
  this->Reset();
}

void irtkHistogram_2D::GetMax(double *max_x, double *max_y) const
{
  *max_x = _max_x;
  *max_y = _max_y;
}

void irtkHistogram_2D::PutWidth(double width_x, double width_y)
{
  int i, j;

  if ((_nbins_x > 0) && (_nbins_y > 0)) {
    Deallocate(_bins);
    _bins = NULL;
  }
  _nbins_x = round((_max_x - _min_x) / width_x);
  _nbins_y = round((_max_y - _min_y) / width_y);
  _width_x = (_max_x - _min_x) / (double)_nbins_x;
  _width_y = (_max_y - _min_y) / (double)_nbins_y;
  if ((_nbins_x < 1) || (_nbins_y < 1)) {
    cerr << "irtkHistogram_2D::PutWidth: Should have at least one bin";
    exit(1);
  }
  _bins  = Allocate(_bins, _nbins_x, _nbins_y);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = 0;
    }
  }
  _nsamp = 0;
}

void irtkHistogram_2D::GetWidth(double *width_x, double *width_y) const
{
  *width_x = _width_x;
  *width_y = _width_y;
}

void irtkHistogram_2D::PutNumberOfBins(int nbins_x, int nbins_y)
{
  int i, j;

  if ((_nbins_x > 0) && (_nbins_y > 0)) {
    Deallocate(_bins);
    _bins = NULL;
  }
  _nbins_x = nbins_x;
  _nbins_y = nbins_y;
  _width_x = (_max_x - _min_x) / (double)_nbins_x;
  _width_y = (_max_y - _min_y) / (double)_nbins_y;
  if ((_nbins_x < 1) || (_nbins_y < 1)) {
    cerr << "irtkHistogram_2D::PutWidth: Should have at least one bin";
    exit(1);
  }
  _bins  = Allocate(_bins, _nbins_x, _nbins_y);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      _bins[j][i] = 0;
    }
  }
  _nsamp = 0;
}

void irtkHistogram_2D::GetNumberOfBins(int *nbins_x, int *nbins_y) const
{
  *nbins_x = _nbins_x;
  *nbins_y = _nbins_y;
}

void irtkHistogram_2D::AddSample(double x, double y)
{
  int i, j;

  if (x < _min_x) return;
  if (x > _max_x) return;
  if (y < _min_y) return;
  if (y > _max_y) return;
  i = round(_nbins_x * (x - _min_x - 0.5*_width_x) / (_max_x - _min_x));
  j = round(_nbins_y * (y - _min_y - 0.5*_width_y) / (_max_y - _min_y));
  if (i < 0) i = 0;
  if (j < 0) j = 0;
  if (i > _nbins_x-1) i = _nbins_x - 1;
  if (j > _nbins_y-1) j = _nbins_y - 1;
  _bins[j][i]++;
  _nsamp++;
}

void irtkHistogram_2D::DelSample(double x, double y)
{
  int i, j;

  if (x < _min_x) return;
  if (x > _max_x) return;
  if (y < _min_y) return;
  if (y > _max_y) return;
  i = round(_nbins_x * (x - _min_x - 0.5*_width_x) / (_max_x - _min_x));
  j = round(_nbins_y * (y - _min_y - 0.5*_width_y) / (_max_y - _min_y));
  if (i < 0) i = 0;
  if (j < 0) j = 0;
  if (i > _nbins_x-1) i = _nbins_x - 1;
  if (j > _nbins_y-1) j = _nbins_y - 1;
  _bins[j][i]--;
  _nsamp--;
}

void irtkHistogram_2D::HistogramX(irtkHistogram_1D &histogram)
{
  int i, j;

  // Reset irtkHistogram
  histogram.PutNumberOfBins(_nbins_x);
  histogram.PutMin(_min_x);
  histogram.PutMax(_max_x);

  for (i = 0; i < _nbins_x; i++) {
    for (j = 0; j < _nbins_y; j++) {
      histogram.Add(i, _bins[j][i]);
    }
  }
}

void irtkHistogram_2D::HistogramY(irtkHistogram_1D &histogram)
{
  int i, j;

  // Reset histogram
  histogram.PutNumberOfBins(_nbins_y);
  histogram.PutMin(_min_y);
  histogram.PutMax(_max_y);

  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      histogram.Add(j, _bins[j][i]);
    }
  }
}

double irtkHistogram_2D::MeanX()
{
  int i, j;
  double val, tmp;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::MeanX: No samples in Histogram" << endl;
    return 0;
  }
  val = 0;
  for (i = 0; i < _nbins_x; i++) {
    tmp = this->BinToValX(i);
    for (j = 0; j < _nbins_y; j++) {
      val += _bins[j][i] * tmp;
    }
  }
  return val / (double)_nsamp;
}

double irtkHistogram_2D::MeanY()
{
  int i, j;
  double val, tmp;

  if (_nsamp == 0) {
    cerr << "Histogram_2D::MeanY: No samples in Histogram" << endl;
    return 0;
  }
  val = 0;
  for (j = 0; j < _nbins_y; j++) {
    tmp = this->BinToValY(j);
    for (i = 0; i < _nbins_x; i++) {
      val += _bins[j][i] * tmp;
    }
  }
  return val / (double)_nsamp;
}

double irtkHistogram_2D::VarianceX()
{
  int i;
  double val;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::VarianceX: No samples in Histogram" << endl;
    return 0;
  }
  val  = 0;
  for (i = 0; i < _nbins_x; i++) {
    val += this->MarginalProbabilityX(i) * pow(this->BinToValX(i), 2.0);
  }
  return val - pow(this->MeanX(), 2.0);
}

double irtkHistogram_2D::VarianceY()
{
  int i;
  double val;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::VarianceY: No samples in Histogram" << endl;
    return 0;
  }
  val  = 0;
  for (i = 0; i < _nbins_y; i++) {
    val += this->MarginalProbabilityY(i) * pow(this->BinToValY(i), 2.0);
  }
  return val - pow(this->MeanY(), 2.0);
}

double irtkHistogram_2D::StandardDeviationX()
{
  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::StandardDeviationX: No samples in Histogram" << endl;
    return 0;
  }
  return sqrt(this->VarianceX());
}

double irtkHistogram_2D::StandardDeviationY()
{
  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::StandardDeviationY: No samples in Histogram" << endl;
    return 0;
  }
  return sqrt(this->VarianceY());
}

double irtkHistogram_2D::Covariance()
{
  int i, j;
  double val, mean_x, mean_y;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::Covariance: No samples in Histogram" << endl;
    return 0;
  }
  val  = 0;
  mean_x = this->MeanX();
  mean_y = this->MeanY();
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      val += _bins[j][i] *
             (this->BinToValX(i) - mean_x) *
             (this->BinToValY(j) - mean_y);
    }
  }
  return val / (double)_nsamp;
}

double irtkHistogram_2D::EntropyX()
{
  int i, j, *ptr;
  double val;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::EntropyX: No samples in Histogram" << endl;
    return 0;
  }

  // Do some initial calculations
  int *tmp = new int[_nbins_x];
  for (i = 0; i < _nbins_x; i++) tmp[i] = 0;

  // Calculate entropy
  val = 0;
  ptr = &(_bins[0][0]);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      tmp[i] += *ptr;
      ptr++;
    }
  }

  // Do some final calculations
  for (i = 0; i < _nbins_x; i++) if (tmp[i] > 0) val += tmp[i] * log(static_cast<double>(tmp[i]));

  delete []tmp;
  return - val / (double)_nsamp + log(static_cast<double>(_nsamp));
}

double irtkHistogram_2D::EntropyY()
{
  int i, j, tmp, *ptr;
  double val;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::EntropyY: No samples in Histogram" << endl;
    return 0;
  }

  // Calculate entropy
  val = 0;
  ptr = &(_bins[0][0]);
  for (j = 0; j < _nbins_y; j++) {
    tmp = 0;
    for (i = 0; i < _nbins_x; i++) {
      tmp += *ptr;
      ptr++;
    }
    if (tmp > 0) val += tmp * log(static_cast<double>(tmp));
  }
  return - val / (double)_nsamp + log(static_cast<double>(static_cast<double>(_nsamp)));
}

double irtkHistogram_2D::JointEntropy()
{
  double val;
  int i, j, *ptr;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::JointEntropy: No samples in Histogram" << endl;
    return 0;
  }

  // Calculate joint entropy
  val = 0;
  ptr = &(_bins[0][0]);
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      if (*ptr > 0) {
        val += *ptr * log(static_cast<double>(*ptr));
      }
      ptr++;
    }
  }
  return - val / (double)_nsamp + log(static_cast<double>(_nsamp));
}

double irtkHistogram_2D::ConditionalMeanXY(int i)
{
  int j;
  double p, m;

  if ((i < 0) || (i > _nbins_y-1)) {
    cerr << "irtkHistogram_2D::ConditionalMeanXY: No such bin " << i << endl;
    return 0;
  }
  m = 0;
  p = 0;
  for (j = 0; j < _nbins_x; j++) {
    m += this->JointProbability(j, i) * this->BinToValX(j);
    p += this->JointProbability(j, i);
  }
  if (p > 0) {
    return m / p;
  } else {
    return 0;
  }
}

double irtkHistogram_2D::ConditionalMeanYX(int i)
{
  int j;
  double p, m;

  if ((i < 0) || (i > _nbins_x-1)) {
    cerr << "irtkHistogram_2D::ConditionalMeanYX: No such bin " << i << endl;
    return 0;
  }
  m = 0;
  p = 0;
  for (j = 0; j < _nbins_y; j++) {
    m += this->JointProbability(i, j) * this->BinToValY(j);
    p += this->JointProbability(i, j);
  }
  if (p > 0) {
    return m / p;
  } else {
    return 0;
  }
}

double irtkHistogram_2D::CorrelationRatioXY()
{
  int i;
  double c, m;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::CorrelationRatioXY: No samples in Histogram";
    cerr << endl;
    return 0;
  }

  c = 0;
  m = this->MeanX();
  for (i = 0; i < _nbins_y; i++) {
    c += this->MarginalProbabilityY(i) * pow(this->ConditionalMeanXY(i)-m, 2.0);
  }
  return (c / this->VarianceX());
}

double irtkHistogram_2D::CorrelationRatioYX()
{
  int i;
  double c, m;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::CorrelationRatioYX: No samples in Histogram";
    cerr << endl;
    return 0;
  }

  c = 0;
  m = this->MeanY();
  for (i = 0; i < _nbins_x; i++) {
    c += this->MarginalProbabilityX(i) * pow(this->ConditionalMeanYX(i)-m, 2.0);
  }
  return (c / this->VarianceY());
}

double irtkHistogram_2D::MutualInformation()
{
  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::MutualInformation: No samples in Histogram" << endl;
    return 0;
  }

  return this->EntropyX() + this->EntropyY() - this->JointEntropy();
}

double irtkHistogram_2D::NormalizedMutualInformation()
{
  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::NormalizedMutualInformation: No samples in Histogram" << endl;
    return 0;
  }
  return (this->EntropyX() + this->EntropyY()) / this->JointEntropy();
}

double irtkHistogram_2D::CrossCorrelation()
{
  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::CrossCorrelation: No samples in Histogram" << endl;
    return 0;
  }
  return fabs(this->Covariance() / (sqrt(this->VarianceX()) *
                                    sqrt(this->VarianceY())));
}

double irtkHistogram_2D::SumsOfSquaredDifferences()
{
  int i, j;
  double val_x, val_y, ssd;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::SumsOfSquaredDifferences: ";
    cerr << "No samples in Histogram" << endl;
    return 0;
  }

  ssd   = 0;
  val_x = this->BinToValX(0);
  val_y = this->BinToValY(0);
  for (j = 0; j < _nbins_y; j++) {
    val_x = this->BinToValX(0);
    for (i = 0; i < _nbins_x; i++) {
      ssd   += _bins[j][i] * (val_x - val_y) * (val_x - val_y);
      val_x += _width_x;
    }
    val_y += _width_y;
  }
  return ssd;
}

double irtkHistogram_2D::LabelConsistency()
{
  int i;
  long int n;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::LabelConsistency: No samples in Histogram" << endl;
    return 0;
  }
  if (_nbins_x != _nbins_y) {
    cerr << "irtkHistogram_2D::LabelConsistency: Histogram must have equal number of bins in X and Y" << endl;
    return 0;
  }

  n = 0;
  for (i = 0; i < _nbins_x; i++) {
    n += _bins[i][i];
  }
  return n / (double)_nsamp;
}

double irtkHistogram_2D::Kappa()
{
  int i, j;

  if (_nsamp == 0) {
    cerr << "irtkHistogram_2D::Kappa: No samples in Histogram" << endl;
    return 0;
  }
  if (_nbins_x != _nbins_y) {
    cerr << "irtkHistogram_2D::Kappa: Histogram must have equal number of bins in X and Y" << endl;
    return 0;
  }

  long int *col_sum = new long int[_nbins_x] ;
  long int *row_sum = new long int[_nbins_x] ;
  for (j = 0; j < _nbins_x; j++) {
    col_sum[j] = 0;
    row_sum[j] = 0;
    for (i = 0; i < _nbins_x; i++) {
      col_sum[j] += _bins[i][j];
      row_sum[j] += _bins[j][i];
    }
  }

  double po = 0.0;
  double pe = 0.0;
  for (j = 0; j < _nbins_x; j++) {
    po += _bins[j][j];
    pe += col_sum[j] * (double)row_sum[j];
  }
  po /= _nsamp;
  pe /= _nsamp * (double)_nsamp;

  delete []row_sum;
  delete []col_sum;
  return ((po-pe)/(1-pe));
}

void irtkHistogram_2D::Read(char *filename)
{
  int i, j;
  char buffer[255];

  ifstream from(filename);
  if (!from) {
    cerr << "irtkHistogram_2D::Read: Can't open file " << filename << "\n";
    exit(1);
  }
  if ((_nbins_x > 0) && (_nbins_y > 0)) {
    Deallocate(_bins);
    _nbins_x = 0;
    _nbins_y = 0;
  }

  from >> buffer;
  if (strcmp(buffer, "irtkHistogram_2D") != 0) {
    cerr << "irtkHistogram_2D::Read: Invalid format" << endl;
    exit(1);
  }

  // Read no. of bins
  from >> _nbins_x;
  from >> _nbins_y;

  // Read no. of samples
  from >> _nsamp;

  // Read min and max of bins
  from >> _min_x;
  from >> _max_x;
  from >> _min_y;
  from >> _max_y;

  // Read width of bins
  from >> _width_x;
  from >> _width_y;

  if ((_nbins_x > 0) && (_nbins_y > 0)) {
    _bins = Allocate(_bins, _nbins_x, _nbins_y);
  }
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      from >> _bins[j][i];
    }
  }
}

void irtkHistogram_2D::Write(char *filename)
{
  int i, j;

  ofstream to(filename);
  if (!to) {
    cerr << "irtkHistogram_2D::Write: Can't open file " << filename << "\n";
    exit(1);
  }
  to << "irtkHistogram_2D\n";
  to << _nbins_x << " "
  << _nbins_y << " "
  << _nsamp << " "
  << _min_x << " "
  << _max_x << " "
  << _min_y << " "
  << _max_y << " "
  << _width_x << " "
  << _width_y << endl;
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      to << _bins[j][i] << " ";
    }
    to << endl;
  }
}

void irtkHistogram_2D::Print()
{
  int i, j;

  cout << _nbins_x << " "
       << _nbins_y << " "
       << _nsamp << " "
       << _min_x << " "
       << _max_x << " "
       << _min_y << " "
       << _max_y << " "
       << _width_x << " "
       << _width_y << endl;
  for (j = 0; j < _nbins_y; j++) {
    for (i = 0; i < _nbins_x; i++) {
      cout << _bins[j][i] << " ";
    }
    cout << endl;
  }
}

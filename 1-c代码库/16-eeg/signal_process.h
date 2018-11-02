#ifndef _SIGNAL_PROCESS_H_
#define _SIGNAL_PROCESS_H_


double filter_delta(double *x, double *y, int len, double raw_data);

double filter_theta(double *x, double *y, int len, double raw_data);

double filter_alpha(double *x, double *y, int len, double raw_data);

double filter_gamma(double *x, double *y, int len, double raw_data);


#endif // !_SIGNAL_PROCESS_H_


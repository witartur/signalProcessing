#ifndef SIGNAL_H
#define SIGNAL_H
void functionGenerator(int secondUserChoice, float *arrayCoefficient, float d1, float dx, float *resultY, float *resultX, int N);
void loadDomain(float *d1_ptr, float *d2_ptr);
void loadCoefficient(float *arrayCoefficient);
void loadSampling(float *dx_ptr);
int resultArraySize(float d1, float d2, float dx, int firstUserChoice, int amountOfAxisElements);
void interference(float *resultY, int N);
void medianFilter(float *resultY, int N);
void movingAverageFilter(float *resultY, int N);

#endif  // SIGNAL_H
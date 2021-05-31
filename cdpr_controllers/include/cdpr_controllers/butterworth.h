#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include <vector>
#include <math.h>

// this class implements a 2nd-order Butterworth low-pass filter

class Butterworth
{
public:
    Butterworth(double frequency, double dt)
    {
        const double ita =1.0/ tan(M_PI*frequency*dt);
        const double q=sqrt(2.0);
        b.resize(3);
        b[0] = 1.0 / (1.0 + q*ita + ita*ita);
        b[1]= 2*b[0];
        b[2]= b[0];
        a = {2.0 * (ita*ita - 1.0) * b[0], -(1.0 - q*ita + ita*ita) * b[0]};

        x.resize(3);
        y.resize(2);

    }
    void Filter(double &v)
    {
        // swap inputs
        x[2] = x[1];
        x[1] = x[0];
        x[0] = v;

        // compute output
        v = b[0]*x[0] +
                b[1]*x[1] +
                b[2]*x[2] +
                a[0]*y[0] +
                a[1]*y[1];

        // swap output
        y[1] = y[0];
        y[0] = v;
    }



protected:
    std::vector<double> a, b;
    std::vector<double> x, y;
};


// same but for nD-variables (std::vectors, Eigen lib vectores...)

class Butterworth_nD
{
public:
    // n identical filters
    Butterworth_nD(int size, double frequency, double dt)
    {
        filters.resize(size, Butterworth(frequency, dt));
    }

    // deduce n from given frequencies
    Butterworth_nD(std::vector<double> frequencies, double dt)
    {
        filters.clear();
        for(auto f: frequencies)
            filters.push_back(Butterworth(f, dt));
    }

    template <class T>
    void Filter(T &var)
    {
        for(unsigned int i=0;i<var.size();++i)
            filters[i].Filter(var.operator[](i));
    }

protected:
    std::vector<Butterworth> filters;
};



#endif // BUTTERWORTH_H

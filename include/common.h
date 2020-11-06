#pragma once

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>
#include <iostream>
#include <string>
#include <vector>

static const size_t numMeasurements = 7; // Количество элементво в строке

typedef dlib::matrix<double, numMeasurements, 1> sampleType;
typedef dlib::radial_basis_kernel<sampleType> kernelType;
typedef dlib::one_vs_one_trainer<dlib::any_trainer<sampleType>> typeTrainer;

std::ostream& operator<<(std::ostream& stream, sampleType value)
{
    std::ios_base::fmtflags fmtFl(stream.flags());
    stream << std::fixed;
    for (size_t i = 0; i < numMeasurements - 1; ++i)
    {
        stream << value(i);
        stream << ";";
    }
    stream << value(numMeasurements - 1) << std::endl;
    stream.setf(fmtFl);
    return stream;
}

sampleType parseLine(std::string line)
{
    std::stringstream sstream(line);

    double param;
    char delimiter;
    sampleType value;

    for (size_t i = 0; i < numMeasurements - 1; ++i)
    {
        sstream >> param;
        if (sstream.fail())
        {
            param = 0;
            sstream.clear(std::ios_base::goodbit);
        }
        sstream >> delimiter;
        if (delimiter != ';')
            throw std::logic_error("Error in format input!");

        value(i) = param;
    };

    sstream >> param;
    value(numMeasurements - 1) = param;
    sstream >> delimiter;

    if (!sstream.eof() && delimiter == ';')
    {
        sstream >> param;
        if (param == value(numMeasurements - 1) || (1 >= param))
        {
            value(numMeasurements - 1) = 0;
        }
        else
            value(numMeasurements - 1) = 1;
    }

    return value;
}

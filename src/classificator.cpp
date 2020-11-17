#include "classificator.h"

#include "common.h"
#include "version.h"

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>
#include <iostream>
#include <map>

int main(int argc, char** argv)
{
    std::cout << "Current classificator version: " << version() << std::endl;
    if (argc != 2)
    {
        std::cout << "usage rclst <modelname>" << std::endl;
        return -1;
    }

    std::string modelname(argv[1]);
    if (modelname.empty())
    {
        std::cout << "Wrong model_name! modelname can't be empty!" << std::endl;
        return -3;
    }

    std::string filename(modelname + ".dat");
    std::ifstream testFile;
    testFile.open(filename.c_str());
    if (!testFile.good())
    {
        std::cout << "Can't open file with classificator for" << modelname << "!" << std::endl;
        std::cout << "Please, enter modelname without .dat at the end" << std::endl;
        return -4;
    }
    testFile.close();

    dlib::one_vs_one_decision_function<typeTrainer, dlib::decision_function<kernelType>> classificator;
    dlib::deserialize(filename) >> classificator;

    sampleType testSample;
    std::string input;
    while (std::getline(std::cin, input))
    {
        try
        {
            testSample = parseLine(input);

            long cluster = std::round(classificator(testSample));

            filename = modelname + ".c" + std::to_string(cluster);
            testFile.open(filename.c_str());
        }
        catch (std::exception& e)
        {
            std::cout << "Wrong input data! " << e.what() << std::endl;
            continue;
        }

        if (!testFile.good())
        {
            std::cout << "File for cluster " << filename << "  not found!";
            continue;
        }
        sampleType sampleInput;
        std::multimap<double, sampleType> clusterMap;

        while (std::getline(testFile, input))
        {
            try
            {
                sampleInput = parseLine(input);
            }
            catch (std::exception& e)
            {
                std::cout << "Error while reading cluster file: " << e.what() << std::endl;
            };

            double dist = distance(sampleInput, testSample);
            clusterMap.insert(std::make_pair(dist, sampleInput));
        }
        testFile.close();

        for (auto cluster : clusterMap)
        {
            std::cout << cluster.second;
        }
    };
    return 0;
}

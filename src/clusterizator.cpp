#include "clusterizator.h"

#include "common.h"
#include "version.h"

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>
#include <iostream>
#include <map>

int main(int argc, char** argv)
{
    std::cout << "Current clusterizator version: " << version() << std::endl;
    if (argc != 3)
    {
        std::cout << "usage rclst <num_clusters> <modelname>" << std::endl;
        return -1;
    }

    unsigned long num_clusters = std::stoul(argv[1]);
    if (num_clusters <= 1)
    {
        std::cout << "Wrong num_clusters! num_clusters should be more than 1!" << std::endl;
        return -2;
    }

    std::string modelname(argv[2]);
    if (modelname.empty())
    {
        std::cout << "Wrong model_name! modelname can't be empty!" << std::endl;
        return -3;
    }

    sampleType m;
    std::vector<sampleType> samples;
    std::string line;
    std::getline(std::cin, line);
    std::cout << "Receiving data..." << std::endl;
    while (!std::cin.eof())
    {
        try
        {
            m = parseLine(line);
            samples.push_back(m);
        }
        catch (std::logic_error& ex)
        {
            std::cout << "Wrong line: " << line << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cout << "Unexpected: " << ex.what() << std::endl;
        }
        std::getline(std::cin, line);
    };
    std::cout << "Data received: total " << samples.size() << " samples" << std::endl;
    std::cout << "Clusterization (kkmeans)" << std::endl;
    dlib::kcentroid<kernelType> kc(kernelType(0.01), 0.01, num_clusters);
    dlib::kkmeans<kernelType> test(kc);
    std::vector<sampleType> initial_centers;

    test.set_number_of_centers(num_clusters);
    dlib::pick_initial_centers(num_clusters, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers, 1000);

    std::vector<unsigned long> assignments = spectral_cluster(kernelType(0.01), samples, num_clusters);
    std::cout << "Saving clusters in files" << std::endl;
    saveClustersInFiles(samples, assignments, modelname, num_clusters);
    std::cout << "Clusters saved" << std::endl;

    std::cout << "Creating Type trainer" << std::endl;
    typeTrainer trainer;

    dlib::krr_trainer<kernelType> trainer_linear;
    trainer_linear.set_kernel(kernelType(0.01));
    trainer.set_trainer(trainer_linear);
    trainer.be_verbose();

    std::vector<double> labels;
    std::copy(assignments.begin(), assignments.end(), back_inserter(labels));
    std::cout << "Type training started" << std::endl;
    dlib::one_vs_one_decision_function<typeTrainer, dlib::decision_function<kernelType>> des_func = trainer.train(samples, labels);
    dlib::serialize(modelname + ".dat") << des_func;
    std::cout << "Model saved at " << modelname << ".dat";
    return 0;
}

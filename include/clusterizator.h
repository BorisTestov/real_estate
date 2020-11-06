#pragma once
#include "common.h"

#include <vector>

void saveClustersInFiles(std::vector<sampleType>& samples,
                         std::vector<unsigned long>& clusters,
                         std::string model_name,
                         unsigned long num_clusters)
{
    std::vector<std::shared_ptr<std::ofstream>> vecFiles;
    vecFiles.reserve(num_clusters);
    for (size_t cluster = 0; cluster < num_clusters; ++cluster)
    {
        vecFiles.push_back(std::make_shared<std::ofstream>(model_name + ".c" + std::to_string(cluster)));
    }
    for (size_t idx = 0; idx < samples.size() && idx < clusters.size(); ++idx)
    {
        unsigned long cluster(clusters.at(idx));
        sampleType sample(samples.at(idx));

        std::cout << cluster << ":" << sample;

        std::ofstream& file(*vecFiles[cluster]);
        file << samples.at(idx);
    }
}
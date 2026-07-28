#pragma once

#include "hnswlib.h"
#include <string>
#include <vector>
#include <memory>
#include <omp.h>

class HnswIndex {
public:
    HnswIndex(size_t dimension, size_t max_elements, size_t M = 16, size_t ef_construction = 200) 
        : dim(dimension) {
        space = std::make_unique<hnswlib::InnerProductSpace>(dim);
        app_alg = std::make_unique<hnswlib::HierarchicalNSW<float>>(space.get(), max_elements, M, ef_construction);
    }

    // High-concurrency data injection using OpenMP
    void BulkInsert(const float* data, const std::vector<size_t>& ids, size_t num_elements) {
        #pragma omp parallel for schedule(dynamic)
        for (size_t i = 0; i < num_elements; ++i) {
            const float* vector_ptr = data + (i * dim);
            // hnswlib addPoint is natively thread-safe via internal node-level mutices
            app_alg->addPoint(vector_ptr, ids[i]);
        }
    }

    void Save(const std::string& filepath) {
        app_alg->saveIndex(filepath);
    }

    void Load(const std::string& filepath) {
        app_alg->loadIndex(filepath, space.get());
    }

    std::priority_queue<std::pair<float, hnswlib::labeltype>> Query(const float* query_vec, size_t k) {
        return app_alg->searchKnn(query_vec, k);
    }
private:
    size_t dim;
    std::unique_ptr<hnswlib::InnerProductSpace> space;
    std::unique_ptr<hnswlib::HierarchicalNSW<float>> app_alg;
};

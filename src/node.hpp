#pragma once
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include "common.hpp"
#include "sampler.hpp"

using namespace std;

class Node {
public:
    hashmap<id, Node*> _children;
    hashmap<id, vector<int>> _arrangement;
    Node *_parent;
    int _num_tables;
    int _num_customers;
    int _stop_count;
    int _pass_count;
    int _depth;
    id _token_id;
    Node(id token_id=0) {
        _num_tables = 0;
        _num_customers = 0;
        _stop_count = 0;
        _pass_count = 0;
        _token_id = token_id;
        _parent = NULL;
    }
    void add_customer_to_table(id token_id, int table_k, double g0, vector<double> &d_m, vector<double> &theta_m) {

    }
    void add_customer_to_new_table(id token_id, double g0, vector<double> &d_m, vector<double> &theta_m) {

    }
    bool add_customer(id token_id, double g0, vector<double> &d_m, vector<double> &theta_m, bool update_beta_count=true) {
        double d_u = d_m[_depth];
        double theta_u = theta_m[_depth];
        double parent_Pw = g0;
        if (_parent) {
            parent_Pw = _parent->compute_Pw(token_id, g0, d_m, theta_m);
        }
        auto itr = _arrangement.find(token_id);

        /* add customer to new table */
        if (itr == _arrangement.end()) {
            add_customer_to_new_table(token_id, g0, d_m, theta_m);
            if (update_beta_count) {
                // increment stop count
            }
            return true;
        }

        /* add customer to existing table */
        // num of customer per table
        vector<int> &num_customers_at_table = itr->second;
        // for normalizing prob, calculate summation of all prob
        double sum = 0;
        for (int k=0; k<num_customers_at_table.size(); ++k) {
            sum += std::max(0.0, num_customers_at_table[k] - d_u);
        }
        double t_u = _num_tables;
        sum += (theta_u + d_u * t_u) * parent_Pw;
        double normalizer = 1.0 / sum;
        double bernoulli = sampler::uniform(0, 1);
        double stack = 0;
        // calculate probabiliry of adding customer to all of table serving `w`
        for (int k=0; k<num_customers_at_table.size(); ++k) {
            stack += std::max(0.0, num_customers_at_table[k] - d_u) * normalizer;
            if (bernoulli <= stack) {
                add_customer_to_table(token_id, k, g0, d_m, theta_m);
                if (update_beta_count) {
                    // increment stop count
                }
                return true;
            }
        }
        add_customer_to_new_table(token_id, g0, d_m, theta_m);
        if (update_beta_count) {
            // increment stop count
        }
        return true;
    }
    double compute_Pw(id token_id, double g0, vector<double> &d_m, vector<double> &theta_m) {

    }
};
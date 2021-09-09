//
// Created by justnik on 28.08.2021.
//

#include <cmath>
#include "filters.hpp"

namespace filter {

    double MedianFilter::filter(double new_val) {
        if (data.size() < size) {
            data.push_back(new_val);
        } else {
            data[ind++ % size] = new_val;
        }
        auto v = data;
        auto m = v.begin() + v.size() / 2;
        std::nth_element(v.begin(), m, v.end());
        return v[v.size() / 2];
    }

    MedianFilter::MedianFilter(size_t size) : size(size) {
        data.reserve(size);
    }

    double RunningAvg::filter(double new_val) {
        if (data.size() < size) {
            data.push_back(new_val);
        } else {
            data[ind++ % size] = new_val;
        }
        double mean = 0;
        for (auto &&v: data) {
            mean += v;
        }
        return mean / static_cast<double>(data.size());
    }

    RunningAvg::RunningAvg(size_t size) : size(size) {
        data.reserve(size);
    }

    double ExpSmooth::filter(double new_val) {
        if (std::isnan(last_val)) {
            last_val = new_val;
        }
        auto res = new_val * coef + (1 - coef) * last_val;
        last_val = res;
        return res;
    }

    ExpSmooth::ExpSmooth(double coef) : coef(coef) {}

    ABFilter::ABFilter(double dt, double sigma_process, double sigma_noise) : dt(dt), sigma_process(sigma_process),
                                                                              sigma_noise(sigma_noise) {}

    double ABFilter::filter(double new_val) {
        static double xk1, vk1, a, b;
        static double xk, vk, rk;
        static double xm;

        double lambda = sigma_process * dt * dt / sigma_noise;
        double r = (4 + lambda - sqrt(8 * lambda + lambda * lambda)) / 4;
        a = 1.0 - r * r;
        b = 2.0 * (2.0 - a) - 4.0 * sqrt(1.0 - a);

        xm = new_val;
        xk = xk1 + vk1 * dt;
        vk = vk1;
        rk = xm - xk;
        xk += a * rk;
        vk += b * rk / dt;
        xk1 = xk;
        vk1 = vk;
        return xk1;
    }

    KalmanFilter::KalmanFilter(double dt, double errMeasure) : dt(dt), err_measure(errMeasure) {}

    double KalmanFilter::filter(double new_val) {
        static double err_estimate = err_measure;
        static double last_estimate;
        double kalman_gain, current_estimate;

        kalman_gain = err_estimate / (err_estimate + err_measure);
        current_estimate = last_estimate + kalman_gain * (new_val - last_estimate);
        err_estimate = (1.0 - kalman_gain) * err_estimate + fabs(last_estimate - current_estimate);
        last_estimate = current_estimate;
        return current_estimate;
    }
}
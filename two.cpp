#include <iostream>
#include <chrono>
#include <numeric> // Required for std::gcd
#include <random>
#include <vector>

// 1. Your production-grade custom implementation
constexpr unsigned long long ll_abs(long long x) {
    return (x < 0) ? -static_cast<unsigned long long>(x) : x;
}

constexpr unsigned long long custom_stein_gcd(long long _a, long long _b) {
    unsigned long long a = ll_abs(_a);
    unsigned long long b = ll_abs(_b);

    const char a_tz = __builtin_ctz(a);
    const char b_tz = __builtin_ctz(b);
    char shifts = (a_tz < b_tz)? a_tz : b_tz;
    a >>= a_tz;
    b >>= b_tz;

    while (a && b)
    {
        const bool a_min = a < b;
        const unsigned long long difference = (a_min)? (b - a) : (a - b);
        a = (a_min)? a : b;
        b = difference >> __builtin_ctzll(difference);
    }

    return (a + b) << shifts;
}

// Helper to prevent the compiler from optimizing out the loops entirely
void do_not_optimize(unsigned long long val) {
    asm volatile("" : : "g"(val) : "memory");
}

int main() {
    const int NUM_PAIRS = 5'000'000;
    std::cout << "Generating " << NUM_PAIRS << " random pairs...\n";

    // Initialize random number generation
    std::mt19937_64 rng(1337); // Fixed seed for identical test criteria
    std::uniform_int_distribution<long long> dist(LLONG_MIN, LLONG_MAX);

    std::vector<std::pair<long long, long long>> test_data(NUM_PAIRS);
    for (int i = 0; i < NUM_PAIRS; ++i) {
        test_data[i] = {dist(rng), dist(rng)};
    }

    // --- Benchmark 1: C++ Standard Library std::gcd ---
    std::cout << "Running Benchmark for std::gcd...\n";
    auto start_std = std::chrono::steady_clock::now();
    for (const auto& pair : test_data) {
        // Handle absolute value manually for std::gcd because it expects non-negative inputs
        unsigned long long a = ll_abs(pair.first);
        unsigned long long b = ll_abs(pair.second);
        unsigned long long result = std::gcd(a, b);
        do_not_optimize(result);
    }
    auto end_std = std::chrono::steady_clock::now();
    auto duration_std = std::chrono::duration_cast<std::chrono::milliseconds>(end_std - start_std).count();

    // --- Benchmark 2: Your Custom Stein GCD ---
    std::cout << "Running Benchmark for Custom Stein GCD...\n";
    auto start_custom = std::chrono::steady_clock::now();
    for (const auto& pair : test_data) {
        unsigned long long result = custom_stein_gcd(pair.first, pair.second);
        do_not_optimize(result);
    }
    auto end_custom = std::chrono::steady_clock::now();
    auto duration_custom = std::chrono::duration_cast<std::chrono::milliseconds>(end_custom - start_custom).count();

    // --- Output Results ---
    std::cout << "\n=== Results for " << NUM_PAIRS << " calculations ===\n";
    std::cout << "std::gcd time:       " << duration_std << " ms\n";
    std::cout << "Custom Stein time:   " << duration_custom << " ms\n";
    
    double difference = ((double)(duration_std - duration_custom) / duration_std) * 100.0;
    if (difference > 0) {
        std::cout << "Your custom code is " << difference << "% FASTER than std::gcd!\n";
    } else {
        std::cout << "std::gcd is " << -difference << "% FASTER than your custom code.\n";
    }

    return 0;
}
#define CATCH_CONFIG_MAIN
#include "catch/include/catch.hpp"

#include "digestible/digestible.h"

#include <random>
#include <chrono>

using namespace digestible;

TEST_CASE("insert and merge", "[t-digest]")
{
    SECTION("<double, uint64_t>") {
        tdigest<double, uint64_t> digest(10);

        for (size_t i = 0; i < 100; i++) {
            digest.insert(i);
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        REQUIRE(digest.size() == 100);

        // Make sure the total weight matches the sum of centroid weights.
        auto centroids = digest.get();

        auto summed_weight = std::accumulate(
          centroids.begin(), centroids.end(), 0,
          [](long sum, const std::pair<double, uint64_t> &val) { return (sum + val.second); });

        REQUIRE(summed_weight == 100);
    }
}

TEST_CASE("insert and query cdf", "[t-digest]")
{
    SECTION("simulated frame lengths using <uint16, uint64>")
    {
        static constexpr unsigned min_val = 64;
        static constexpr unsigned max_val = 16384;
        static constexpr size_t val_count = 1000;

        //25 is arbitrary here.
        tdigest<uint16_t, uint64_t> digest(25);

        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uint16_t> distribution(min_val, max_val);

        for (size_t i = 0; i < val_count; i++) {
            digest.insert(distribution(generator));
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        static constexpr double epsilon = 0.06; // +/- 0.06 (aka 6%)
        for (double test_point = 1; test_point <= 10; test_point++) {
            auto target = test_point / 10.0;
            auto x = target * (max_val - min_val) + min_val;
            REQUIRE(fabs(target - digest.cumulative_distribution(x)) < epsilon);
        }
    }

    SECTION("simulated latency using <double, uint64>")
    {
        static constexpr double min_val = 0.001;
        static constexpr double max_val = 2.0;
        static constexpr size_t val_count = 1000;

        tdigest<double, uint64_t> digest(25);

        std::default_random_engine generator(
          std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<float> distribution(min_val, max_val);

        for (size_t i = 0; i < val_count; i++) {
            digest.insert(distribution(generator));
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        static constexpr double epsilon = 0.06;  // +/- 0.06 (aka 6%)
        for (double test_point = 1; test_point <= 10; test_point++) {
            auto target = test_point / 10.0;
            auto x = target * (max_val - min_val) + min_val;
            REQUIRE(fabs(target - digest.cumulative_distribution(x)) < epsilon);
        }
    }

    SECTION("minimum value is much greater than zero")
    {
        static constexpr unsigned min_val = 10000;
        static constexpr unsigned max_val = 50000;
        static constexpr size_t val_count = 128;

        //25 is arbitrary here.
        tdigest<uint16_t, uint64_t> digest(25);

        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uint16_t> distribution(min_val, max_val);

        for (size_t i = 0; i < val_count; i++) {
            digest.insert(distribution(generator));
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        // This test generates fewer values over a much larger range
        // so relax the check here slightly.
        static constexpr double epsilon = 0.10; // +/- 0.10 (aka 10%)
        for (double test_point = 1; test_point <= 10; test_point++) {
            auto target = test_point / 10.0;
            auto x = target * (max_val - min_val) + min_val;
            REQUIRE(fabs(target - digest.cumulative_distribution(x)) < epsilon);
        }
    }
}

TEST_CASE("insert and query quantile", "[t-digest]")
{
    SECTION("simulated frame lengths using <uint16, uint64>")
    {
        static constexpr unsigned min_val = 64;
        static constexpr unsigned max_val = 16384;
        static constexpr size_t val_count = 1000;

        //25 is arbitrary here.
        tdigest<uint16_t, uint64_t> digest(25);

        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uint16_t> distribution(min_val, max_val);

        for (size_t i = 0; i < val_count; i++) {
            digest.insert(distribution(generator));
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        static constexpr double epsilon = (max_val - min_val) * 0.06; // +/- 0.06 (aka 6%)
        for (size_t x = 0; x <= 100; x += 10) {
            double target = min_val + (x * max_val / 100.0);
            REQUIRE(fabs(target - digest.quantile(x)) < epsilon);
        }
    }

    SECTION("simulated latency using <double, uint64>")
    {
        static constexpr double min_val = 0.001;
        static constexpr double max_val = 2.0;
        static constexpr size_t val_count = 1000;

        tdigest<double, uint64_t> digest(25);

        std::default_random_engine generator(
          std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<float> distribution(min_val, max_val);

        for (size_t i = 0; i < val_count; i++) {
            digest.insert(distribution(generator));
        }
        // Depending on how the data gets merged, there
        // could be some left in the input buffer and that's fine.
        digest.merge();

        static constexpr double epsilon = (max_val - min_val) * 0.06;  // +/- 0.06 (aka 6%)
        for (size_t x = 0; x <= 100; x += 10) {
            double target = min_val + (x * max_val / 100.0);
            REQUIRE(fabs(target - digest.quantile(x)) < epsilon);
        }
    }

    SECTION("pair of values")
    {
        const uint64_t first_val = 1.0;
        const uint64_t last_val = 2.0;

        tdigest<uint64_t, unsigned> digest(25);

        digest.insert(first_val);
        digest.insert(last_val);
        digest.merge();

        for (double q = 0; q <= 100.0; q += 0.1) {
            if (q < 50.0) {
                REQUIRE(digest.quantile(q) == first_val);
            } else {
                REQUIRE(digest.quantile(q) == last_val);
            }
        }
    }
}

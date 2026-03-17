#ifndef typedefs_h
#define typedefs_h

struct BmeValues {
    float temperature;
    float humidity;
    float pressure;

    bool operator==(const BmeValues &other) const {
        const float eps = 0.01; // tolérance pour float
        return fabs(temperature - other.temperature) < eps &&
               fabs(humidity - other.humidity) < eps &&
               fabs(pressure - other.pressure) < eps;
    }
};

#endif // typedefs_h
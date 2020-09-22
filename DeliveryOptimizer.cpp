/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: DeliveryOptimizer.cpp
*/

#include "provided.h"
#include <cmath>
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* m_strMap;
    void generateNew(vector<DeliveryRequest>& deliveries) const;
    double calTotalDis(vector<DeliveryRequest>& deliveries, const GeoCoord& depot) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
    :m_strMap(sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
    //delete m_strMap;
}

void DeliveryOptimizerImpl::generateNew(vector<DeliveryRequest>& deliveries) const {
    int index1 = (rand() % (deliveries.size() - 0)) + 0;
    int index2 = (rand() % (deliveries.size() - 0)) + 0;
    DeliveryRequest temp = deliveries[index1];
    deliveries[index1] = deliveries[index2];
    deliveries[index2] = temp;
}

double DeliveryOptimizerImpl::calTotalDis(vector<DeliveryRequest>& deliveries, const GeoCoord& depot) const {
    double distance = 0;
    distance += distanceEarthMiles(depot, deliveries[0].location);
    for (int i = 0;i < deliveries.size() - 1;i++) {
        distance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
    }
    distance += distanceEarthMiles(deliveries[deliveries.size() - 1].location, depot);
    return distance;
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;
    newCrowDistance = 0;
    oldCrowDistance = calTotalDis(deliveries, depot);   //calculate the original distance
    //Simulated Annealing
    double T = 5000;
    double q = 0.98;
    double T_min = (1e-8);
    while (T > T_min) {
        for (int i = 0;i < deliveries.size() * deliveries.size();i++) {
            vector<DeliveryRequest> temp = deliveries;
            generateNew(deliveries);
            double dis1 = calTotalDis(deliveries,depot); //get the total distance travelled if we accept the new route
            double dis2 = calTotalDis(temp, depot);     //get the total distance travelled if we take the original route
            if (dis1 > dis2) {  //if the new route is shorter, we accept it
                double r = ((double)rand()) / (RAND_MAX);   //else there is certain chance that we will accept this worse solution
                if (exp(-(dis1 - dis2) / T) <= r) {
                    deliveries = temp;  //keep the original solution
                }
            }
        }
        T *= q;
    }
    newCrowDistance = calTotalDis(deliveries,depot);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}

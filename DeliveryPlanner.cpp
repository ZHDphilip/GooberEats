/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: DeliveryPlanner.cpp
*/

#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_strMap;
    //PointToPointRouter* m_router;
    //DeliveryOptimizer* m_optimizer;
    DeliveryResult generate(GeoCoord start, GeoCoord end, vector<DeliveryCommand>& commands, double& totaldist) const;
    string getDir(double direction) const {     //generate the direction we should move in (along streetSegment(s))
        if (direction >= 0 && direction < 22.5)
            return "east";
        if (direction >= 22.5 && direction < 67.5)
            return "northeast";
        if (direction >= 67.5 && direction < 112.5)
            return "north";
        if (direction >= 112.5 && direction < 157.5)
            return "northwest";
        if (direction >= 157.5 && direction < 202.5)
            return "west";
        if (direction >= 202.5 && direction < 247.5)
            return "southwest";
        if (direction >= 247.5 && direction < 292.5)
            return "south";
        if (direction >= 292.5 && direction < 337.5)
            return "southeast";
        return "east";
    }
    string leftRight(double direction) const {  //check wheter turn left or turn right
        if (direction >= 1 && direction < 180)
            return "left";
        return "right";
    }
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
    :m_strMap(sm)
{
    //m_router = new PointToPointRouter(sm);
    //m_optimizer = new DeliveryOptimizer(sm);
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
    //delete m_router;
    //delete m_optimizer;
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    commands.clear();
    DeliveryOptimizer m_optimizer(m_strMap);
    totalDistanceTravelled = 0;
    double oldCrowDistance, newCrowDistance;
    vector<DeliveryRequest> New = deliveries;
    m_optimizer.optimizeDeliveryOrder(depot, New, oldCrowDistance, newCrowDistance);
    if (depot != New[0].location) {
        double disIncre = 0;
        DeliveryResult res= generate(depot, New[0].location, commands, disIncre);
        if (res == NO_ROUTE)
            return NO_ROUTE;
        else if (res == BAD_COORD)
            return BAD_COORD;
        totalDistanceTravelled += disIncre;
    }
    DeliveryCommand beginDeliver;
    beginDeliver.initAsDeliverCommand(New[0].item);
    commands.push_back(beginDeliver);
    for (int i = 0;i < New.size()-1;i++) {
        double disIncre = 0;
        DeliveryResult res2 = generate(New[i].location, New[i + 1].location, commands, disIncre);
        if (res2 == NO_ROUTE)
            return NO_ROUTE;
        else if (res2 == BAD_COORD)
            return BAD_COORD;
        totalDistanceTravelled += disIncre;
        DeliveryCommand Deliver;
        Deliver.initAsDeliverCommand(New[i+1].item);
        commands.push_back(Deliver);
    }
    double disIncre;
    DeliveryResult res3 = generate(New[New.size() - 1].location, depot, commands, disIncre);
    if (res3 == NO_ROUTE)
        return NO_ROUTE;
    else if (res3 == BAD_COORD)
        return BAD_COORD;
    totalDistanceTravelled += disIncre;
    cerr << endl << totalDistanceTravelled << endl;
    return DELIVERY_SUCCESS;
}

DeliveryResult DeliveryPlannerImpl::generate(GeoCoord start, GeoCoord end, vector<DeliveryCommand>& commands, double& totaldist) const{
    list<StreetSegment> route;
    PointToPointRouter m_router(m_strMap);
    DeliveryResult res = m_router.generatePointToPointRoute(start, end, route, totaldist);
    if (res == NO_ROUTE)
        return NO_ROUTE;
    else if (res == BAD_COORD)
        return BAD_COORD;
    typename list<StreetSegment>::iterator p = route.begin();
    StreetSegment before;
    while (p!=route.end()) {
        if (p != route.begin()) {
            double turnDir = angleBetween2Lines(before,*p); //not sure
            string turnDirection = leftRight(turnDir);
            DeliveryCommand turn;
            turn.initAsTurnCommand(turnDirection, (*p).name);
            commands.push_back(turn);
        }
        string streetName = (*p).name;
        double dir = angleOfLine((*p));
        string direction = getDir(dir);
        double distCount = 0;
        while (p != route.end()) {
            if ((*p).name == streetName) {
                distCount = distCount + distanceEarthMiles((*p).start, (*p).end);
                before = *p;
                p++;
            }
            else
                break;
        }
        DeliveryCommand proceed;
        proceed.initAsProceedCommand(direction, streetName, distCount);
        commands.push_back(proceed);
        //typename list<StreetSegment>::iterator temp = --p;
    }
    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}

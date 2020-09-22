/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: Main.cpp
*/

#include <iostream>
#include "ExpandableHashMap.h"
#include "provided.h"
#include <functional>
#include <fstream>
#include <ctime>


//#include <string>
using namespace std;


int main(int argc, const char* argv[]) {
	// Define a hashmap that maps strings to doubles and has a maximum
	// load factor of 0.2. It will initially have 8 buckets when empty.
	/*ExpandableHashMap<string, double> nameToGPA(0.2);
	// Add new items to the hashmap. Inserting the third item will cause
	// the hashmap to increase the number of buckets (since the maximum
	// load factor is 0.2), forcing a rehash of all items.
	nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
	*davidsGPA = 1.5; // after a re-grade of David’s exam
	nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
	// replaces old 3.5 GPA
	double* lindasGPA = nameToGPA.find("Abe");
	if (lindasGPA == nullptr)
	cout << "Linda is not in the roster!" << endl;
	else
	cout << "Linda’s GPA is: " << *lindasGPA << endl;*/
	//clock_t start, end;
	//start = clock();
	//StreetMap m;
	//m.load("C:/Users/dongz/Desktop/Zihao DONG/UCLA/cs 32/Project4/skeleton/skeleton/mapdata.txt");
	//vector<StreetSegment> TESTSEG;

	//clock_t begin2 = clock();
	//m.getSegmentsThatStartWith(GeoCoord("34.0734335", "-118.4449143"), TESTSEG);
	//clock_t end2 = clock();

	// Calculating total time taken by the program. 
	//double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;
	//cerr << end << elapsed_secs2 << "SECS" << endl;

	// Define a hashmap that maps strings to doubles and has a maximum
// load factor of 0.2. It will initially have 8 buckets when empty.
	/*ExpandableHashMap<string, double> nameToGPA(0.2);
	// Add new items to the hashmap. Inserting the third item will cause
	// the hashmap to increase the number of buckets (since the maximum
	// load factor is 0.2), forcing a rehash of all items.
	nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
		*davidsGPA = 1.5; // after a re-grade of David’s exam
	nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
	// replaces old 3.5 GPA
	double* lindasGPA = nameToGPA.find("Abe");
	if (lindasGPA == nullptr)
		cout << "Linda is not in the roster!" << endl;
	else
		cout << "Linda’s GPA is: " << *lindasGPA << endl;*/
	StreetMap m;
	m.load("C:/Users/dongz/Desktop/Zihao DONG/UCLA/cs 32/Project4/skeleton/skeleton/mapdata.txt");
	vector<StreetSegment> res;
	m.getSegmentsThatStartWith(GeoCoord("34.0718238", "-118.4525699"), res);
	typename std::vector<StreetSegment>::iterator p = res.begin();
	while (p != res.end()) {
		cerr << '(' << (*p).start.latitudeText << "," << (*p).start.longitudeText << ") (" << (*p).end.latitudeText << "," << (*p).end.longitudeText << ") " << (*p).name << endl;
		p++;
	}
	cerr << "Here Comes Other Tests..."<< endl;
	PointToPointRouter test(&m);
	list<StreetSegment> result;
	double TotalDist = 0;
	test.generatePointToPointRoute(GeoCoord("34.0732012", "-118.4491058"), GeoCoord("34.0886026", "-118.4758705"), result, TotalDist);
	typename std::list<StreetSegment> ::iterator it = result.begin();
	while (it != result.end()) {
		cerr << '(' << (*it).start.latitudeText << "," << (*it).start.longitudeText << ") (" << (*it).end.latitudeText << "," << (*it).end.longitudeText << ") " << (*it).name << endl;
		it++;
	}
	cerr << TotalDist << endl;
	DeliveryPlanner testing(&m);
	GeoCoord depot("34.0625329", "-118.4470263");
	DeliveryRequest one("Apple", GeoCoord("34.0625329", "-118.4470263"));
	DeliveryRequest two("Banana", GeoCoord("34.0625329", "-118.4470263"));
	DeliveryRequest three("Pear", GeoCoord("34.0625329", "-118.4470263"));
	DeliveryRequest four("Peach", GeoCoord("34.0625329", "-118.4470263"));
	DeliveryRequest five("5", GeoCoord("34.0732012", "-118.4491058"));
	DeliveryRequest six("6", GeoCoord("34.0886026", "-118.4758705"));

	GeoCoord test1("34.0420561", "-118.5011699");
	GeoCoord test2("34.0793418", "-118.3813583");

	GeoCoord test3("34.0419265", "-118.5010322");
	GeoCoord test4("34.0304673", "-118.4876393");
	GeoCoord test5("0", "0");

	GeoCoord Philip1("34.0718238", "-118.4525699");
	GeoCoord Philip2("34.0547000", "-118.4794734");


	GeoCoord Weyburn("34.0625329", "-118.4470263");
	GeoCoord Strathmore("34.0656797", "-118.4505131");
	GeoCoord Frat("34.0685657", "-118.4489289");
	GeoCoord Manning("34.0660665", "-118.4385079");

	DeliveryRequest a1("a", Weyburn);
	DeliveryRequest b1("b", Strathmore);
	DeliveryRequest c1("c", Frat);
	DeliveryRequest d1("d", Manning);
	DeliveryRequest e1("e", test1);
	DeliveryRequest f1("f", test2);
	DeliveryRequest g1("g", Philip1);
	DeliveryRequest h1("h", Philip2);


	vector<DeliveryRequest> request;
	vector<DeliveryCommand> commands;
	double distanceTravelled;
	//request.push_back(one);
	//request.push_back(two);
	//request.push_back(three);
	request.push_back(four);
	//request.push_back(five);
	request.push_back(five);
	request.push_back(a1);
	request.push_back(b1);
	request.push_back(c1);
	request.push_back(d1);
	request.push_back(e1);
	request.push_back(f1);
	request.push_back(g1);
	testing.generateDeliveryPlan(four.location, request, commands, distanceTravelled);
	typename vector<DeliveryCommand> ::iterator itr = commands.begin();
	while (itr != commands.end()) {
		cerr << (*itr).description() << endl;
		itr++;
	}
	DeliveryOptimizer optimizer(&m);
	double old_crow;
	double new_crow;
	optimizer.optimizeDeliveryOrder(d1.location, request, old_crow, new_crow);
	cerr << endl << "NEW CROW: " << new_crow / 1 << "  OLD CROW: " << old_crow / 1 << endl;
	cerr << distanceTravelled;

	cerr << endl << "BROXTON TEST" << endl;

	GeoCoord DEPOT_TEST("34.0625329", "-118.4470263");
	GeoCoord BOULEVARD("34.0570663", "-118.4428298");
	DeliveryRequest SARDINE("Sars", GeoCoord("34.0636671", "-118.4461842"));
	DeliveryRequest PLAZA("Salmon", GeoCoord("34.0707966", "-118.4447645"));
	vector<DeliveryRequest> TESTVEC;
	TESTVEC.push_back(SARDINE);
	TESTVEC.push_back(PLAZA);

	optimizer.optimizeDeliveryOrder(DEPOT_TEST, TESTVEC, old_crow, new_crow);
	testing.generateDeliveryPlan(BOULEVARD, TESTVEC, commands, distanceTravelled);

	itr = commands.begin();
	while (itr != commands.end()) {
		cerr << (*itr).description() << endl;
		itr++;
	}
	cerr << endl << "NEW CROW: " << new_crow / 1 << "  OLD CROW: " << old_crow / 1 << endl;
	cerr << distanceTravelled;
}
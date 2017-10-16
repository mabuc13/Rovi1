#include <iostream>
#include <rw/rw.hpp>
#include <rwlibs/pathplanners/rrt/RRTPlanner.hpp>
#include <rwlibs/pathplanners/rrt/RRTQToQPlanner.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
using namespace rw::common;
using namespace rw::math;
using namespace rw::kinematics;
using namespace rw::loaders;
using namespace rw::models;
using namespace rw::pathplanning;
using namespace rw::proximity;
using namespace rw::trajectory;
using namespace rwlibs::pathplanners;
using namespace rwlibs::proximitystrategies;

#define MAXTIME 10.

bool checkCollisions(Device::Ptr device, const State &state, const CollisionDetector &detector, const Q &q) {
	State testState;
	CollisionDetector::QueryResult data;
	bool colFrom;

	testState = state;
	device->setQ(q,testState);
	colFrom = detector.inCollision(testState,&data);
	if (colFrom) {
		cerr << "Configuration in collision: " << q << endl;
		cerr << "Colliding frames: " << endl;
		FramePairSet fps = data.collidingFrames;
		for (FramePairSet::iterator it = fps.begin(); it != fps.end(); it++) {
			cerr << (*it).first->getName() << " " << (*it).second->getName() << endl;
		}
		return false;
	}
	return true;
}

//Functions to handle the creation of the lua script
void initLuaStream(string Filename, ofstream &myOfStream)
{
    myOfStream.open(Filename);
    //We opened a file stream, first things is to make sure that the lua script is initialized as it should be
    myOfStream << "wc = rws.getRobWorkStudio():getWorkCell()" << endl;
    myOfStream << "state = wc:getDefaultState()" << endl;
    myOfStream << "device = wc:findDevice(\"KukaKr16\")"<< endl;
    myOfStream << "gripper = wc:findFrame(\"Tool\");" << endl;
    myOfStream << "bottle = wc:findFrame(\"Bottle\");" << endl;
    myOfStream << "table = wc:findFrame(\"Table\");" << endl << endl;

    myOfStream << "function setQ(q)" << endl;
    myOfStream << "qq = rw.Q(#q,q[1],q[2],q[3],q[4],q[5],q[6])" << endl;
    myOfStream << "device:setQ(qq,state)" << endl;
    myOfStream << "rws.getRobWorkStudio():setState(state)" << endl;
    myOfStream << "rw.sleep(0.1)" << endl;
    myOfStream << "end" << endl << endl;

    myOfStream << "function attach(obj, tool)" << endl;
    myOfStream << "rw.gripFrame(obj, tool, state)" << endl;
    myOfStream << "rws.getRobWorkStudio():setState(state)" << endl;
    myOfStream << "rw.sleep(0.1)" << endl;
    myOfStream << "end" << endl << endl;


    //myOfStream << "setQ({-3.142, -0.827, -3.002, -3.143, 0.099, -1.573})" << endl;

    //myOfStream << "attach(bottle,gripper)" << endl;

    //myOfStream << "attach(bottle,table)" << endl;
    //myOfStream << "setQ({1.571, 0.006, 0.03, 0.153, 0.762, 4.49})" << endl;

}

void closeOfstream(ofstream &myOfStream)
{
    myOfStream.close();
}


int main(int argc, char** argv) {
        rw::math::Math::seed();
        const string wcFile = "/home/student/workspace/robotics_assignment2/workcell/Kr16WallWorkCell/Scene.wc.xml"; //Edited this to my filepath
	const string deviceName = "KukaKr16";
	cout << "Trying to use workcell " << wcFile << " and device " << deviceName << endl;

	WorkCell::Ptr wc = WorkCellLoader::Factory::load(wcFile);
	Device::Ptr device = wc->findDevice(deviceName);
	if (device == NULL) {
		cerr << "Device: " << deviceName << " not found!" << endl;
		return 0;
	}
	const State state = wc->getDefaultState();

	CollisionDetector detector(wc, ProximityStrategyFactory::makeDefaultCollisionStrategy());
	PlannerConstraint constraint = PlannerConstraint::make(&detector,device,state);

	/** Most easy way: uses default parameters based on given device
		sampler: QSampler::makeUniform(device)
		metric: PlannerUtil::normalizingInfinityMetric(device->getBounds())
		extend: 0.05 */
	//QToQPlanner::Ptr planner = RRTPlanner::makeQToQPlanner(constraint, device, RRTPlanner::RRTConnect);

	/** More complex way: allows more detailed definition of parameters and methods */
	QSampler::Ptr sampler = QSampler::makeConstrained(QSampler::makeUniform(device),constraint.getQConstraintPtr());
	QMetric::Ptr metric = MetricFactory::makeEuclidean<Q>();
	double extend = 0.1;
	QToQPlanner::Ptr planner = RRTPlanner::makeQToQPlanner(constraint, sampler, metric, extend, RRTPlanner::RRTConnect);

        //Q from(6,-0.2,-0.6,1.5,0.0,0.6,1.2); Original line from pathplanner.cpp!!! I think there may be a collision in this frame tho
        Q from(6,0, 0, 0, 0, 0, 0); // Starting position!
                                                                /*
                                                                 * q_pick = (-3.142,-0.827,-3.002,-3.143,0.099,-1.573) [rad]
•
                                                                  q_place = (1.571,0.006,0.030,0.153,0.762,4.490) [rad]
                                                                 *  */
	//Q to(6,1.7,0.6,-0.8,0.3,0.7,-0.5); // Very difficult for planner


        ofstream myOfStream;
        initLuaStream("test.txt", myOfStream);



        Q to(6,-3.142,-0.827,-3.002,-3.143,0.099,-1.5736); //Bottle position

	if (!checkCollisions(device, state, detector, from))
		return 0;
	if (!checkCollisions(device, state, detector, to))
		return 0;

	cout << "Planning from " << from << " to " << to << endl;
	QPath path;
	Timer t;
	t.resetAndResume();
	planner->query(from,to,path,MAXTIME);
	t.pause();
	cout << "Path of length " << path.size() << " found in " << t.getTime() << " seconds." << endl;
	if (t.getTime() >= MAXTIME) {
		cout << "Notice: max time of " << MAXTIME << " seconds reached." << endl;
	}

        string tmpString;
        stringstream oneState;
	for (QPath::iterator it = path.begin(); it < path.end(); it++) {
           //tmpString = (*it);
            //tmpString.erase(tmpString.begin(),tmpString.begin()+4);
            //cout << tmpString << endl;
            stringstream oneState;
           oneState << *it;
           tmpString = oneState.str();

           tmpString.erase(tmpString.begin(), tmpString.begin()+4);
           myOfStream << "setQ(" << tmpString << ")" << endl;
	}
        closeOfstream(myOfStream);
	cout << "Program done." << endl;
	return 0;
}



#include <iostream>
#include <rw/rw.hpp>
#include <rwlibs/pathplanners/rrt/RRTPlanner.hpp>
#include <rwlibs/pathplanners/rrt/RRTQToQPlanner.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>

//#include <rw/models/WorkCell.hpp>
//#include <rw/models/Device.hpp>
//#include <rw/kinematics/Frame.hpp>
//#include <boost/foreach.hpp>
#include <rw/kinematics/MovableFrame.hpp>
#include <rw/kinematics/FKRange.hpp>
#include <rw/math/Transform3D.hpp>

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

//using namespace rw::math::Transform3D;
//using namespace rw::models::WorkCell;
using rw::math::Transform3D;

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

void gripMovableFrame(
    MovableFrame& item, Frame& gripper, State& state)
{
    FKRange fk(&gripper, &item, state);
    const Transform3D<> transform = fk.get(state);
    item.setTransform(transform, state);
    item.attachTo(&gripper, state);
}
/*
void printKinematicTree(
    Frame& frame,
    const State& state,
    const Transform3D<>& parentTransform,
    int level)
{
    const Transform3D<> transform = parentTransform * frame.getTransform(state);
    std::cout
        << std::string(level, ' ')
        << frame.getName()
        << " at "
        << transform.P()
        << "\n";
    BOOST_FOREACH(Frame& child, frame.getChildren(state)) {
        printKinematicTree(child, state, transform, level + 1);
    }
}


void printDefaultWorkCellStructure(const WorkCell& workcell)
{
    std::cout << workcell << "\n";
    printKinematicTree(
        *workcell.getWorldFrame(),
        workcell.getDefaultState(),
        Transform3D<>::identity(),
        0);
}
*/

int main(int argc, char** argv) {
        rw::math::Math::seed(); //Marks edit
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
        double extend = 0.5;
	QToQPlanner::Ptr planner = RRTPlanner::makeQToQPlanner(constraint, sampler, metric, extend, RRTPlanner::RRTConnect);
//First we move the robot arm from the starting position to right above the bottle positon
        Q from(6,0, 0, 0, 0, 0, 0); // Starting position!

        ofstream myOfStream;
        initLuaStream("take1.txt", myOfStream);

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
/*************************************************************************************************************
                                                        PATH 2
*************************************************************************************************************/
//Next we attach the bottle to the gripper, before moving it to the bottles end position
//Lets see if we cant print the frame state of the workcell

        myOfStream << "attach(bottle,gripper)" << endl;



//Next we move the bottle to the desired end position
        Q from2(6, -3.142,-0.827,-3.002,-3.143,0.099,-1.574); //Rounded up here
        Q to2(6, 1.571,0.006,0.030,0.153,0.762,4.490);

        State state2 = wc->getDefaultState();
        device->setQ(from2, state2);

        MovableFrame *bottle =dynamic_cast<MovableFrame*>(wc->findFrame("Bottle"));
        Frame *tool = wc->findFrame("Tool");
       rw::kinematics::Kinematics::gripFrame(bottle, tool, state2);

       CollisionDetector detector2(wc, ProximityStrategyFactory::makeDefaultCollisionStrategy());
       PlannerConstraint constraint2 = PlannerConstraint::make(&detector2,device,state2);
       QToQPlanner::Ptr planner2 = RRTPlanner::makeQToQPlanner(constraint2, sampler, metric, extend, RRTPlanner::RRTConnect);

        if (!checkCollisions(device, state2, detector2, from2))
                return 0;
        if (!checkCollisions(device, state2, detector2, to2))
                return 0;

        cout << "Planning from " << from2 << " to " << to2 << endl;
        QPath path2;
        Timer t2;
        t2.resetAndResume();
        planner2->query(from2,to2,path2,MAXTIME);
        t2.pause();
        cout << "Path of length " << path2.size() << " found in " << t2.getTime() << " seconds." << endl;
        if (t2.getTime() >= MAXTIME) {
                cout << "Notice: max time of " << MAXTIME << " seconds reached." << endl;
        }

        string tmpString2;

        for (QPath::iterator it2 = path2.begin(); it2 < path2.end(); it2++) {
           //tmpString = (*it);
            //tmpString.erase(tmpString.begin(),tmpString.begin()+4);
            //cout << tmpString << endl;
            stringstream oneState2;
           oneState2 << *it2;
           tmpString2 = oneState2.str();

           tmpString2.erase(tmpString2.begin(), tmpString2.begin()+4);
           myOfStream << "setQ(" << tmpString2 << ")" << endl;
        }
//Finally we attach the bottle to the table
        myOfStream << "attach(bottle,table)" << endl;
        closeOfstream(myOfStream);
	cout << "Program done." << endl;
	return 0;
}



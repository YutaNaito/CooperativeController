// -*- C++ -*-
/*!
 * @file  CooperativeController.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#define _USE_MATH_DEFINES
#include "CooperativeController.h"
#include <math.h>

//*** prototype declare ***//
void Updatevalue(double _newValue, int _length, double _Value[]); // update input value
double Average(int _length, double _Value[]);                     // calcurate of moving average(element number 5)
double Slope_LeastSquare(int N, double x[], double y[]);          // calcurate slope with least squares method
double Sigmoid(double _Slope);                                    // calcurate sigmoid


// Module specification
// <rtc-template block="module_spec">
static const char* cooperativecontroller_spec[] =
  {
    "implementation_id", "CooperativeController",
    "type_name",         "CooperativeController",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "YutaNaito",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
	"conf.default.inputDOF", "3",
	"conf.default.armDOF", "5",
	"conf.default.offset", "0.03",
    // Widget
	"conf.__widget__.inputDOF", "text",
	"conf.__widget__.armDOF", "text",
	"conf.__widget__.offset", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
CooperativeController::CooperativeController(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_RobotArmIn("RobotArm", m_RobotArm),
    m_MobileRobotIn("MobileRobot", m_MobileRobot),
    m_LRFIn("LRF", m_LRF),
	m_ControllerValueIn("ControllerValue" , m_ControllerValue),
    m_CooperateArmOut("CooperateArm", m_CooperateArm),
    m_CooperateMobileOut("CooperateMobile", m_CooperateMobile)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CooperativeController::~CooperativeController()
{
}



RTC::ReturnCode_t CooperativeController::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("RobotArm", m_RobotArmIn);
  addInPort("MobileRobot", m_MobileRobotIn);
  addInPort("LRF", m_LRFIn);
  addInPort("ControllerValue", m_ControllerValueIn);
  // Set OutPort buffer
  addOutPort("CooperateArm", m_CooperateArmOut);
  addOutPort("CooperateMobile", m_CooperateMobileOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("offset", m_offset, "0.03");
  bindParameter("inputDOF", m_inputDOF, "3");
  bindParameter("armDOF", m_armDOF, "5");
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CooperativeController::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CooperativeController::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CooperativeController::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CooperativeController::onActivated(RTC::UniqueId ec_id)
{

	//* setup value
	for (int i = 0; i < 5; i++){
		InputValueTime[i] = 0.0;
		InputValue[i] = 0.0;
		MovingAverageTime[i] = 0.0;
		MovingAverage[i] = 0.0;
		if (i > 1){
			PreInputValue[i - 2] = 0.0;
		}
	}
	m_ControllerValue.data.length(m_inputDOF);
	m_RobotArm.data.length(m_armDOF);
	m_CooperateArm.data.length(m_armDOF);

	//* setup input
	m_MobileRobot.data.vx = 0.0;
	m_MobileRobot.data.vy = 0.0;
	m_MobileRobot.data.va = 0.0;
	for (int i = 0; i < m_armDOF; i++){
		m_RobotArm.data[i] = 0.0;
	}
	m_ControllerValue.tm.nsec = 0.0;
	for (int i = 0; i < m_inputDOF; i++){
		m_ControllerValue.data[i] = 0.0;
	}

	//* setup output
	m_CooperateMobile.data.vx = 0.0;
	m_CooperateMobile.data.vy = 0.0;
	m_CooperateMobile.data.va = 0.0;
	m_CooperateArm.tm.nsec = 0.0;
	for (int i = 0; i < m_armDOF; i++){
		m_CooperateArm.data[i] = 0.0;
	}

	while (m_armDOF > 7){
		std::cout << "Please change configulation DOF " << std::endl;
		return RTC::RTC_ERROR;
	}
	//* set first value
	/*int counter = 0;
	while (counter < 5){
		if (m_ControllerAccelIn.isNew()){
			m_ControllerAccelIn.read();
			InputValue[counter] = sqrt(pow(m_ControllerAccel.data[0], 2) + pow(m_ControllerAccel.data[1], 20) + pow(m_ControllerAccel.data[2], 2.0));
			counter++;
		}
	}*/

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CooperativeController::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout << "Deactivated" << std::endl;
	for (int i = 0; i < 5; i++){
		InputValueTime[i] = 0.0;
		InputValue[i] = 0.0;
		MovingAverageTime[i] = 0.0;
		MovingAverage[i] = 0.0;
		if (i > 1){
			PreInputValue[i - 2] = 0.0;
		}
	}

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CooperativeController::onExecute(RTC::UniqueId ec_id)
{
	system("cls");

	//* check the input value
	if (m_ControllerValueIn.isNew()){

		//* read input value
		m_ControllerValueIn.read();

		//* update time and value
		double NewInputTime = (double)m_ControllerValue.tm.nsec / (pow(10, 9));//[sec]
		Updatevalue(NewInputTime, 5, InputValueTime);
		std::cout << " InputTime[0] : " << InputValueTime[0] << std::endl;
		std::cout << " InputTime[1] : " << InputValueTime[1] << std::endl;
		std::cout << " InputTime[2] : " << InputValueTime[2] << std::endl;
		std::cout << " InputTime[3] : " << InputValueTime[3] << std::endl;
		std::cout << " InputTime[4] : " << InputValueTime[4] << std::endl;
		double powSum = 0.0;
		for (int i = 0; i < m_inputDOF; i++){
			powSum += pow((m_ControllerValue.data[i] - PreInputValue[i]), 2);
		}
		double NewInputValue = sqrt(powSum);
		Updatevalue(NewInputValue, 5, InputValue);
		std::cout << " InputValue[0] : " << InputValue[0] << std::endl;
		std::cout << " InputValue[1] : " << InputValue[1] << std::endl;
		std::cout << " InputValue[2] : " << InputValue[2] << std::endl;
		std::cout << " InputValue[3] : " << InputValue[3] << std::endl;
		std::cout << " InputValue[4] : " << InputValue[4] << std::endl;

		//* calculate moving average
		double NewMovingAverageTime = Average(5, InputValueTime);
		Updatevalue(NewInputTime, 5, MovingAverageTime);
		double NewMovingAverage = Average(5, InputValue);
		Updatevalue(NewMovingAverage, 5, MovingAverage);

		//* calulate slope velocity and sigmoid 
		double a = Slope_LeastSquare(5, MovingAverageTime, MovingAverage);
		double sigmoid = Sigmoid(abs(a));
		std::cout << "sigmoid is " << sigmoid << std::endl;
		
		//* decide which robot is controlled
		if (ModeChange){
			if (sigmoid > ChangeThreshold){
				MobileRobot = true;
			}
			else if(sigmoid == 0.5){
			}
			else{
				MikataArm = true;
			}
			ModeChange = false;
		}
		std::cout << "MobileRobot : " << MobileRobot << "  MikataArm : " << MikataArm << std::endl;

		//* feedback from LRF
		int NearObstacle = 0;
		int VeryNearObstacle = 0;
		if (m_LRFIn.isNew()){
			m_LRFIn.read();
			for (int i = 0; i < m_LRF.ranges.length(); i++){
				double LaserR = (double)m_LRF.ranges[i];;
				if (LaserR > LRFCutOffset && LaserR <= WarningDistance){
					NearObstacle++;
				}
				if (LaserR > LRFCutOffset && LaserR <= EmergencyDistance)
				{
					VeryNearObstacle++;
				}
			}
		}

		//* write each value
		if (m_RobotArmIn.isNew() && m_MobileRobotIn.isNew()){
			m_RobotArmIn.read();
			m_MobileRobotIn.read();
		}
		if (MikataArm){
			m_CooperateArm.data = m_RobotArm.data;
			std::cout << "Robot mode : RobotArm" << std::endl;
		}
		else if (MobileRobot){
			double ErrorRate = 0.05 * m_LRF.ranges.length();
			if (NearObstacle > ErrorRate && VeryNearObstacle < ErrorRate){
				m_CooperateArm.data = m_RobotArm.data;
				m_CooperateMobile.data = m_MobileRobot.data;
				m_CooperateArm.data[1] = 0.0;
				m_CooperateMobile.data.vx = 0.0;
				std::cout << "Robot mode : Arm & Mobile" << std::endl;
			}else if (NearObstacle > ErrorRate && VeryNearObstacle > ErrorRate){
				m_CooperateArm.data = m_RobotArm.data;
				std::cout << "Robot mode : RobotArm" << std::endl;
			}
			else{
				m_CooperateMobile.data = m_MobileRobot.data;
				std::cout << "Robot mode : MobileRobot" << std::endl;
			}
		}
		m_CooperateArmOut.write();
		m_CooperateMobileOut.write();

		//* check the switch
		if ((abs(m_ControllerValue.data[0]) <= m_offset)  && (abs(m_ControllerValue.data[1]) <= m_offset) && (abs(m_ControllerValue.data[2]) <= m_offset)){
			ModeChange = true;
			MobileRobot = false;
			MikataArm = false;
		}
		std::cout << "ModeChange : " << ModeChange << std::endl;

		//* record old value
		for (int i = 0; i < m_inputDOF; i++){
			PreInputValue[i] = m_ControllerValue.data[i];
		}

		//* reset outputvalue
		for (int i = 0; i < m_armDOF; i++){
			m_CooperateArm.data[i] = 0.0;
		}
		m_CooperateMobile.data.vx = 0.0;
		m_CooperateMobile.data.vy = 0.0;
		m_CooperateMobile.data.va = 0.0;
	}
	else{
		std::cout << "No control value" << std::endl;
	}

	Sleep(10);

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CooperativeController::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CooperativeController::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CooperativeController::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CooperativeController::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CooperativeController::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*RTC::ReturnCode_t CooperativeController::Updatevalue(double _newValue, int _length, double _Value[])
{
	for (int i = 0; i < _length; i++){
		_Value[i] = _Value[i + 1];
	}
	_Value[_length - 1] = _newValue;
}

RTC::ReturnCode_t CooperativeController::Average(int _length, double _Value[])
{
	double _Mean = 0.0;
	double _Total = 0.0;
	for (int i = 0; i < _length; i++)
	{
		_Total += _Value[i];
	}
	_Mean = _Total / _length;

	return _Mean;
}*/

extern "C"
{
 
  void CooperativeControllerInit(RTC::Manager* manager)
  {
    coil::Properties profile(cooperativecontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<CooperativeController>,
                             RTC::Delete<CooperativeController>);
  }
  
};


void Updatevalue(double _newValue, int _length, double _Value[])
{
	for (int i = 0; i < _length; i++){
		_Value[i] = _Value[i + 1];
	}
	_Value[_length - 1] = _newValue;
}

double Average(int _length, double _Value[])
{
	double _Mean = 0.0;
	double _Total = 0.0;
	for (int i = 0; i < _length; i++)
	{
		_Total += _Value[i];
	}
	_Mean = _Total / _length;

	return _Mean;
}

double Slope_LeastSquare(int N, double _x[], double _y[])
{
	double a = 0.0, b = 0.0, A00 = 0.0, A01 = 0.0, A02 = 0.0, A11 = 0.0, A12 = 0.0;
	for (int i = 0; i < N; i++) {
		A00 += 1.0;
		A01 += _x[i];
		A02 += _y[i];
		A11 += _x[i] * _x[i];
		A12 += _x[i] * _y[i];
	}

	b = (A02 * A11 - A01 * A12) / (A00 * A11 - A01 * A01);
	a = (A00 * A12 - A01 * A02) / (A00 * A11 - A01 * A01);

	return a;
}

double Sigmoid(double _Slope)
{
	double value = 0.0;
	value = 1 / (1 + exp(-2*_Slope));
	return value;
}
#include "WPILib.h"
#define RIGHTDRIVEPORT 4 //right drive motor is in pwm port 4
#define LEFTDRIVEPORT 8 //left drive motor is in pwm port 8
#define TOPFANPORT 1 // top fan is in pwm port 1
#define TORRENTPORT 2 // torrent rotating is in pwm port 2
#define SHOOTPORT 3 // shooter port is pwm port 3
#define BOTTOMFANPORT 5 // bottom fan is pwm port 5
#define PICKUPPORT 6 // pick up port is pwm port 6
#define MOVEUPPORT 7 //conveyorbelt systm is pwm port 7


#define RIGHTDRIVESTICKPORT 2 //right drive stick is usb port 2
#define LEFTDRIVESTICKPORT 3 // left drive stick is usb port 3
#define MANIPULATOR 1 //manipulator is in usb port 1



/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick rightdrivestick; // rightdrivestick is a joystick
	Joystick leftdrivestick; //leftdrivestick is a joystick
	Joystick manipulator; //manipulator is a joystick

	Victor topfan; //topfan is a victor
	Victor bottomfan; //bottom fan is controlled by a victor
	Victor pickup; //pickup is controlled by a victor
	Victor moveup; //move up conveyorbelt is controled by a victor
	Victor shoot; // shooter is controlled by a victor
	Victor leftdrive; // leftdrive motor is controlled by a victor
	Victor rightdrive; //right drive motor is controlled by a victor
	Victor torrent; // torrent rotating is controlled by a victor
	

	
	RobotDrive myRobot; // robot drive system; myrobot is the RobotDrive system

	

public:
	RobotDemo(void):
		
				rightdrivestick(RIGHTDRIVESTICKPORT),
				leftdrivestick(LEFTDRIVESTICKPORT),
				manipulator(MANIPULATOR),
				topfan(TOPFANPORT),
				bottomfan(BOTTOMFANPORT),
				pickup(PICKUPPORT),
				moveup(MOVEUPPORT),
				shoot(SHOOTPORT),
				leftdrive(LEFTDRIVEPORT),
				rightdrive(RIGHTDRIVEPORT),
				torrent(TORRENTPORT),
				myRobot(leftdrive, rightdrive)
				
				
		{
		myRobot.SetExpiration(0.1);
		
		}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, false); //left motor is inverted
			myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, false); //right motor is inverted
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true); //left motor is inverted
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true); //left motor is inverted

			myRobot.TankDrive(leftdrivestick, rightdrivestick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
			if(manipulator.GetRawButton(1))
			{
				float x = manipulator.GetThrottle();
					x++;
					shoot.Set(-x);
					moveup.Set(-0.5);
			}else if(manipulator.GetRawButton(3))
			{				
				float x = manipulator.GetThrottle();
				x++;
				shoot.Set(x);
			}else if(manipulator.GetRawButton(2)){
				moveup.Set(0.5);
			}else{
				shoot.Set(0);
				moveup.Set(0);
			}
			
			if(manipulator.GetRawButton(5))
				{torrent.Set(-0.3);
				
				}else if
			(manipulator.GetRawButton(4))
				{torrent.Set(0.40);
				
				}else{
					torrent.Set(0);
				}
			
			if(manipulator.GetRawButton(6))
			{
				pickup.Set(0.5);
			}else if(manipulator.GetRawButton(7)){
				pickup.Set(-0.5);
			}
			else
			{
				pickup.Set(0);
			}
			
			
			if(leftdrivestick.GetRawButton(3))
			{
				float y = leftdrivestick.GetThrottle();
					y++;
				topfan.Set(y);
				bottomfan.Set(y);
			}else if(leftdrivestick.GetRawButton(2)){
				float y = leftdrivestick.GetThrottle();
					y++;
				topfan.Set(-y);
				bottomfan.Set(-y);
			}else{
				topfan.Set(0);
				bottomfan.Set(0);
				
			}
			
			if(rightdrivestick.GetRawButton(3))
			{
				float z = rightdrivestick.GetThrottle();
					z++;
				bottomfan.Set(z);
				topfan.Set(z);
			}else if(rightdrivestick.GetRawButton(2)){
				float z = rightdrivestick.GetThrottle();
					z++;
				bottomfan.Set(-z);
				topfan.Set(-z);
			}else{
				bottomfan.Set(0);
				topfan.Set(0);
				
			}
		
	}	}
};

START_ROBOT_CLASS(RobotDemo);


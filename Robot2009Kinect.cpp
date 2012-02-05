#include "WPILib.h"

/* Port configuration for sensors and actuators. */
	#define LEFT_DRIVE_JOYSTICK_USB_PORT 3
	#define MANIPULATOR_JOYSTICK_USB_PORT 1
	#define RIGHT_DRIVE_JOYSTICK_USB_PORT 2
	
	#define DRIVE_KINECT_IDENT 1
	#define MANIPULATOR_KINECT_IDENT 2

	#define LEFT_DRIVE_MOTOR_PORT 8
	#define RIGHT_DRIVE_MOTOR_PORT 4
	
	#define SHOOTER_MOTOR_PORT 3
	#define TURRET_MOTOR_PORT 2
	#define PICKUP_MOTOR_PORT 6
	#define FEED_MOTOR_PORT 7

/* Button configuration. */
	#define MANIPULATOR_SHOOT_BUTTON 1
	#define MANIPULATOR_SUCK_BUTTON 3

	#define XBOX_SHOOT_BUTTON 1
	#define XBOX_SUCK_BUTTON 2

	#define KINECT_FORWARD_BUTTON 5
	#define KINECT_REVERSE_BUTTON 1
	#define KINECT_LEFT_BUTTON 3
	#define KINECT_RIGHT_BUTTON 4

	#define KINECT_SHOOT_BUTTON 2
	#define KINECT_SUCK_BUTTON 5
	#define KINECT_TURRET_LEFT_BUTTON 3
	#define KINECT_TURRET_RIGHT_BUTTON 4

/* Actuator polarity and speed configuration. */
	#define LEFT_DRIVE_POWER 1.0
	#define RIGHT_DRIVE_POWER 1.0
	
	#define SHOOTER_MOTOR_POWER -1.0
	#define FEED_MOTOR_POWER -1.0
	#define PICKUP_MOTOR_POWER 1.0
	
	#define TURRET_POWER 0.4

class Robot2009Kinect : public SimpleRobot {
	Joystick joystickManipulator;
	Joystick joystickDriveLeft;
	Joystick joystickDriveRight;
	
	KinectStick kinectDrive;
	KinectStick kinectManipulator;
	
	Victor motorDriveLeft;
	Victor motorDriveRight;

	Victor motorShooter;
	Victor motorTurret;
	Victor motorPickup;
	Victor motorFeed;
	
	bool kinectMode;
	SendableChooser *kinectModeSelector;
	
	bool demoMode;
	SendableChooser *demoModeSelector;
	
	double const100;
	double const90;
	double const80;
	double const70;
	double const60;
	double const50;
	double const40;
	double const30;
	double const20;
	double const10;
	double const0;
	
	double *speedModeMult;
	SendableChooser *speedModeSelector;
		
	public:
		Robot2009Kinect (void):
			joystickManipulator(MANIPULATOR_JOYSTICK_USB_PORT),
			joystickDriveLeft(LEFT_DRIVE_JOYSTICK_USB_PORT),
			joystickDriveRight(RIGHT_DRIVE_JOYSTICK_USB_PORT),
			kinectDrive(DRIVE_KINECT_IDENT),
			kinectManipulator(MANIPULATOR_KINECT_IDENT),
			motorDriveLeft(LEFT_DRIVE_MOTOR_PORT),
			motorDriveRight(RIGHT_DRIVE_MOTOR_PORT),
			motorShooter(SHOOTER_MOTOR_PORT),
			motorTurret(TURRET_MOTOR_PORT),
			motorPickup(PICKUP_MOTOR_PORT),
			motorFeed(FEED_MOTOR_PORT)
		{
			GetWatchdog().SetEnabled(false); // If you're just beginning, and nothing's going on, there's no need for Watchdog to be doing anything.
			
			kinectMode = false;
			
			kinectModeSelector = new SendableChooser();
			kinectModeSelector->AddDefault("Kinect Mode - OFF", (void*) false);
			kinectModeSelector->AddObject("Kinect Mode - ON", (void*) true);
			
			demoMode = false;
			
			demoModeSelector = new SendableChooser();
			demoModeSelector->AddDefault("Demo Mode - OFF", (void*) false);
			demoModeSelector->AddObject("Demo Mode - ON", (void*) true);
			
			const100 = 1;
			const90 = 0.9;
			const80 = 0.8;
			const70 = 0.7;
			const60 = 0.6;
			const50 = 0.5;
			const40 = 0.4;
			const30 = 0.3;
			const20 = 0.2;
			const10 = 0.1;
			const0 = 0;
			
			speedModeMult = &const100;
			
			speedModeSelector = new SendableChooser();
			speedModeSelector->AddDefault("Speed - 100%", &const100);
			speedModeSelector->AddObject("Speed - 90%", &const90);
			speedModeSelector->AddObject("Speed - 80%", &const80);
			speedModeSelector->AddObject("Speed - 70%", &const70);
			speedModeSelector->AddObject("Speed - 60%", &const60);
			speedModeSelector->AddObject("Speed - 50%", &const50);
			speedModeSelector->AddObject("Speed - 40%", &const40);
			speedModeSelector->AddObject("Speed - 30%", &const30);
			speedModeSelector->AddObject("Speed - 20%", &const20);
			speedModeSelector->AddObject("Speed - 10%", &const10);
			speedModeSelector->AddObject("Speed - 0%", &const0);
		}
		
		float Deadband (float power) {
			if (power > -0.2 && power < 0.2)
				return 0;
			
			return power;
		}

		void Autonomous (void) {
			GetWatchdog().SetEnabled(false); // No need for Watchdog in Autonomous, either.
		}

		void OperatorControl (void) {
			GetWatchdog().SetEnabled(true); // We do want Watchdog in Teleop, though.
			
			DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
			
			dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "                     ");
			dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Joystick Mode");
			
			SmartDashboard::GetInstance()->PutData("kinectMode?", kinectModeSelector);
			SmartDashboard::GetInstance()->PutData("demoMode?", demoModeSelector);
			SmartDashboard::GetInstance()->PutData("speedMode?", speedModeSelector);
			
			while (IsOperatorControl() && IsEnabled()) {
				GetWatchdog().Feed(); // Feed the Watchdog.
				
				kinectMode = (bool) kinectModeSelector->GetSelected();
				demoMode = (bool) demoModeSelector->GetSelected();
				speedModeMult = static_cast<double*>(speedModeSelector->GetSelected());
				
				if (kinectMode) {
					dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "                     ");
					dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Kinect Mode");
					
					if (!demoMode) {
						if (kinectDrive.GetRawButton(KINECT_FORWARD_BUTTON)) {
							motorDriveLeft.Set(LEFT_DRIVE_POWER * *speedModeMult);
							motorDriveRight.Set(RIGHT_DRIVE_POWER * -1 * *speedModeMult);
						} else if (kinectDrive.GetRawButton(KINECT_REVERSE_BUTTON)) {
							motorDriveLeft.Set(LEFT_DRIVE_POWER * -1 * *speedModeMult);
							motorDriveRight.Set(RIGHT_DRIVE_POWER * *speedModeMult);
						} else if (kinectDrive.GetRawButton(KINECT_LEFT_BUTTON)) {
							motorDriveLeft.Set(LEFT_DRIVE_POWER * -1 * *speedModeMult);
							motorDriveRight.Set(RIGHT_DRIVE_POWER * -1 * *speedModeMult);
						} else if (kinectDrive.GetRawButton(KINECT_RIGHT_BUTTON)) {
							motorDriveLeft.Set(LEFT_DRIVE_POWER * *speedModeMult);
							motorDriveRight.Set(RIGHT_DRIVE_POWER * *speedModeMult);
						} else {
							motorDriveLeft.Set(0);
							motorDriveRight.Set(0);
						}
					}
			
					if (kinectManipulator.GetRawButton(KINECT_SHOOT_BUTTON)) {
						motorShooter.Set(SHOOTER_MOTOR_POWER);
						motorFeed.Set(FEED_MOTOR_POWER);
						motorPickup.Set(PICKUP_MOTOR_POWER);
					} else if (kinectManipulator.GetRawButton(KINECT_SUCK_BUTTON)) {
						motorShooter.Set(SHOOTER_MOTOR_POWER * -1);
						motorFeed.Set(FEED_MOTOR_POWER * -1);
						motorPickup.Set(PICKUP_MOTOR_POWER * -1);
					} else {
						motorShooter.Set(0);
						motorFeed.Set(0);
						motorPickup.Set(0);
					}
				
					if (kinectManipulator.GetRawButton(KINECT_TURRET_LEFT_BUTTON)) {
						motorTurret.Set(TURRET_POWER);
					} else if(kinectManipulator.GetRawButton(KINECT_TURRET_RIGHT_BUTTON)) {
						motorTurret.Set(TURRET_POWER * -1);
					} else {
						motorTurret.Set(0);
					}
				} else {
					if (joystickDriveLeft.GetThrottle() == 0) {
						dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "                     ");
						dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Xbox Mode");
						
						motorDriveLeft.Set(Deadband(joystickManipulator.GetRawAxis(2) * -1 * *speedModeMult));
						motorDriveRight.Set(Deadband(joystickManipulator.GetRawAxis(5) * *speedModeMult));
						
						if (joystickManipulator.GetRawButton(XBOX_SHOOT_BUTTON) || (demoMode && joystickDriveRight.GetRawButton(XBOX_SHOOT_BUTTON))) {
							motorShooter.Set(SHOOTER_MOTOR_POWER);
							motorFeed.Set(FEED_MOTOR_POWER);
							motorPickup.Set(PICKUP_MOTOR_POWER);
						} else if (joystickManipulator.GetRawButton(XBOX_SUCK_BUTTON) || (demoMode && joystickDriveRight.GetRawButton(XBOX_SUCK_BUTTON))) {
							motorShooter.Set(SHOOTER_MOTOR_POWER * -1);
							motorFeed.Set(FEED_MOTOR_POWER * -1);
							motorPickup.Set(PICKUP_MOTOR_POWER * -1);
						} else {
							motorShooter.Set(0);
							motorFeed.Set(0);
							motorPickup.Set(0);
						}
						
						if (joystickManipulator.GetRawAxis(3) > 0.2 || (demoMode && joystickDriveRight.GetRawAxis(3) > 0.2)) {
							motorTurret.Set(TURRET_POWER);
						} else if(joystickManipulator.GetRawAxis(3) < -0.2 || (demoMode && joystickDriveRight.GetRawAxis(3) < -0.2)) {
							motorTurret.Set(TURRET_POWER * -1);
						} else {
							motorTurret.Set(0);
						}
					} else {
						dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "                     ");
						dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Joystick Mode");
						
						motorDriveLeft.Set(Deadband(joystickDriveLeft.GetY() * -1 * *speedModeMult));
						motorDriveRight.Set(Deadband(joystickDriveRight.GetY() * *speedModeMult));
						
						if (joystickManipulator.GetRawButton(MANIPULATOR_SHOOT_BUTTON)) {
							motorShooter.Set(SHOOTER_MOTOR_POWER);
							motorFeed.Set(FEED_MOTOR_POWER);
							motorPickup.Set(PICKUP_MOTOR_POWER);
						} else if (joystickManipulator.GetRawButton(MANIPULATOR_SUCK_BUTTON) || (demoMode && joystickDriveRight.GetRawButton(XBOX_SUCK_BUTTON))) {
							motorShooter.Set(SHOOTER_MOTOR_POWER * -1);
							motorFeed.Set(FEED_MOTOR_POWER * -1);
							motorPickup.Set(PICKUP_MOTOR_POWER * -1);
						} else {
							motorShooter.Set(0);
							motorFeed.Set(0);
							motorPickup.Set(0);
						}
						
						motorTurret.Set(joystickManipulator.GetX() * -1 * TURRET_POWER);
					}
				}
				
				dsLCD->UpdateLCD();
			}

			GetWatchdog().SetEnabled(false); // Teleop is done, so let's turn off Watchdog.
		}
		
		void Disabled (void) {
			GetWatchdog().SetEnabled(false);
		}
	};

START_ROBOT_CLASS(Robot2009Kinect);

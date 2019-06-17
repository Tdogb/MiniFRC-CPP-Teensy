#define WHEEL_DIA 0.051

#define X_BUTTON 1
#define TRI_BUTTON 3
#define SQR_BUTTON 0
#define O_BUTTON 2

#define L1 4
#define L2 5
#define R1 6
#define R2 7

#define DPAD_UP 8
#define DPAD_RIGHT 9
#define DPAD_DOWN 10
#define DPAD_LEFT 11

//Controls

#define LEVEL_UP DPAD_UP
#define LEVEL_DOWN DPAD_DOWN
#define ROTATION_LEFT_COURSE DPAD_LEFT
#define ROTATION_RIGHT_COURSE DPAD_RIGHT
#define ROTATION_LEFT_FINE SQR_BUTTON
#define ROTATION_RIGHT_FINE O_BUTTON
#define THROTTLE_AXIS yLAxis
#define TURN_AXIS xRAxis
#define BALL_INTAKE R2
#define BALL_OUTTAKE R1
#define HATCH_INTAKE L2
#define HATCH_OUTTAKE L1

//Packet size
#define ML 4

//Pins
#define ROTATION_STEP_PIN 11
#define ROTATION_DIR_PIN 12
#define LEADSCREW_STEP_PIN 29
#define LEADSCREW_DIR_PIN 30

#define R_MOTOR_FWD 6
#define R_MOTOR_RVS 5
#define R_MOTOR_EN 7

#define L_MOTOR_FWD 3
#define L_MOTOR_RVS 4
#define L_MOTOR_EN 2

#define R_ENC_A 9
#define R_ENC_B 10
#define L_ENC_A 25
#define L_ENC_B 26
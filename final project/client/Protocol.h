#pragma once

//CLIENT:


//sign in:
#define	CLIENT_SIGN_IN 200
#define	CLIENT_SIGN_OUT 201

//sign up:
#define	CLIENT_SIGN_UP 203

//room stuff:
#define	CLIENT_GET_EXIST_ROOMS 205
#define	CLIENT_USERS_IN_ROOM 207
#define	CLIENT_JOIN_EXISTING_ROOM 209
#define	CLIENT_LEAVE_ROOM 211
#define	CLIENT_CREATE_NEW_ROOM 213
#define	CLIENT_CLOSE_ROOM 215

// game stuff:
#define	CLIENT_START_GAME 217
#define	CLIENT_ANSWER 219
#define	CLIENT_LEAVE_GAME 222
#define	CLIENT_BEST_SCORE 223
#define	CLIENT_STATUS 225
#define	CLIENT_EXIT 299

//SERVER:

//sign in:
#define	SERVER_SIGN_IN_SUCCESS "1020"
#define	SERVER_SIGN_IN_WRONG_DETAILS "1021"
#define	SERVER_SIGN_IN_ALLREADY_CONNECTAED "1022"
#define	SERVER_SIGN_IN_SUCCESS_ADMIN "1023"

//sign up:
#define	SERVER_SIGN_UP_SUCCESS "1040"
#define SERVER_SIGN_UP_PASS_ILEGAL "1041"
#define SERVER_SIGN_UP_USERNAME_ALLREDY_EXIST "1042"
#define SERVER_SIGN_UP_USERNAME_ILEGAL "1043"
#define SERVER_SIGN_UP_OTHER "1044"

//join room:
#define SERVER_JOIN_ROOM_SUCCESS "1100"
#define SERVER_JOIN_ROOM_FULL "1101"
#define SERVER_JOIN_ROOM_NOT_EXIST "1102"

//creat room:
#define SERVER_CREATE_ROOM_SUCCESS "1140"
#define SERVER_CREATE_ROOM_FAIL "1141"


#define SERVER_SEND_QUESTION "118"
#define SERVER_ANSWER "120"
#define SERVER_CLOSE_ROOM "116"
#define SERVER_GET_EXIST_ROOMS "106"
#define SERVER_USER_IN_ROOM "108"
#define SERVER_LEAVE_ROOM "1120"
#define SERVER_END_GAME "121"
#define SERVER_BEST_SCORES "124"
#define SERVER_PERSONAL_STATUS "126"
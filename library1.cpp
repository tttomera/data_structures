/*
 * library1.cpp
 *
 *  Created on: 10 במאי 2017
 *      Author: Tomer Abraham
 */


#include "library1.h"
#include "school.h"

void* Init() {
	School *DS = new School();
	return (void*)DS;
}


StatusType AddStudent(void *DS, int StudentID, int Grade, int Power) {

	if(!DS) return INVALID_INPUT;
	return ((School*)DS)->addStudent(StudentID,Grade,Power);
}

StatusType AddTeam(void* DS, int TeamID){
	if(!DS)	return INVALID_INPUT;
	return ((School*)DS)->addTeam(TeamID);
}

StatusType MoveStudentToTeam(void* DS, int StudentID, int TeamID){
	if(!DS) return INVALID_INPUT;
	return ((School*)DS)->moveStudentToTeam(StudentID,TeamID);
}

StatusType GetMostPowerful(void *DS, int TeamID, int* StudentID){
	if(!DS)	return INVALID_INPUT;
	return ((School*)DS)->getMostPowerful(TeamID,StudentID);
}

StatusType RemoveStudent(void* DS, int StudentID){
	if(!DS)	return INVALID_INPUT;
	return ((School*)DS)->removeStudent(StudentID);
}

StatusType GetAllStudentsByPower(void *DS, int TeamID, int **Students,
		int *numOfStudents) {

	if(!DS) return INVALID_INPUT;
	return ((School*)DS)->getAllStudentsByPower(TeamID,Students,numOfStudents);
}


StatusType IncreaseLevel(void* DS, int Grade, int PowerIncrease){
	if(!DS)	return INVALID_INPUT;
	return ((School*)DS)->IncreaseLevel(Grade,PowerIncrease);
}


void Quit(void** DS){
	delete (School*)(*DS);
	*(DS)=NULL;
}


/*
 * student.h
 *
 *  Created on: 2 במאי 2017
 *      Author: Tomer Abraham
 */

#ifndef STUDENT_H_
#define STUDENT_H_

#include <iostream>
#include "Avl.h"

#define NOT_IN_TEAM -1


class Team;
class Student;


class Student_Ext{
	int id,power,grade;

public:
	//Student_Ext() : id(), grade(), power(){}

	Student_Ext(int id,int power,int grade) :
		id(id),power(power),grade(grade) {}

	int Power() const {
		return power;
	}

	int ID() const{
		 return id;
	}

	int Grade() const {
		return grade;
	}

	Student_Ext(const Student_Ext& student) :
		id(student.ID()),
		power(student.Power()),
		grade(student.Grade()) {}


	void IncPower(int power_points){
		power += power_points;
	}

	bool operator>(const Student_Ext& student) const{
		if(power > student.Power()) return true;
		else if(power < student.Power())	return false;
		return id < student.ID();
	}

	bool operator<(const Student_Ext& student) const{
		if(power < student.Power()) return true;
		else if(power > student.Power())	return false;
		return id > student.ID();
	}

	bool operator==(const Student_Ext& student) const{
		return((power == student.Power()) && (id == student.ID()));
	}

};

class Student{

	int id,power,grade,team_id;
	AVLtree<Student_Ext>* team_tree;

public:

	Student(int id) : id(id), power(-1), grade(-1), team_id(NOT_IN_TEAM),
		team_tree(NULL) {}

	Student(int id, int power, int grade) : id(id), power(power),
		grade(grade), team_id(NOT_IN_TEAM), team_tree(NULL){}

	int ID() const{
		 return id;
	}

	int Grade() const {
		return grade;
	}

	int TeamID() const {
			return team_id;
	}

	int Power() const {
			return power;
	}

	AVLtree<Student_Ext>* GetTeamTree() const {
		return team_tree;
	}

	Student(const Student& student) :
		id(student.ID()),
		power(student.Power()),
		grade(student.Grade()),
		team_id(student.TeamID()),
		team_tree(student.GetTeamTree()) {}


	bool operator>(const Student& student) const{
		return (id > student.ID());
	}

	bool operator<(const Student& student) const{
		return (id < student.ID());
	}

	bool operator==(const Student& student) const{
		return (id == student.ID());
	}

	void addTeamTree(AVLtree<Student_Ext>* ptr){
		team_tree = ptr;
	}

	void changeTeamId(int team) {
		team_id = team;
	}

	void IncPower(int power_points){
		power += power_points;
	}

};



#endif /* STUDENT_H_ */

/*
 * team.h
 *
 *  Created on: 2 במאי 2017
 *      Author: Tomer Abraham
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "student.h"

#define NO_STUDENTS_IN_TEAM -1

class Team{
	int id;
	AVLtree<Student_Ext> students_tree;
	int strongest;

public:

	//Team() : id(), students_tree(NULL),  strongest(NO_STUDENTS_IN_TEAM){}

	Team(int id) : id(id), students_tree(), strongest(NO_STUDENTS_IN_TEAM) {}


	~Team() {}

	int ID () const {
		return id;
	}

	bool operator==(const Team& team) const{
		return id == team.ID();
	}

	bool operator>(const Team& team) const{
		return id > team.ID();
	}

	bool operator<(const Team& team) const{
		return id < team.ID();
	}

	AVLtree<Student_Ext>& getTree() {
		return students_tree;
	}

	int Strongest() const {
		return strongest;
	}

	void updatedStrongest() {

		if(students_tree.getRoot() == NULL) strongest = NO_STUDENTS_IN_TEAM;
		else {
			strongest = (students_tree.getMax())->obj.ID();
		}
	}

};

#endif /* TEAM_H_ */

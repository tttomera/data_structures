/*
 * school.h
 *
 *  Created on: 10 במאי 2017
 *      Author: Tomer Abraham
 */

#ifndef SCHOOL_H_
#define SCHOOL_H_

#include "team.h"
#include "library1.h"
#include <cstdlib>

#define NO_STUDENTS_YET -1


//merge two given arrays into one sorted array
template<class T>
static void merge(T a[], int na, T b[], int nb, T c[]) {
	int ia=0, ib=0, ic=0;
	for(; (ia < na) && (ib < nb); ic++){
		if(a[ia] < b[ib]) {
			c[ic] = a[ia];
			ia++;
		}else if(b[ib] < a[ia]) {
			c[ic] = b[ib];
			ib++;
		}
	}
	for(;ia < na; ia++, ic++) c[ic] = a[ia];
	for(;ib < nb; ib++, ic++) c[ic] = b[ib];
}

class School{
	AVLtree<Student> by_id;
	AVLtree<Student_Ext> by_power;
	AVLtree<Team> teams;
	int strongest;


	//increase the power of all the students in the array, by the amount of
	//PowerIncrease
	StatusType makeInc(int Grade, int PowerIncrease,
			int size, Student_Ext tree_array[]){
		Student_Ext* in_grade = (Student_Ext*)malloc(sizeof(Student_Ext)*size);
		if(!in_grade){ //לאפס
			free(tree_array);
			return ALLOCATION_ERROR;
		}
		Student_Ext* not_in_grade = (Student_Ext*)malloc(sizeof(Student_Ext)*size);
		if(!not_in_grade){
			free(in_grade);
			free(tree_array);
			return ALLOCATION_ERROR;
		}
		int idx_in_grade = 0, idx_not_in_grade = 0;
		for(int i=0; i<size; i++){
			if(tree_array[i].Grade() == Grade) in_grade[idx_in_grade++] = tree_array[i];
			else not_in_grade[idx_not_in_grade++] = tree_array[i];
		}
		for(int i=0; i<idx_in_grade; i++){
			in_grade[i].IncPower(PowerIncrease);
		}
		merge(in_grade,idx_in_grade,not_in_grade,idx_not_in_grade,tree_array);
		free(in_grade);
		free(not_in_grade);

		return SUCCESS;
	}
	void makeIncForStu(int Grade, int PowerIncrease,
			int size, Student tree_array[]){
		for(int i=0; i<size; i++){
			if(tree_array[i].Grade() == Grade)	tree_array[i].IncPower(PowerIncrease);
		}
	}
	void moveInorder(AVLnode<Team>* root) {
		if(root) {
			moveInorder(root->left);
			root->obj.updatedStrongest();
			moveInorder(root->right);
		}
	}

	//increase the power of the students in each team, which belongs to this
	//specific grade
	StatusType increaseForEachTeam(int Grade, int PowerIncrease){
		if(teams.NumOfNodes() == 0) return SUCCESS;
		Team* arr = teams.moveIntoArray();
		for(int i=0; i<teams.NumOfNodes(); i++){
			if(arr[i].getTree().isEmpty()) continue;
			Student_Ext* tree_array = arr[i].getTree().moveIntoArray();
			if(!tree_array){
				free(arr);
				return ALLOCATION_ERROR;
			}

			if(makeInc(Grade,PowerIncrease,
					arr[i].getTree().NumOfNodes(),tree_array)!=SUCCESS){
				free(tree_array);
				free(arr);
				return ALLOCATION_ERROR;
			}
			arr[i].getTree().insertToTree(arr[i].getTree().getRoot(),tree_array,0);
			arr[i].updatedStrongest();
			free(tree_array);
		}
		free(arr);

		moveInorder(teams.getRoot());


		return SUCCESS;
	}

public:
	School() : by_id(), by_power(), teams(), strongest(NO_STUDENTS_YET){}


	~School(){}

	AVLtree<Student> getByIdTree() const {
		return by_id;
	}

	AVLtree<Student_Ext> getByPowerTree() const {
		return by_power;
	}

	StatusType addStudent(int studentId, int grade, int power) {

		if(studentId <= 0 || grade < 0 || power <= 0) return INVALID_INPUT;

		if(by_id.find(by_id.getRoot(),Student(studentId,power,grade)) != NULL) {
			if(by_id.find(by_id.getRoot(),Student(studentId,power,grade))
					->obj.ID() == studentId)
							return FAILURE;
		}
		//logn
		if(!by_id.insert(Student(studentId,power,grade))) return ALLOCATION_ERROR;

		//logn

		if(!(by_power.insert(Student_Ext(studentId,power,grade)))){
			by_id.remove(Student(studentId,power,grade));
			return ALLOCATION_ERROR;
		}

		//logn
		strongest = by_power.getMax()->obj.ID();
		return SUCCESS;
	}


	StatusType addTeam(int teamId) {
		if(teamId <= 0)	return INVALID_INPUT;

		if(teams.find(teams.getRoot(),Team(teamId)) != NULL) return FAILURE;

		if(!teams.insert(Team(teamId)))  return ALLOCATION_ERROR;

		return SUCCESS;
	}


	StatusType moveStudentToTeam(int student_id, int team_id){
		if(student_id <= 0 || team_id <= 0)	return INVALID_INPUT;

		AVLnode<Student>* stu_node = by_id.find
				(by_id.getRoot(),Student(student_id));
		if(!stu_node || stu_node->obj.ID() != student_id) return FAILURE;

		AVLnode<Team>* team_node = teams.find(teams.getRoot(),Team(team_id));
		if(!team_node || team_node->obj.ID() != team_id) return FAILURE;

		if(team_id == stu_node->obj.TeamID()) return SUCCESS;


		AVLtree<Student_Ext>* privious_tree = stu_node->obj.GetTeamTree();
		int privious_team_id = stu_node->obj.TeamID();


		if(!team_node->obj.getTree().insert(Student_Ext(student_id,
				stu_node->obj.Power(),stu_node->obj.Grade()))) {
			return ALLOCATION_ERROR;
		}

		stu_node->obj.addTeamTree(&(team_node->obj.getTree()));
		stu_node->obj.changeTeamId(team_id);

		//he is now in the new team tree

		//updated the strongest and remove from privious tree
		if(privious_tree) {
			privious_tree->remove(Student_Ext(student_id,
					stu_node->obj.Power(),stu_node->obj.Grade()));

			AVLnode<Team>* prev_team_node = teams.find(teams.getRoot(),
					Team(privious_team_id));
				prev_team_node->obj.updatedStrongest();
		}


		//updated the strongest in the new tree
		team_node->obj.updatedStrongest();
		moveInorder(teams.getRoot());
		strongest = by_power.getMax()->obj.ID();


		return SUCCESS;
	}

	StatusType getMostPowerful(int TeamID, int *StudentID) {

		if(!StudentID || TeamID == 0) return INVALID_INPUT;

		AVLnode<Team>* node ;
		if(TeamID>0) {
			node = teams.find(teams.getRoot(), TeamID);
			if(!node || node->obj.ID() != TeamID) return FAILURE;
		}


		//no students at all

		if(TeamID < 0) {
			if(by_id.isEmpty()) {
				*StudentID = -1;
				return SUCCESS;
			} else {
				*StudentID = strongest;
				return SUCCESS;
			}
		}

		//such a team is exists
		if(node->obj.getTree().isEmpty()) {
			*StudentID = -1;
			return SUCCESS;
		}else{
			*StudentID = node->obj.Strongest();
		}

		return SUCCESS;
	}

	StatusType removeStudent(int ID){
		if(ID <= 0) return INVALID_INPUT;
		Student same_student(ID);
		AVLnode<Student>* node = by_id.find(by_id.getRoot(),same_student);
		if(!node || node->obj.ID() != ID) return FAILURE;

		//students is exits

		if(node->obj.GetTeamTree()) {
			node->obj.GetTeamTree()->remove(Student_Ext(ID,node->obj.Power(),
					node->obj.Grade()));
			AVLnode<Team>* team = teams.find(teams.getRoot(),node->obj.TeamID());
			team->obj.updatedStrongest();
		}
		moveInorder(teams.getRoot());

		by_power.remove(Student_Ext(ID,node->obj.Power(),
				node->obj.Grade()));
		by_id.remove(same_student);
		if(by_power.getMax() != NULL) strongest = by_power.getMax()->obj.ID();
		//if(!(by_power.isEmpty())) strongest = by_power.getMax()->obj.ID();
		else strongest = NO_STUDENTS_YET;

		//moveInorder(by_power.getRoot());



		return SUCCESS;
	}

	StatusType getAllStudentsByPower(int teamId, int **students,
				int* numOfStudents) {

		if(!students || !numOfStudents || teamId == 0) return INVALID_INPUT;

		AVLnode<Team>* node;
		Team same_team(teamId);
		if(teamId>0) {
			node = teams.find(teams.getRoot(),same_team);
			if(!node) {
				*students = NULL;
				*numOfStudents = 0;
				return FAILURE;
			}
		}
		//such team exists

		if(teamId < 0) {
			if(by_id.isEmpty()) {
				*students = NULL;
				*numOfStudents = 0;
				return SUCCESS;
			} else {
				Student_Ext* array = by_power.moveIntoArray();
				int* int_array = (int*)malloc(sizeof(int)*by_power.NumOfNodes());
				for(int i=0; i < by_power.NumOfNodes(); i++) {
					int_array[i] = (array[by_power.NumOfNodes() -1 -i]).ID();
				}
				free(array);
				*numOfStudents = by_power.NumOfNodes();
				*students = int_array;
				return SUCCESS;
			}
		}

		AVLtree<Student_Ext>& tree = node->obj.getTree();
		if(tree.isEmpty()) {
			*students = NULL;
			*numOfStudents = 0;
			return SUCCESS;
		} else {

			Student_Ext* array = tree.moveIntoArray();
			int* int_array = (int*)malloc(sizeof(int)*tree.NumOfNodes());
			for(int i=0; i < tree.NumOfNodes(); i++) {
				int_array[i] = (array[tree.NumOfNodes()-1 -i]).ID();
			}
				free(array);
				*numOfStudents = tree.NumOfNodes();
				*students = int_array;
				return SUCCESS;
		}

		return SUCCESS;
	}


	StatusType IncreaseLevel(int Grade, int PowerIncrease){
		if(Grade < 0 || PowerIncrease <= 0)	return INVALID_INPUT;
		int size = by_power.NumOfNodes();
		if(size == 0)	return SUCCESS;
		Student_Ext* tree_array = by_power.moveIntoArray();
		if(!tree_array)	return ALLOCATION_ERROR;
		if(makeInc(Grade,PowerIncrease,size,tree_array)!=SUCCESS){
			return ALLOCATION_ERROR;
		}
		by_power.insertToTree(by_power.getRoot(),tree_array,0);
		strongest = by_power.getMax()->obj.ID();
		free(tree_array);

		Student* tree_array2 = by_id.moveIntoArray();
		if(!tree_array2)	return ALLOCATION_ERROR;
		makeIncForStu(Grade,PowerIncrease,size,tree_array2);
		by_id.insertToTree(by_id.getRoot(),tree_array2,0);
		free(tree_array2);
		return increaseForEachTeam(Grade,PowerIncrease);
	}


};


#endif /* SCHOOL_H_ */

#ifndef MONTHLY_REPORT_H
#define MONTHLY_REPORT_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using std::string;
using std::vector;


class reports{

	private:
		string r_name;
		string r_email;
		string r_department;		// Using std::vector for dynamic lines
		string r_position;			// and std::string for constant lines from CVS file
		vector<string> r_project;
		vector<string> r_task;
		vector<string> r_date;
		vector<int> r_hours;

	public:
		reports();
		reports(
			string name,
			string email, 
			string department,
			string position,
			string project,
			string task,
			string date,
			int hours);
		reports(const reports &obj);
		reports& operator=(const reports &obj);


		void show_all_worker_reports();
		void add_report(
			string project,
    		string task,
   			string date,
    		int hours);
		string monthly_worker_report() ;
		friend void monthly_report_to_CVS(
			 std::unordered_map<string, reports> &map, 
			 std::set<string> &keys) ;

};





#endif
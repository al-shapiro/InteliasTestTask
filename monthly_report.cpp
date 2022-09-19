#include <iostream>
#include <fstream>
#include <sstream>

#include "monthly_report.hpp"

	//CONSTRUCTORS

reports::reports(){

	//do nothing
}

reports::reports(
			string name,
			string email, 
			string department,
			string position,
			string project,
			string task,
			string date,
			int hours){

	r_name = name;
	r_email = email;
	r_department = department;
	r_position = position;
	r_project.push_back(project);
	r_task.push_back(task);
	r_date.push_back(date);
	r_hours.push_back(hours);
}

reports::reports(const reports &obj){

    r_name = obj.r_name;
    r_email = obj.r_email;
    r_department = obj.r_department;
    r_position = obj.r_position;
    r_project = obj.r_project;
    r_task = obj.r_task;
    r_date = obj.r_date;
    r_hours = obj.r_hours;

}

reports& reports::operator=(const reports &obj){

    r_name = obj.r_name;
    r_email = obj.r_email;
    r_department = obj.r_department;
    r_position = obj.r_position;
    r_project = obj.r_project;
    r_task = obj.r_task;
    r_date = obj.r_date;
    r_hours = obj.r_hours;

    return *this;

}

	//CLASS MEMBER-FUNCTIONS

void reports::show_all_worker_reports(){	// Show all of the reports from this employee

    std::cout<<"\nWorker's name: " << r_name << std::endl;
    std::cout<<"Email: " << r_email << std::endl;
    std::cout<<"Department: " << r_department << std::endl;
    std::cout<<"Position: " << r_position << std::endl;

    for(unsigned i = 0 ; i < r_project.size() ; i++){

        std::cout<<"\nProject: " << r_project[i] << std::endl;
        std::cout<<"Task: " << r_task[i] << std::endl;
        std::cout<<"Report's date: " << r_date[i] << std::endl;
        std::cout<<"Logged hours: " <<r_hours[i] << std::endl;
    }

}

void reports::add_report(	// Add report to the existing report object
        string project,
        string task,
        string date,
        int hours){

    r_project.push_back(project);
    r_task.push_back(task);
    r_date.push_back(date);
    r_hours.push_back(hours);   

}

string reports::monthly_worker_report() {	//Monthly report from one worker

	vector<std::pair<string,string>> hours_per_month;

    int total_hours = 0 , month_number = 0;

    const string months[12] = {"January" , "February" , "March" , "April" ,
    					"May" , "June" , "July" , "Aug" , "September" ,
    					"October" , "November" , "December"};

    
    for(unsigned i = 0 ; i < r_hours.size() - 1 ; ++i){ //adding hours per month for each month

        total_hours += r_hours[i];

    	if(r_date[i].substr(5,2) != r_date[i+1].substr(5,2)){

    		month_number = atoi((r_date[i].substr(5,2)).c_str());

    		std::pair<string,string> current_month;
    		current_month.first = std::to_string(total_hours);
    		current_month.second = months[month_number-1] + ' ' + r_date[i].substr(0,4);
    		hours_per_month.push_back(current_month);

    		total_hours = 0;
    	}

	  }

    total_hours += r_hours[r_hours.size()-1];
    month_number = atoi((r_date[r_date.size()-1].substr(5,2)).c_str());

    std::pair<string,string> current_month;
    current_month.first = std::to_string(total_hours);
    current_month.second = months[month_number-1] + ' ' + r_date[r_date.size()-1].substr(0,4);
    hours_per_month.push_back(current_month);

	string res_report = "";

    for(unsigned i = 0 ; i < hours_per_month.size(); ++i){

    	res_report += r_name + ';' + hours_per_month[i].second + ';' + hours_per_month[i].first + '\n';
    }

	return res_report;
}

void monthly_report_to_CVS(
			 std::unordered_map<string, reports> &map, 
			 std::set<string> &keys) {

	std::fstream final_report_file;
	final_report_file.open("monthly_report.csv",std::ios::out);

	if(final_report_file.is_open()){

		final_report_file << "Name;Month;Total hours\n";	//creating a header in resulting CVS file
		for(std::set<string>::iterator it = keys.begin() ; it != keys.end() ; ++it){

			final_report_file << map[*it].monthly_worker_report();
		}

		final_report_file.close();

		std::cout <<"\nAll done!\n";
	}

	else{

		std::cout<<"\nERROR CREATING A FILE\n";
	}

}

	//MAIN

int main(){

	std::cout<<"\nPlease input the path to the CSV file(or if it's in a current folder - it's name with resolution): ";

	string path_to_file = "";
	std::cin >> path_to_file;

    std::ifstream reports_file; 
    reports_file.open(path_to_file);

    if(reports_file.is_open()){

        string line = "";

        getline(reports_file , line); // getting rid of the header in CVS file

        std::set<string> worker_names; 

        std::unordered_map<string, reports> mapped_reports;

        while(getline(reports_file , line)){

            string name;
            string email;
            string department;
            string position;
            string project;
            string task;
            string date;
            int hours;

            string buffer_str = ""; //std::string buffer to convert string to int

            std::stringstream report_line(line);

            getline(report_line , name , ';');
            getline(report_line , email , ';');
            getline(report_line , department , ';');
            getline(report_line , position , ';');
            getline(report_line , project , ';');
            getline(report_line , task , ';');
            getline(report_line , date , ';');
            getline(report_line , buffer_str , ';');

            hours =  std::atoi(buffer_str.c_str());

            std::pair<std::set<string>::iterator,bool> insert_result = worker_names.insert(name); // using std::set to create key-names for std::unordered_map and 
            																			 // check if this worker's report was previously encountered

            if(insert_result.second){	// First time encountering this worker's report

                reports worker_report(name , email , department , position , project , task , date , hours); // creating a new "reports" object
                mapped_reports[*insert_result.first] = worker_report; // adding "reports" object and it's key-name to std::unordered_map

            }
            else{	// Already encountered this worker's report, just adding to the existing object

                mapped_reports[*insert_result.first].add_report(project , task , date , hours); 

            } 

        }

        monthly_report_to_CVS(mapped_reports , worker_names);
    }

    else{

    	std::cout<<"\nERROR OPENING FILE\n";
    }

    return 0;
}
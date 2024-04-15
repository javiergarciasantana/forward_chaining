#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef FODWARD_CHAINING_H
#define FORWARD_CHAINING_H

class FwdChaining {
 public:
  FwdChaining(std::ifstream& input, std::string text_file_name) {
    input.open(text_file_name, std::ios_base::in);
    if (input.is_open()) {
      std::string line, aux;
      while(getline(input, line)) {
        if (line == "1) Rules") {
          while (getline(input, line) && line != "") {
            for (int i = line.length() - 1; i >= 2; i = i - 2) {
              aux += line[i];
            }
            rules_.push_back(std::make_pair(std::string(aux.rbegin(), aux.rend()), line[0]));
            aux.clear();
          }
        }
        if (line == "2) Facts") {
          while (getline(input, line) && line != "") {
            for (int i = 0; i < line.length(); ++i) {
              if (line[i] != ' ') {
                facts_.push_back(line[i]);
              }
            }
          }
          initial_facts_size_ = facts_.size();
        }
        if (line == "3) Goal") {
          while (getline(input, line) && line != "") {
            goal_ = line[0];
          }
        }
      }
      std::vector<std::vector<char> > succesors(facts_.size());
      flags1_.resize(rules_.size(), false);
      flags2_.resize(rules_.size(), false);
      
      if (output_file.is_open()) {
        DataPrint(output_file);
        PrintResult(output_file, FindGoal(output_file));
      }
    }
  }

  void DataPrint(std::ostream& out);
  void PrintFacts(std::ostream& out);
  void PrintLHS(std::ostream& out, const std::string kRuleLhs);
  void PrintResult(std::ostream& out, int goal_reached);

  bool ContainsGoal();
  std::pair<char,bool> IsAFact(const std::string sequence);
  bool FactsNotContains(const char ch);
  void Write();
  
  int FindGoal(std::ostream& out);

 private:
  std::ofstream output_file{"output.txt"};
  std::vector<std::pair<std::string, char> > rules_;
  std::vector<bool> flags1_, flags2_;
  std::vector<char> facts_;
  int initial_facts_size_;
  std::vector<int> path_;
  char goal_;
};

#endif
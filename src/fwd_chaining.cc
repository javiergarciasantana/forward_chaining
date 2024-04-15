#include "fwd_chaining.h"

void FwdChaining::DataPrint(std::ostream& out) {
  out << "PART 1. Data\n";
  out << "\n   1) Rules:\n";
  for(int i = 0; i < rules_.size(); ++i) {
    out << "      R" << i + 1 << ": ";
    PrintLHS(out, rules_[i].first); 
    out << " -> " << rules_[i].second << std::endl;
  }
  out << "   2) Facts ";
  for (int i = 0; i < facts_.size(); ++i) {
    out << facts_[i] << ", ";
  }
  out << std::endl << "   3) Goal " << goal_ << std::endl;
}

void FwdChaining::PrintFacts(std::ostream& out) {
  out << " Facts";
  for (int i = 0; i < facts_.size(); ++i) {
    if (i == initial_facts_size_ - 1) {
      out << " " << facts_[i];
      out << " and";
    } else if (i != facts_.size() - 1) {
      out << " " << facts_[i] << ",";
    } else {
      out << " " << facts_[i];
    }
  }
  out << "." << std::endl;
}

void FwdChaining::PrintLHS(std::ostream& out, const std::string kRuleLhs) {
  for (int i = 0; i < kRuleLhs.length(); ++i) {
    out << kRuleLhs[i];
    if (i != kRuleLhs.length() - 1) {
      out << ",";
    } 
  }
}

bool FwdChaining::ContainsGoal() {
  for (int i = 0; i < facts_.size(); ++i) {
    if (facts_[i] == goal_) {
      return true;
    }
  }
  return false;
}

std::pair<char,bool> FwdChaining::IsAFact(const std::string kSequence) {
  std::vector<bool> flags(kSequence.size(), false);
  for (int i = 0; i < kSequence.size(); ++i) {
    for (int j = 0; j < facts_.size(); ++j) {
      if (kSequence[i] == facts_[j]) {
        flags[i] = true;
      }
    }
  }
  for (int i = 0; i < flags.size(); ++i) {
    if (!flags[i]) {
      return std::make_pair(kSequence[i], false);
    }
  }
  return std::make_pair(' ', true);
}

bool FwdChaining::FactsNotContains (const char ch) {
  for (int i = 0; i < facts_.size(); ++i) {
    if (ch == facts_[i]) {
      return false;
    }
  }
  return true;
}

int FwdChaining::FindGoal(std::ostream& out) {
  out << "\nPART 2: Trace" << std::endl;
  if (ContainsGoal()) {
    out << "     Goal already in facts. No path" << std::endl;
    return 1;
  }
  int it = 1;
  while (!ContainsGoal() && it <= rules_.size()) {
    out << "\n   ITERATION " << it << std::endl;
    for (int i = 0; i < rules_.size(); ++i) {        
      //Checking wether lhs is a fact and if rhs is not a fact already
      if (IsAFact(rules_[i].first).second && FactsNotContains(rules_[i].second)) {
        facts_.push_back(rules_[i].second);
        path_.push_back(i + 1);
        flags1_[i] = true;
        out << "     R" << i + 1 << ":";
        PrintLHS(output_file, rules_[i].first); 
        out << "->" << rules_[i].second << " apply. Raise flag1.";
        PrintFacts(output_file);
        break;
      //Checking for flag2(RHS in facts already taken into account)
      } else if (flags2_[i]) {
        out << "     R" << i + 1 << ":";
        PrintLHS(output_file, rules_[i].first); 
        out << "->" << rules_[i].second << " skip, because flag2 raised." << std::endl;
      //Checking for flag1(Rule already taken into account)
      } else if (flags1_[i]) {
        out << "     R" << i + 1 << ":";
        PrintLHS(output_file, rules_[i].first);  
        out << "->" << rules_[i].second << " skip, because flag1 raised." << std::endl;   
        //If lhs is missing the fact, rule is not aplicable       
      } else if (!IsAFact(rules_[i].first).second) {
        out << "     R" << i + 1 << ":";
        PrintLHS(output_file, rules_[i].first); 
        out << "->" << rules_[i].second << " not applied, because of lacking " << IsAFact(rules_[i].first).first << "." << std::endl; 
        //RHS is a rule already
      } else  if (!FactsNotContains(rules_[i].second)) {
        out << "     R" << i + 1 << ":";
        PrintLHS(output_file, rules_[i].first); 
        out << "->" << rules_[i].second << " not applied because RHS in facts. Raise flag2." << std::endl;
        flags2_[i] = true;
      }
    }
    //std::cout << "Facts " << facts_.size() << std::endl;
    ++it;
  }
  if (ContainsGoal()) {
    out << "     Goal achieved." << std::endl;
    return 0;
  } else {
    out << "     Goal not achieved." << std::endl;
    return 2;
  }
}

void FwdChaining::PrintResult(std::ostream& out, int goal_reached) {
  out << "\nPART 3: Results" << std::endl;
  if (goal_reached == 0) {
    out << "   1) Goal " << goal_ << " achieved." << std::endl;
    out << "   2) Path";
    for (int i = 0; i < path_.size(); ++i) {
      out << " R" << path_[i] << ",";
    }
    out << std::endl;
  } else if (goal_reached == 1) {
    out << "   1) Goal " << goal_ << " is already in facts. Empty path." << std::endl;
  } else if (goal_reached == 2) {
    out << "   1) Goal " << goal_ << " not achieved." << std::endl;
    out << "   2) No Path" << std::endl;
  }
}
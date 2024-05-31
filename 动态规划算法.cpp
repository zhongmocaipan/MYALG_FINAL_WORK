#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// 读取CSV文件
void readCSV(const string& filename, vector<int>& demand, int& supply) {
	ifstream file(filename);
	string line;
	getline(file, line); // 跳过标题行
	while (getline(file, line)) {
		stringstream ss(line);
		string demandStr, supplyStr;
		getline(ss, demandStr, ',');
		getline(ss, supplyStr, ',');
		demand.push_back(stoi(demandStr));
		if (!supplyStr.empty()) {
			supply = stoi(supplyStr);
		}
	}
}

// 动态规划算法
vector<vector<int>> dynamicProgramming(const vector<int>& demand, int supply) {
	int n = demand.size();
	vector<vector<int>> dp(n + 1, vector<int>(supply + 1, 0));
	dp[0][0] = 1;

	for (int i = 1; i <= n; ++i) {
		for (int j = 0; j <= supply; ++j) {
			if (j >= demand[i - 1]) {
				dp[i][j] = dp[i - 1][j] + dp[i - 1][j - demand[i - 1]];
			}
			else {
				dp[i][j] = dp[i - 1][j];
			}
		}
	}
	return dp;
}

// 输出CSV文件
void writeCSV(const string& filename, const vector<vector<int>>& dp) {
	ofstream file(filename);
	for (const auto& row : dp) {
		for (size_t i = 0; i < row.size(); ++i) {
			file << row[i];
			if (i < row.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
	}
}

int main() {
	vector<int> demand;
	int supply = 0;
	readCSV("input.csv", demand, supply);

	vector<vector<int>> dp = dynamicProgramming(demand, supply);

	writeCSV("output_dp.csv", dp);

	cout << "动态规划结果已写入output_dp.csv" << endl;
	return 0;
}

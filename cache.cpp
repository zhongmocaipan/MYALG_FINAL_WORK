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
int dynamicProgramming(const vector<int>& demand, int supply) {
	int n = demand.size();
	vector<int> dp(supply + 1, 0);
	dp[0] = 1;

	for (int i = 0; i < n; ++i) {
		for (int j = supply; j >= demand[i]; --j) {
			dp[j] += dp[j - demand[i]];
		}
	}
	return dp[supply];
}

int main() {
	vector<int> demand;
	int supply = 0;
	readCSV("input.csv", demand, supply);

	int result = dynamicProgramming(demand, supply);

	ofstream file("output_dp.csv");
	file << result << endl;

	cout << "动态规划结果已写入output_dp.csv" << endl;
	return 0;
}

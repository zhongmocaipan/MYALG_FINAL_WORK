#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <limits>

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
		if (!demandStr.empty()) {
			demand.push_back(stoi(demandStr));
		}
		if (!supplyStr.empty()) {
			supply = stoi(supplyStr);
		}
	}
}

// 输出CSV文件
void writeCSV(const string& filename, const vector<int>& solution) {
	ofstream file(filename);
	for (size_t i = 0; i < solution.size(); ++i) {
		file << solution[i];
		if (i < solution.size() - 1) {
			file << ",";
		}
	}
	file << "\n";
}

struct Particle {
	vector<int> position;
	vector<int> velocity;
	vector<int> bestPosition;
	int bestFitness;
};

// 评估适应度函数
int evaluate(const vector<int>& position, const vector<int>& demand, int supply) {
	int totalDemand = 0;
	for (int i = 0; i < position.size(); ++i) {
		totalDemand += position[i] * demand[i];
	}
	return abs(supply - totalDemand);
}

// 粒子群优化算法
vector<int> particleSwarmOptimization(const vector<int>& demand, int supply, int numParticles, int maxIterations) {
	int numDimensions = demand.size();
	vector<Particle> particles(numParticles);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 1);

	for (auto& particle : particles) {
		particle.position.resize(numDimensions);
		particle.velocity.resize(numDimensions);
		particle.bestPosition.resize(numDimensions);
		for (int i = 0; i < numDimensions; ++i) {
			particle.position[i] = dis(gen);
			particle.velocity[i] = 0;
		}
		particle.bestPosition = particle.position;
		particle.bestFitness = evaluate(particle.position, demand, supply);
	}

	vector<int> globalBestPosition = particles[0].bestPosition;
	int globalBestFitness = particles[0].bestFitness;

	for (const auto& particle : particles) {
		if (particle.bestFitness < globalBestFitness) {
			globalBestFitness = particle.bestFitness;
			globalBestPosition = particle.bestPosition;
		}
	}

	uniform_real_distribution<> distReal(0.0, 1.0);
	double w = 0.5, c1 = 1.0, c2 = 1.0;

	for (int iter = 0; iter < maxIterations; ++iter) {
		for (auto& particle : particles) {
			for (int i = 0; i < numDimensions; ++i) {
				double r1 = distReal(gen);
				double r2 = distReal(gen);

				particle.velocity[i] = w * particle.velocity[i] + c1 * r1 * (particle.bestPosition[i] - particle.position[i]) + c2 * r2 * (globalBestPosition[i] - particle.position[i]);
				particle.position[i] += particle.velocity[i];

				if (particle.position[i] < 0) particle.position[i] = 0;
				if (particle.position[i] > 1) particle.position[i] = 1;
			}

			int fitness = evaluate(particle.position, demand, supply);
			if (fitness < particle.bestFitness) {
				particle.bestFitness = fitness;
				particle.bestPosition = particle.position;
			}

			if (fitness < globalBestFitness) {
				globalBestFitness = fitness;
				globalBestPosition = particle.position;
			}
		}
	}

	return globalBestPosition;
}

int main() {
	vector<int> demand;
	int supply = 0;
	readCSV("input.csv", demand, supply);

	int numParticles = 30;
	int maxIterations = 100;
	vector<int> bestSolution = particleSwarmOptimization(demand, supply, numParticles, maxIterations);

	writeCSV("output_pso.csv", bestSolution);

	cout << "粒子群优化结果已写入output_pso.csv" << endl;
	return 0;
}

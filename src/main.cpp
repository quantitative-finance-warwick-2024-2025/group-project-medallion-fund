#include <iostream>
#include "fileio.hpp"
#include "matrix.hpp"
#include "general_agent.hpp"
#include "specialised_agents.hpp"

int main()
{
	FileIO file;
	std::string path = "/Users/akshayzine/Academics/Programming_Group_Project/Akshay_Branch/group-project-medallion-fund/Assets.csv";
	auto [assetnames, assetvals] = file.read(path);

	// price matrix
	Matrix asset_vals(assetvals);

	bool index_present = true;
	if (index_present)
	{
		asset_vals = asset_vals.slicecols(1, asset_vals.NCols());
	}

	// time-period N and number of risky assets M
	int N = asset_vals.NRows();
	int M = asset_vals.NCols() - 1;

	// Bond Returns and Asset Prices
	std::vector<double> bond_rets = asset_vals.getcol(0);
	Matrix asset_prices = asset_vals.slicecols(1, M + 1);

	// Agents

	RiskHater riskhater(M, N);
	NaiveInvestor naiveinvestor(M, N);
	unsigned int l = 10; // lookback for markowitz
	double target_return_1 = 0.1;
	MarkowitzSavvy markowitzsavvy1(M, N, l, target_return_1);
	double target_return_2 = 0.2;
	MarkowitzSavvy markowitzsavvy2(M, N, l, target_return_2);
	double target_return_3 = 0.3;
	MarkowitzSavvy markowitzsavvy3(M, N, l, target_return_3);

	// setup/initialisation
	double bond_ret = bond_rets[0];								// bond return at time beginning
	std::vector<double> asset_price_r = asset_prices.getrow(0); // risky asset prices at time beginning

	riskhater.setup(bond_ret, asset_price_r);
	naiveinvestor.setup(bond_ret, asset_price_r);
	markowitzsavvy1.setup(bond_ret, asset_price_r);
	markowitzsavvy2.setup(bond_ret, asset_price_r);
	markowitzsavvy3.setup(bond_ret, asset_price_r);

	// loop for all subsequent time steps
	for (int t = 1; t < N; t++)
	{
		double bond_ret = bond_rets[t];								// bond return at time t
		std::vector<double> asset_price_r = asset_prices.getrow(t); // risky asset prices at time t

		// update
		riskhater.next_step(bond_ret, asset_price_r);
		naiveinvestor.next_step(bond_ret, asset_price_r);
		markowitzsavvy1.next_step(bond_ret, asset_price_r);
		markowitzsavvy2.next_step(bond_ret, asset_price_r);
		markowitzsavvy3.next_step(bond_ret, asset_price_r);

	}
	// header for csv file
	std::vector<std::string> header = {"riskhater_wealth", "riskhater_creturns",
									   "naive_wealth", "naive_creturns",
									   "markowitzsavvy1_wealth", "markowitzsavvy1_creturns",
									   "markowitzsavvy2_wealth", "markowitzsavvy2_creturns",
									   "markowitzsavvy3_wealth", "markowitzsavvy3_creturns"};

	// info to feed into csv file
	std::vector<double> riskhater_wealth = riskhater.getwealth();
	std::vector<double> riskhater_creturns = riskhater.getcumulativereturns();
	std::vector<double> naive_wealth = naiveinvestor.getwealth();
	std::vector<double> naive_creturns = naiveinvestor.getcumulativereturns();
	std::vector<double> markowitzsavvy1_wealth = markowitzsavvy1.getwealth();
	std::vector<double> markowitzsavvy1_creturns = markowitzsavvy1.getcumulativereturns();
	std::vector<double> markowitzsavvy2_wealth = markowitzsavvy2.getwealth();
	std::vector<double> markowitzsavvy2_creturns = markowitzsavvy2.getcumulativereturns();
	std::vector<double> markowitzsavvy3_wealth = markowitzsavvy3.getwealth();
	std::vector<double> markowitzsavvy3_creturns = markowitzsavvy3.getcumulativereturns();


	// create data to feed write file
	std::vector<std::vector<double>> data;

	// at start - wealth but no return.
	data.push_back({riskhater_wealth[0], 0.0, naive_wealth[0], 0.0, markowitzsavvy1_wealth[0], 0.0, markowitzsavvy2_wealth[0], 0.0, markowitzsavvy3_wealth[0], 0.0});

	for (size_t i = 1; i < riskhater_wealth.size(); i++)
	{
		data.push_back({riskhater_wealth[i], riskhater_creturns[i - 1],
						naive_wealth[i], naive_creturns[i - 1],
						markowitzsavvy1_wealth[i], markowitzsavvy1_creturns[i - 1],
						markowitzsavvy2_wealth[i], markowitzsavvy2_creturns[i - 1],
						markowitzsavvy3_wealth[i], markowitzsavvy3_creturns[i - 1]});
	}

	// Specify the output file path.
	std::string outputPath = "/Users/akshayzine/Academics/Programming_Group_Project/Akshay_Branch/group-project-medallion-fund/Strategy_Output.csv";

	// Write the CSV file.
	file.write(outputPath, header, data);

	// the final metrics for each agent.
	std::cout << "RiskHater Metrics:" << std::endl;
	std::cout << "RH: Final Wealth: " << riskhater.current_wealth() << std::endl;
	std::cout << "RH: Cumulative Return: " << riskhater_creturns[N - 2] << std::endl; // last index of N-1 length vector
	std::cout << "RH: Variance: " << riskhater.variance_return() << std::endl;
	std::cout << "RH: Sharpe Ratio: " << riskhater.sharpe_ratio() << std::endl;
	std::cout << std::endl;

	std::cout << "NaiveInvestor Final Metrics:" << std::endl;
	std::cout << "NI: Final Wealth: " << naiveinvestor.current_wealth() << std::endl;
	std::cout << "NI: Cumulative Return: " << naive_creturns[N - 2] << std::endl; // last index of N-1 length vector
	std::cout << "NI: Variance: " << naiveinvestor.variance_return() << std::endl;
	std::cout << "NI: Sharpe Ratio: " << naiveinvestor.sharpe_ratio() << std::endl;
	std::cout << std::endl;

	std::cout << "MarkowitzSavvy1 with target return " << target_return_1<< " Metrics:" << std::endl;
	std::cout << "MS1: Final Wealth: " << markowitzsavvy1.current_wealth() << std::endl;
	std::cout << "MS1: Cumulative Return: " << markowitzsavvy1_creturns[N - 2] << std::endl; // last index of N-1 length vector
	std::cout << "MS1: Variance: " << markowitzsavvy1.variance_return() << std::endl;
	std::cout << "MS1: Sharpe Ratio: " << markowitzsavvy1.sharpe_ratio() << std::endl;

	std::cout << "MarkowitzSavvy2 with target return " << target_return_2<< " Metrics:" << std::endl;
	std::cout << "MS2: Final Wealth: " << markowitzsavvy2.current_wealth() << std::endl;
	std::cout << "MS2: Cumulative Return: " << markowitzsavvy2_creturns[N - 2] << std::endl; // last index of N-1 length vector
	std::cout << "MS2: Variance: " << markowitzsavvy2.variance_return() << std::endl;
	std::cout << "MS2: Sharpe Ratio: " << markowitzsavvy2.sharpe_ratio() << std::endl;

	std::cout << "MarkowitzSavvy3 with target return " << target_return_3<< " Metrics:" << std::endl;
	std::cout << "MS3: Final Wealth: " << markowitzsavvy3.current_wealth() << std::endl;
	std::cout << "MS3: Cumulative Return: " << markowitzsavvy3_creturns[N - 2] << std::endl; // last index of N-1 length vector
	std::cout << "MS3: Variance: " << markowitzsavvy3.variance_return() << std::endl;
	std::cout << "MS3: Sharpe Ratio: " << markowitzsavvy3.sharpe_ratio() << std::endl;
}

#include <iostream>
#include "fileio.hpp"
#include  "matrix.hpp"
#include "general_agent.hpp"
#include "specialised_agents.hpp"

int main()
{
	FileIO file;
	std::string path = "/Users/akshayzine/Academics/Programming_Group_Project/matrix/Assets.csv"; 
 	auto [assetnames, assetvals] = file.read(path);

	//price matrix
	Matrix asset_vals (assetvals);

	//time-period N and number of risky assets M
	int N = asset_vals.NRows();
	int M = asset_vals.NCols()-1;

	//Bond Returns and Asset Prices
	std::vector<double> bond_rets = asset_vals.getcol(0); 
	Matrix asset_prices = asset_vals.slicecols(1,M+1);
	
	//Agents
	RiskHater riskhater(M,N);
	NaiveInvestor naiveinvestor(M,N);
	int l = 10; //lookback for markowitz
	MarkowitzSavvy markowitzsavvy(M,N,l);

	//setup/initialisation
	double bond_ret = bond_rets[0]; //bond return at time beginning
	std::vector<double> asset_price_r = asset_prices.getrow(0); //risky asset prices at time beginning
	
	riskhater.setup(bond_ret, asset_price_r);
    naiveinvestor.setup(bond_ret, asset_price_r);
    markowitzsavvy.setup(bond_ret, asset_price_r);

	//loop for all subsequent time steps
	for (int t = 1; t < N; t++)
	{
		double bond_ret = bond_rets[t]; //bond return at time t
		std::vector<double> asset_price_r = asset_prices.getrow(t); //risky asset prices at time t

		//update 
		riskhater.next_step(bond_ret, asset_price_r);
        naiveinvestor.next_step(bond_ret, asset_price_r);
        markowitzsavvy.next_step(bond_ret, asset_price_r);

	}

	

	std::cout << "Hello" << std::endl;
}


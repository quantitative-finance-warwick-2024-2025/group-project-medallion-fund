#pragma once
#include <iostream>
#include <vector>
#include <cmath>

// ALL RETURNS MUST BE IN DECIMAL FORM e.g 0.05 for 5% NOT 1.05 or 5

class Agent
{
protected:
  unsigned int current_period;
  unsigned int M; // Number of assets
  unsigned int N; // Number of periods

  // Store previous information about bonds, past_bond_returns[i] = return from period i to period i + 1
  std::vector<double> past_bond_returns;

  // Store previous information about assets
  std::vector<std::vector<double>> past_asset_prices;

  // Store previous information about asset positions of the agent, THESE ARE FRACTIONS OF WEALTH, M+1 array (includes bond in zeroth index)
  std::vector<std::vector<double>> positions;

  // Store wealth of the agent at each period
  std::vector<double> wealth;

  // Store returns of the agent at each period, returns[i] = return from period i to period i + 1
  std::vector<double> returns;

  // Store cumulative returns of the agent at each period, cumulative_returns[i] = return from period 0 to period i + 1
  std::vector<double> cumulative_returns;

public:
  Agent(unsigned int m, unsigned int n); // Constructor

  // Return the current wealth of the agent
  double current_wealth();

  // Return the return up to now of the agent, period_average = true returns the geometric mean return per period
  double total_return(bool period_average = false, int time1 = 0, int time2 = -1);

  // Return the variance of the returns of the agent
  double variance_return();

  // Return the Sharpe ratio of the agent
  double sharpe_ratio();

  // Update the position of the agent
  void setup(double bond_return, std::vector<double> asset_prices);

  // Update the position of the agent
  void next_step(double bond_return, std::vector<double> asset_prices);

  // Take in current prices and form initial position
  virtual std::vector<double> initial_position() = 0;

  // Take in current prices and form initial position
  virtual std::vector<double> update_position() = 0;

  // Functions for testing
  std::vector<std::vector<double>> get_positions() const;
  std::vector<double> getwealth() const;
  std::vector<double> get_returns() const;
  std::vector<double> getcumulativereturns() const;
};
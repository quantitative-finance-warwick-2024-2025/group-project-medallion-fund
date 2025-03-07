# include "general_agent.hpp"

Agent::Agent(unsigned int m, unsigned int n)
:
current_period(0),
M(m),
N(n),
past_bond_returns(N, 0.0),
past_asset_prices(N, std::vector<double>(M, 0.0)),
positions(N, std::vector<double>(M + 1, 0.0)),
wealth(N, 0.0),
returns(N - 1, 0.0) {}

double Agent::current_wealth() {
    return wealth[current_period];
}

// Defaults are defined in the header file
double Agent::total_return(bool period_average, int time1, int time2) {
    if (time2 == -1) {
        time2 = current_period; // Default to current period
    }

    if (time2 == 0 || time1 >= time2) {
        return 0.0; // If bad input, return 0
    }

    return period_average ? pow(wealth[time2] / wealth[time1], 1.0 / (time2 - time1)) - 1.0 : wealth[time2] / wealth[time1] - 1.0;
}

double Agent::variance_return() {
    if (current_period == 0) {
        return 0.0; // If bad input, return 0
    }

    double mean = 0;
    for (unsigned int i = 0; i < current_period; i++) {
        mean += returns[i];
    }
    mean /= current_period;

    double variance = 0;
    for (unsigned int i = 0; i < current_period; i++) {
        variance += pow(returns[i] - mean, 2);
    }
    variance /= (current_period - 1); // Adjusted sample variance

    return variance;
}

double Agent::sharpe_ratio() {
    if (current_period <= 1) {
        return 0.0; // If bad input, return 0
    }
    
    double r = 1;
    for (unsigned int i = 0; i < current_period - 1; i++) {
        r *= 1 + past_bond_returns[i];
    }
    r = pow(r, 1.0 / (current_period - 1)) - 1.0;

    return (total_return(true) - r) / sqrt(variance_return());
}

void Agent::setup(double bond_return, std::vector<double> asset_prices) {
    past_bond_returns[current_period] = bond_return;
    past_asset_prices[current_period] = asset_prices;
    positions[current_period] = initial_position();
    wealth[current_period] = 1.0;
}

void Agent::next_step(double bond_return, std::vector<double> asset_prices) {
    current_period++;
    
    past_bond_returns[current_period] = bond_return;
    past_asset_prices[current_period] = asset_prices;
    
    // If agent is bankrupt then do nothing
    if (wealth[current_period - 1] <= 0) {
        wealth[current_period] = 0;
        returns[current_period - 1] = 0;
        return;
    }

    double new_wealth = 0;

    double current_bond_value = 1.0;

    for (unsigned int i = 0; i < current_period; i++) {
        current_bond_value *= 1 + past_bond_returns[i];
    }

    new_wealth += positions[current_period - 1][0] * current_bond_value; // Bond return

    for (unsigned int i = 1; i < M + 1; i++) {
        new_wealth += positions[current_period - 1][i] * asset_prices[i - 1]; // Asset return
    }

    // If wealth is negative then agent is bankrupt and out of the simulation
    if (new_wealth <= 0) {
        new_wealth = 0.0;
        wealth[current_period] = 0.0;
        returns[current_period - 1] = 0.0;
        positions[current_period] = positions[current_period - 1]; // Not necessary but just in case
        return;
    }

    wealth[current_period] = new_wealth;
    returns[current_period - 1] = new_wealth / wealth[current_period - 1] - 1.0;

    positions[current_period] = update_position();
}

std::vector<std::vector<double>> Agent::get_positions() const {
    return positions;
}

std::vector<double> Agent::get_wealth() const {
    return wealth;
}

std::vector<double> Agent::get_returns() const {
    return returns;
}
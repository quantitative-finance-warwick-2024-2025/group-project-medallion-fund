# include "specialised_agents.hpp"
RiskHater::RiskHater(unsigned int m, unsigned int n) : Agent(m, n) {}

std::vector<double> RiskHater::initial_position() {
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0;
    return position;
}

std::vector<double> RiskHater::update_position() {
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0;
    return position;
}

////////////////

NaiveInvestor::NaiveInvestor(unsigned int m, unsigned int n) : Agent(m, n) {}

std::vector<double> NaiveInvestor::initial_position() {
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

std::vector<double> NaiveInvestor::update_position() {
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

////////////////

MarkowitzSavvy::MarkowitzSavvy(unsigned int m, unsigned int n, unsigned int l, double r) : Agent(m, n), lookback(l), target_return(r) {}

/////// write function for covariance matrix here please

// THE FUNCTIONS BELOW ARE PLACEHOLDERS AND ARE INCORRECT, I JUST NEEDED TO WRITE SOMETHING OR THE PROGRAM WOULDN'T COMPILE
std::vector<double> MarkowitzSavvy::initial_position() {
    std::vector<double> position(M + 1, 0.0);
    position[0] = 1.0;
    return position;
}

std::vector<double> MarkowitzSavvy::update_position() {
    std::vector<double> position(M + 1, 0.0);
    
    // Invest only in bonds for the first lookback periods
    if (current_period < lookback) {
        position[0] = 1.0;
        return position;
    }

    return position;
}
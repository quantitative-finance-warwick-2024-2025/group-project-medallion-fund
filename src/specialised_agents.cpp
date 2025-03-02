# include "specialised_agents.hpp"
RiskHater::RiskHater(unsigned int m) : Agent(m) {}

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

////

NaiveInvestor::NaiveInvestor(unsigned int m) : Agent(m) {}

std::vector<double> NaiveInvestor::initial_position() {
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

std::vector<double> NaiveInvestor::update_position() {
    std::vector<double> position(M + 1, 1.0 / (M + 1));
    return position;
}

////

MarkowitzSavvy::MarkowitzSavvy(unsigned int m, unsigned int l) : Agent(m), lookback(l) {}
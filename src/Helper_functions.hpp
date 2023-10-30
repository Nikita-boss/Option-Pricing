// Functions to define mesh, parameter matrix and help print option price

#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include "Payoff.hpp"
#include "AnalyticEuropeanEngine.hpp"
#include "AnalyticAmericanPerpetualEngine.hpp"
#include "VanillaOption.hpp"
#include "ExoticOption.hpp"
#include <vector>

namespace Helper_functions
{
	// Create a mesh
	std::vector<double> create_mesh(double min_value, double max_value, double step);
	// Create a matrix of parameters given underlying price, strike price, 
	// cost-of-carry parameter and vectorsof expiry time, volatility 
	// and risk-free rate
	std::vector<std::vector<double>> create_mesh_matrix(double S, double K,
														const std::vector<double>& expiry, 
														const std::vector<double>& volatility,
														const std::vector<double>& rate,
														double b=-1);
	// Compute call option price given parameter matrix
	std::vector<std::vector<double>> compute_call_option_prices(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute put option price given parameter matrix
	std::vector<std::vector<double>> compute_put_option_prices(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute call option delta given parameter matrix
	std::vector<std::vector<double>> compute_call_option_delta(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute put option delta given parameter matrix
	std::vector<std::vector<double>> compute_put_option_delta(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute call option gamma given parameter matrix
	std::vector<std::vector<double>> compute_call_option_gamma(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute put option gamma given parameter matrix
	std::vector<std::vector<double>> compute_put_option_gamma(const std::vector<std::vector<double>>& parameterMatrix);
	// Compute option price given matrix of parameters
	std::vector<std::vector<double>> compute_option_prices(const std::vector<std::vector<double>>& parameterMatrix, 
														   const PricingLibrary::Payoff::Type& type, const std::string& function);
	// Compute perpetual american option price
	std::vector<std::vector<double>> compute_perpetual_american_option_prices(const std::vector<std::vector<double>>& parameterMatrix,
                                                                              const PricingLibrary::Payoff::Type& type);
	// Print option prices
	void print_option_prices(const std::vector<std::vector<double>>& optionPrice);
};

#endif
// Implementation of header file Helper_functions.hpp

#include "Helper_functions.hpp"
#include "iostream"

namespace Helper_functions
{
	/// @brief Generate a mesh vector from min_value 
	/// till max_value with certain step
	/// @param min_value
	/// @param max_value 
	/// @param step mesh size
	/// @return vector of doubles
	std::vector<double> create_mesh(double min_value, double max_value, double step) 
	{
		int n_elements = (max_value - min_value) / step;
	
		std::vector<double> result(n_elements + 1);
		double val{min_value};

		for (long i = 0; i < n_elements + 1; i++)	
		{
			result[i] = val;
			val += step;
		}
	
		return result;
	}

	/// @brief Create parameter matrix
	/// @param S Underlying price
	/// @param K Strike price
	/// @param expiry vector
	/// @param volatility vector
	/// @param rate vector
	/// @param b cost-of-carry parameter
	/// @return parameter matrix
	std::vector<std::vector<double>> create_mesh_matrix(double S, double K,
														const std::vector<double>& expiry, 
														const std::vector<double>& volatility,
														const std::vector<double>& rate,
														double b)
	{
		// If b is -1, then it equals to rate, otherwise it has certain value
		bool tmp = (std::abs(b + 1.0) <= 0.01) ? 1:0;
		// Create result vector
		std::vector<std::vector<double>> res;
		// Iterate through all input parameter vectors
		for (const auto& T : expiry)
		{
			for (const auto& sigma : volatility)
			{
				for (const auto& r : rate)
				{
					if (tmp) 
					{
						b=r;
					}
					res.push_back({S, K, T, sigma, r, b});
				}
			}
		}
		return res;
	}

	/// @brief compute call option prices for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_call_option_prices(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Call, "price");
	}

	/// @brief compute put option prices for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_put_option_prices(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Put, "price");
	}

	/// @brief compute perpetual american option prices for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_perpetual_american_option_prices(const std::vector<std::vector<double>>& parameterMatrix,
																			  const PricingLibrary::Payoff::Type& type) 
	{
		std::vector<std::vector<double>> optionPrices;

		auto exercise = PricingLibrary::Payoff::American;
		for (const std::vector<double>& parameters : parameterMatrix) {

			double S = parameters[0];
			double K = parameters[1];
			double T = parameters[2];
			double sig = parameters[3];
			double r = parameters[4];
			double b = parameters[5];

			auto analytic_perpetual_engine{std::make_shared<PricingLibrary::AnalyticAmericanPerpetualEngine>(S, 
																	  sig, 
																	  r, 
																	  b)};
			auto payoff_perpetual{std::make_shared<PricingLibrary::Payoff>(T, 
									   K, type, exercise)};
		   PricingLibrary::ExoticOption perpetual_option{payoff_perpetual, analytic_perpetual_engine};
			double option_price = perpetual_option.getPrice();
			// Store the option price in the result matrix.
			optionPrices.push_back({S, K, T, sig, r, option_price});
		}

		return optionPrices;
	}

	/// @brief compute call option delta for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_call_option_delta(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Call, "delta");
	}

	/// @brief compute put option delta for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_put_option_delta(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Call, "delta");
	}

	/// @brief compute call option gamma for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_call_option_gamma(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Call, "gamma");
	}

	/// @brief compute put option gamma for a matrix of parameters.
	/// @param parameterMatrix 
	/// @return matrix
	std::vector<std::vector<double>> compute_put_option_gamma(const std::vector<std::vector<double>>& parameterMatrix) 
	{
		return compute_option_prices(parameterMatrix, PricingLibrary::Payoff::Call, "gamma");
	}

	/// @brief compute option prices for a matrix of parameters.
	/// @param parameterMatrix 
	/// @param type
	/// @param function what to compute
	/// @return vector with option prices
	std::vector<std::vector<double>> compute_option_prices(const std::vector<std::vector<double>>& parameterMatrix,
														   const PricingLibrary::Payoff::Type& type, const std::string& function) 
	{
		std::vector<std::vector<double>> optionPrices;

		for (const std::vector<double>& parameters : parameterMatrix) {

			double S = parameters[0];
			double K = parameters[1];
			double T = parameters[2];
			double sig = parameters[3];
			double r = parameters[4];
			double b = parameters[5];
			auto exercise = PricingLibrary::Payoff::European;

			auto analytic_engine{std::make_shared<PricingLibrary::AnalyticEuropeanEngine>(S, sig, r, b)};
			auto payoff_call{std::make_shared<PricingLibrary::Payoff>(T, K, type, exercise)};
			PricingLibrary::VanillaOption myEuropeanOption{payoff_call, analytic_engine};
			double res{};
			if (function == "price")
			{
				res = myEuropeanOption.getPrice();
			}
			else if (function == "delta")
			{
				res = myEuropeanOption.getDelta();
			}
			else if (function == "gamma")
			{
				res = myEuropeanOption.getGamma();
			}
			// Store the option price in the result matrix.
			optionPrices.push_back({S, K, T, sig, r, res});
		}

		return optionPrices;
	}

	/// @brief print option price with parameter values
	/// @param optionPrice 
	void print_option_prices(const std::vector<std::vector<double>>& optionPrice)
	{
		const int fieldWidth = 5;
		for (const std::vector<double>& parameters : optionPrice) {
			std::cout << "Underlying: " << std::setw(fieldWidth) << parameters[0] << " | ";
			std::cout << "Strike: " << std::setw(fieldWidth) << parameters[1] << " | ";
			std::cout << "Expiration: " << std::setw(fieldWidth) << parameters[2] << " | ";
			std::cout << "Volatility: " << std::setw(fieldWidth) << parameters[3] << " | ";
			std::cout << "Risk-free rate: " << std::setw(fieldWidth) << parameters[4] << " | ";
			std::cout << "Option price: " << std::setw(fieldWidth) << parameters[5] << '\n';
		}
	}
}
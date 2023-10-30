// Program to compute exact Vanilla option price, greeks 
// and price of perpetual American options
//
// Compiled from terminal: 
// clang++ -std=c++20 -Wall -Wpedantic -Weffc++ Main.cpp AnalyticEuropeanEngine.cpp 
// Option.cpp Payoff.cpp PricingEngine.cpp VanillaOption.cpp Helper_functions.cpp 
// NumericalEuropeanEngine.cpp ExoticOption.cpp AnalyticAmericanPerpetualEngine.cpp -o Main

#include "Payoff.hpp"
#include "VanillaOption.hpp"
#include "ExoticOption.hpp"

#include "AnalyticEuropeanEngine.hpp"
#include "NumericalEuropeanEngine.hpp"
#include "AnalyticAmericanPerpetualEngine.hpp"

#include "Helper_functions.hpp"

#include <iostream>
#include <memory>
#include <vector>

using namespace PricingLibrary;
using namespace Helper_functions;

int main()
{
	/************************ Part A. Exact Solutions of One-Factor Plain Options ************************/
	std::cout << "Part A. Exact Solutions of One-Factor Plain Options\n";
	// Test Batches
	std::vector<double>	T_vector{0.25, 1.0, 1.0, 30.0};		// Expiry time
	std::vector<double>	K_vector{65.0, 100.0, 10.0, 100.0};	// Strike price
	std::vector<double>	S_vector{60.0, 100.0, 5.0, 100.0};	// Underlying price
	std::vector<double>	sig_vector{0.3, 0.2, 0.5, 0.3};		// Volatility
	std::vector<double>	r_vector{0.08, 0.0, 0.12, 0.08};	// Risk-free rate

	/*
	The cost-of-carry parameter b has specific values depending on the kind of security in question: 
	• b = r is the Black–Scholes stock option model. 
	• b = r - q is the Morton model with continuous dividend yield q. 
	• b = 0 is the Black–Scholes futures option model. 
	• b = r - R is the  Garman and Kohlhagen currency option model, where R is the foreign risk-free interest 
	rate. 
	*/
	// Test for Black–Scholes stock option model, therefore, cost of carry parameter, b,
	// is equal to risk-free rate r (b=r)
	std::vector<double> b_vector{r_vector};

	// Store option price vectors
	std::vector<double> call_price_vector;
	std::vector<double> put_price_vector;

	/************************ a) ************************/
	// Put price is calculated using Put-Call parity, 
	// so that put and call prices always satisfy the parity.
	// Implementing put-call parity as a separate member function would 
	// either cause cyclical dependence problem or be inelegant solution
	// given code design. 
	
	// Test Call options
	std::cout << "Question a): Price Call and Put Options on Batches 1 to 4\n\n";

	// Define type of the option: Call or Put
	Payoff::Type type{Payoff::Call};
	// Define exercise type of the option: European
	Payoff::Exercise exercise(Payoff::European);
	// Iterate through batches of data to determine call price
	std::cout << "European call prices\n";
	for (int i{}; i < T_vector.size(); i++)
	{
		// Define pricing engine for price calculation. In this case it is analytic
		auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(S_vector[i], 
																	  sig_vector[i], 
																	  r_vector[i], 
																	  b_vector[i])};
		// Define payoff object of the option
		auto payoff_call{std::make_shared<Payoff>(T_vector[i], K_vector[i], type, exercise)};
		// Define option
		VanillaOption myEuropeanCall{payoff_call, analytic_engine};
		double price = myEuropeanCall.getPrice();
		call_price_vector.push_back(price);
		std::cout << "Batch: " << i + 1 << ". Call: " << price;
		std::cout << ". (K=" << K_vector[i] << ", S=" << S_vector[i] << ", T=" << T_vector[i]
				  << ", sigma=" << sig_vector[i] << ", r=" << r_vector[i] 
				  << ", b=" << b_vector[i] << ")\n";
	}

	// Test Put options
	type = Payoff::Put;
	// Iterate through batches of data to determine put price
	std::cout << "European put prices\n";
	for (int i{}; i < T_vector.size(); i++)
	{
		auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(S_vector[i], 
																	  sig_vector[i], 
																	  r_vector[i], 
																	  b_vector[i])};
		auto payoff_put{std::make_shared<Payoff>(T_vector[i], K_vector[i], type, exercise)};
		VanillaOption myEuropeanPut{payoff_put, analytic_engine};
		double price = myEuropeanPut.getPrice();
		put_price_vector.push_back(price);
		std::cout << "Batch: " << i + 1 << ". Put: " << price;
		std::cout << ". (K=" << K_vector[i] << ", S=" << S_vector[i] << ", T=" << T_vector[i]
				  << ", sigma=" << sig_vector[i] << ", r=" << r_vector[i] << ")\n";
	}

	// Test on American options
	// Should give EngineException
	std::cout << '\n';
	try
	{
		auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(S_vector[0], 
																	  sig_vector[0], 
																	  r_vector[0], 
																	  b_vector[0])};
		auto payoff_american{std::make_shared<Payoff>(T_vector[0], K_vector[0], type, Payoff::American)};
		VanillaOption myAmericanCall{payoff_american, analytic_engine};
		std::cout << "American Call option price: " << myAmericanCall.getPrice() << '\n';
	}
	catch(const EngineException& e)
	{
		std::cerr << e.what() << "\n\n";
	}

	/************************ b) ************************/
	std::cout << "\nPart B: Check if a given set of put/call prices satisfy parity\n\n";
	for (int i{}; i < call_price_vector.size(); i++)
	{
		bool answer = AnalyticEuropeanEngine::satisfy_put_call_parity(call_price_vector[i],
																	 put_price_vector[i],
																	 K_vector[i],
																	 S_vector[i],
																	 r_vector[i],
																	 T_vector[i]);
		std::cout << "Batch: " << i + 1 << ", Put-call parity is " << ((answer) ? "satisfied":"not satisfied") << '\n';
	}

	/************************ c) ************************/
	std::cout << "\n\nPart C: Price Call and Put Options with strike price from 10 to 50 step 1 on Batches 1 to 4\n\n";
	// Generate mesh vector
	auto underlying_price_vector = create_mesh(10.0, 50.0, 1.0);
	// Clear call and put price vectors
	call_price_vector.clear();
	put_price_vector.clear();
	// Create option price matrix
	std::vector<std::vector<double>> option_chain;

	// Call option price
	type = Payoff::Call;
	for (int i{}; i < T_vector.size(); i++)
	{
		for (auto& price : underlying_price_vector)
		{
			auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(price, 
																		  sig_vector[i], 
																		  r_vector[i], 
																		  b_vector[i])};
			auto payoff_call{std::make_shared<Payoff>(T_vector[i], K_vector[i], type, exercise)};
			VanillaOption myEuropeanCall{payoff_call, analytic_engine};
			double option_price = myEuropeanCall.getPrice();
			// Append price to option price vector
			call_price_vector.push_back(option_price);
			// Append price to the option_chain matrix with parameters
			option_chain.push_back({price, K_vector[i], T_vector[i], sig_vector[i], r_vector[i], option_price});
		}
	}
	// Print call option price
	std::cout << "European call prices for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	// Put option price
	type = Payoff::Put;
	for (int i{}; i < T_vector.size(); i++)
	{
		for (auto& price : underlying_price_vector)
		{
			auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(price, 
																		  sig_vector[i], 
																		  r_vector[i], 
																		  b_vector[i])};
			auto payoff_put{std::make_shared<Payoff>(T_vector[i], K_vector[i], type, exercise)};
			VanillaOption myEuropeanPut{payoff_put, analytic_engine};
			double option_price = myEuropeanPut.getPrice();
			// Append price to option price vector
			put_price_vector.push_back(option_price);
			// Append price to the option_chain matrix with parameters
			option_chain.push_back({price, K_vector[i], T_vector[i], sig_vector[i], r_vector[i], option_price});
		}
	}
	// Print put option price
	std::cout << "\nEuropean put prices for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	/************************ d) ************************/
	// compute option prices as a function of expiry time, volatility and risk-free rate
	std::cout << "\n\nPart D: Compute call option prices as a function of expiry time, volatility and risk-free rate.\n";
	// Create mesh of expiry time with 0.25 and 0.5
	std::vector<double> expiry_time = create_mesh(0.25, 0.5, 0.25);
	// Create mesh of volatility with 0.1 and 0.2
	std::vector<double> volatility = create_mesh(0.1, 0.2, 0.1);
	// Create mesh of risk-free rate with 0.04 and 0.08
	std::vector<double> rate = create_mesh(0.04, 0.08, 0.04);
	// Create a matrix of parameters
	for (int i{}; i < S_vector.size(); i++)
	{
		std::vector<std::vector<double>> option_parameter_matrix = create_mesh_matrix(S_vector[i],
																					  K_vector[i],
																					  expiry_time,
																					  volatility,
																					  rate);
		// Compute call option price given parameter matrix
		option_chain = compute_call_option_prices(option_parameter_matrix);
		// Print call option prices
		std::cout << "\nCall options\n";
		print_option_prices(option_chain);
		// Compute put option price given parameter matrix
		option_chain = compute_put_option_prices(option_parameter_matrix);
		// Print put option prices
		std::cout << "\nPut options\n";
		print_option_prices(option_chain);
	}


	/************************ Part A. Option Sensitivities, aka the Greeks ************************/
	std::cout << "\n\nPart A. Option Sensitivities, aka the Greeks.\n";
	/************************ a) ************************/
	std::cout << "Question a) Implement greek functions.\n\n";
	type = Payoff::Call;
	double K{100}, S{105}, T{0.5}, r{0.1}, b{0.0}, sig{0.36};
	// Save delta and gamma results for question d
	double call_delta{}, call_gamma{}, put_delta{}, put_gamma{}; 
	auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(S, sig, r, b)};
	auto payoff_call{std::make_shared<Payoff>(T, K, type, exercise)};
	VanillaOption myEuropeanCall{payoff_call, analytic_engine};
	std::cout << "Greeks for S=" << S << ", K=" << K << ", T=" << T
			  << ", Volatility=" << sig << ", risk-free rate=" << r 
			  << ", b=" << b <<  '\n';
	call_delta = myEuropeanCall.getDelta();
	call_gamma = myEuropeanCall.getGamma();
	std::cout << "European Call option delta: " << call_delta << '\n';
	std::cout << "European Call option gamma: " << call_gamma << '\n';
	std::cout << "European Call option vega: " << myEuropeanCall.getVega() << '\n';
	std::cout << "European Call option theta: " << myEuropeanCall.getTheta() << '\n';

	auto payoff_put{std::make_shared<Payoff>(T, K, Payoff::Put, exercise)};
	VanillaOption myEuropeanPut{payoff_put, analytic_engine};
	put_delta = myEuropeanPut.getDelta();
	put_gamma = myEuropeanPut.getGamma();
	std::cout << "European Put option delta: " << put_delta << '\n';
	std::cout << "European Put option gamma: " << put_gamma << '\n';
	std::cout << "European Put option vega: " << myEuropeanPut.getVega() << '\n';
	std::cout << "European Put option theta: " << myEuropeanPut.getTheta() << '\n';


	/************************ b) ************************/
	std::cout << "\n\nQuestion b) output delta for range of underlying price.\n\n";
	underlying_price_vector = create_mesh(10.0, 50.0, 1.0);
	// Create option price matrix
	option_chain.clear();

	// Call option delta
	type = Payoff::Call;
	for (auto& price : underlying_price_vector)
	{
		auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(price, 
																	  sig, 
																	  r, 
																	  b)};
		auto payoff_call{std::make_shared<Payoff>(T, K, type, exercise)};
		VanillaOption myEuropeanCall{payoff_call, analytic_engine};
		double delta = myEuropeanCall.getDelta();
		// Append delta to the option_chain matrix with parameters
		option_chain.push_back({price, K, T, sig, r, delta});
	}

	// Print call option delta
	std::cout << "European call delta for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	// Put option price
	type = Payoff::Put;
	for (auto& price : underlying_price_vector)
	{
		auto analytic_engine{std::make_shared<AnalyticEuropeanEngine>(price, 
																	  sig, 
																	  r, 
																	  b)};
		auto payoff_put{std::make_shared<Payoff>(T, K, type, exercise)};
		VanillaOption myEuropeanPut{payoff_put, analytic_engine};
		double delta = myEuropeanPut.getDelta();
		// Append delta to the option_chain matrix with parameters
		option_chain.push_back({price, K, T, sig, r, delta});
	}
	// Print put option price
	std::cout << "\nEuropean put delta for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	/************************ c) ************************/
	// Compute call option delta and put option gamma as a 
	//function of expiry time, volatility and risk-free rate
	std::cout << "\n\nQuestion c): Compute call option delta as" 
			  << "a function of expiry time, volatility and risk-free rate.\n";
	// Create a matrix of parameters
	std::vector<std::vector<double>> option_parameter_matrix = create_mesh_matrix(S,
																				  K,
																				  expiry_time,
																				  volatility,
																				  rate,
																				  b);
	// Compute call option delta given parameter matrix
	option_chain = compute_call_option_delta(option_parameter_matrix);
	// Print call option delta
	std::cout << "\nCall option delta.\n";
	print_option_prices(option_chain);
	// Compute put option gamma given parameter matrix
	option_chain = compute_put_option_gamma(option_parameter_matrix);
	// Print put option gamma
	std::cout << "\nPut option gamma\n";
	print_option_prices(option_chain);


	/************************ d) ************************/
	// Compute delta and gamma numerically using NumericalEuropeanEngine
	std::cout << "\n\nQuestion d): Compute call option delta and gamma using numerical method.\n";
	type = Payoff::Call;
	auto numerical_engine = std::make_shared<NumericalEuropeanEngine>(S, sig, r, b);
	payoff_call = std::make_shared<Payoff>(T, K, type, exercise);
	VanillaOption myEuropeanCall1{payoff_call, numerical_engine};
	std::cout << "Delta for S=" << S << ", K=" << K << ", T=" << T
			  << ", Volatility=" << sig << ", risk-free rate=" << r << '\n';
	std::cout << "European numerical Call option delta. numerical: " << myEuropeanCall1.getDelta() 
			  << ", exact: " << call_delta << ", error: " 
			  << std::abs(call_delta - myEuropeanCall1.getDelta()) << '\n';
	std::cout << "European Call option gamma. numerical " << myEuropeanCall1.getGamma()
			  << ", exact: " << call_gamma << ", error: " 
			  << std::abs(call_gamma - myEuropeanCall1.getGamma()) << '\n';
	type = Payoff::Put;
	payoff_put = std::make_shared<Payoff>(T, K, type, exercise);
	VanillaOption myEuropeanPut1{payoff_put, numerical_engine};
	std::cout << "European Put option delta. numerical: " << myEuropeanPut1.getDelta()
			  << ", exact: " << put_delta << ", error: " 
			  << std::abs(put_delta - myEuropeanPut1.getDelta()) << '\n';
	std::cout << "European Put option gamma. numerical " << myEuropeanPut1.getGamma()
			  << ", exact: " << put_gamma << ", error: " 
			  << std::abs(put_gamma - myEuropeanPut1.getGamma()) << '\n';


	/************************ Part B. Perpetual American Options ************************/
	std::cout << "\n\nPart B. Perpetual American Options.\n";
	/************************ b) ************************/
	std::cout << "Price perpetual American options for K=100, sig=0.1, r=0.1, b=0.02, S=110\n";
	K = 100, S = 110, r = 0.1, b = 0.02, sig= 0.1;
	type = Payoff::Call;
	exercise = Payoff::American;
	auto analytic_perpetual_engine = std::make_shared<AnalyticAmericanPerpetualEngine>(S, sig, r, b);
	auto payoff_perpetual_call = std::make_shared<Payoff>(std::numeric_limits<double>::quiet_NaN(), K, type, exercise);
	ExoticOption perpetual_call_option{payoff_perpetual_call, analytic_perpetual_engine};
	type = Payoff::Put;
	auto payoff_perpetual_put = std::make_shared<Payoff>(std::numeric_limits<double>::quiet_NaN(), K, type, exercise);
	ExoticOption perpetual_put_option{payoff_perpetual_put, analytic_perpetual_engine};
	std::cout << "Call price: " << perpetual_call_option.getPrice() << '\n';
	std::cout << "Put price: " << perpetual_put_option.getPrice() << '\n';

	/************************ c) ************************/
	std::cout << "\n\nQuestion c) output american perpetual option price for range of underlying price.\n\n";
	underlying_price_vector = create_mesh(10.0, 50.0, 1.0);
	// Create option price matrix
	option_chain.clear();

	// American perpetual call option price
	type = Payoff::Call;
	for (auto& price : underlying_price_vector)
	{
		auto analytic_perpetual_engine{std::make_shared<AnalyticAmericanPerpetualEngine>(price, 
																	  sig, 
																	  r, 
																	  b)};
		auto payoff_perpetual_call{std::make_shared<Payoff>(std::numeric_limits<double>::quiet_NaN(), 
								   K, type, exercise)};
		ExoticOption perpetual_call_option{payoff_perpetual_call, analytic_perpetual_engine};
		double option_price = perpetual_call_option.getPrice();
		// Append price to the option_chain matrix with parameters
		option_chain.push_back({price, K, std::numeric_limits<double>::quiet_NaN(), 
								sig, r, option_price});
	}
	
	// Print call option delta
	std::cout << "American perpetual call price for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	// American perpetual put option price
	type = Payoff::Put;
	for (auto& price : underlying_price_vector)
	{
		auto analytic_perpetual_engine{std::make_shared<AnalyticAmericanPerpetualEngine>(price, 
																	  					 sig, 
																	  					 r, 
																	  					 b)};
		auto payoff_perpetual_put{std::make_shared<Payoff>(std::numeric_limits<double>::quiet_NaN(), 
								   K, type, exercise)};
		ExoticOption perpetual_put_option{payoff_perpetual_put, analytic_perpetual_engine};
		double option_price = perpetual_put_option.getPrice();
		// Append price to the option_chain matrix with parameters
		option_chain.push_back({price, K, std::numeric_limits<double>::quiet_NaN(), 
								sig, r, option_price});
	}
	// Print american perpetual put option price
	std::cout << "\nAmerican perpetual put price for underlying price from 10 to 50\n";
	print_option_prices(option_chain);
	option_chain.clear();

	/************************ d) ************************/
	// Compute call and put option price as a function of expiry time, volatility and risk-free rate
	std::cout << "\n\nQuestion d): Compute call and put option price as a function of expiry time, volatility and risk-free rate.\n";
	// Create a matrix of parameters
	S = 110, K = 100, b = 0.02;
	option_parameter_matrix = create_mesh_matrix(S, K, expiry_time, volatility, rate, b);
	// Compute call option price given parameter matrix
	option_chain = compute_perpetual_american_option_prices(option_parameter_matrix, Payoff::Call);
	// Print call option delta
	std::cout << "\nCall option price.\n";
	print_option_prices(option_chain);
	// Compute put option gamma given parameter matrix
	option_chain = compute_perpetual_american_option_prices(option_parameter_matrix, Payoff::Put);
	// Print put option gamma
	std::cout << "\nPut option price\n";
	print_option_prices(option_chain);

	return 0;
}
// Define engine to price european vanilla options analytically

#ifndef ANALYTICEUROPEANENGINE_HPP
#define ANALYTICEUROPEANENGINE_HPP

#include "PricingEngine.hpp"
#include "EngineException.hpp"
#include "IncorrectEngineException.hpp"

#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace PricingLibrary {

	class AnalyticEuropeanEngine : public PricingEngine
	{
	private:
		double _S;        // underlying price
		double _sigma;   // volatility
		double _r;       // risk-free rate
		double _b;       // cost of carry

		// Get call option price
		double getEngineCallPrice(const std::shared_ptr<Payoff>& payoff) const;
		// Get put option price
		double getEnginePutPrice(const std::shared_ptr<Payoff>& payoff) const;

	public:
		AnalyticEuropeanEngine(double S, double sigma, double r, double b); // default constructor
		AnalyticEuropeanEngine(const AnalyticEuropeanEngine& source); // copy constructor
		AnalyticEuropeanEngine& operator= (const AnalyticEuropeanEngine& source); // copy assignment
		~AnalyticEuropeanEngine(); // destructor

		// Get option price, calls appropriately either getEngineCallPrice 
		// or getEnginePutPrice
		double getEnginePrice(const std::shared_ptr<Payoff>& payoff) const override;
		// Calculate Greeks
		double getEngineDelta(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineGamma(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineVega(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineTheta(const std::shared_ptr<Payoff>& payoff) const override;

		// Validate if engine was passed to a correct option
		void validate(const Payoff::Exercise& exercise) const override;
		// Put-call parity
		static bool satisfy_put_call_parity(double call, double put, double K, 
											double S, double r, double T,
											double epsilon=1e-6);
	};
}

#endif
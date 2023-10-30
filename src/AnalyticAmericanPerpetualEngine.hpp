// Define engine to price american perpetual options analytically

#ifndef ANALYTICAMERICANPERPETUALENGINE_HPP
#define ANALYTICAMERICANPERPETUALENGINE_HPP

#include "PricingEngine.hpp"
#include "EngineException.hpp"
#include "IncorrectEngineException.hpp"

#include <cmath>

namespace PricingLibrary {

	class AnalyticAmericanPerpetualEngine : public PricingEngine
	{
	private:
		double _S;        // underlying price
		double _sigma;    // volatility
		double _r;        // risk-free rate
		double _b;        // cost of carry

		// Get call option price
		double getEngineCallPrice(const std::shared_ptr<Payoff>& payoff) const;
		// Get put option price
		double getEnginePutPrice(const std::shared_ptr<Payoff>& payoff) const;

		public:
		AnalyticAmericanPerpetualEngine(double S, double sigma, double r, double b); // default constructor
		AnalyticAmericanPerpetualEngine(const AnalyticAmericanPerpetualEngine& source); // copy constructor
		AnalyticAmericanPerpetualEngine& operator= (const AnalyticAmericanPerpetualEngine& source); // copy assignment
		~AnalyticAmericanPerpetualEngine();	// destructor

		// Get option price, calls appropriately either getEngineCallPrice 
		// or getEnginePutPrice
		double getEnginePrice(const std::shared_ptr<Payoff>& payoff) const override;
		// Not implemented calculation of greeks
		double getEngineDelta(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineGamma(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineTheta(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineVega(const std::shared_ptr<Payoff>& payoff) const override;
		// Validate if engine was passed to a correct option
		void validate(const Payoff::Exercise& exercise) const override;
	};
}

#endif
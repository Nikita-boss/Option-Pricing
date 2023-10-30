// General engine class from which specific pricing engine class are derived:
// AnalyticEuropeanEngine, NumericalEuropeanEngine, AnalyticAmericanPerpetualEngine

#ifndef PRICINGENGINE_HPP
#define PRICINGENGINE_HPP

#include "Payoff.hpp"
#include <memory>

namespace PricingLibrary {

	class PricingEngine
	{
	public:
		PricingEngine() =default; // default constructor
		virtual ~PricingEngine(); // destructor

		// Get option price
		virtual double getEnginePrice(const std::shared_ptr<Payoff>& payoff) const =0;
		// Get option Greeks
		virtual double getEngineDelta(const std::shared_ptr<Payoff>& payoff) const =0;
		virtual double getEngineGamma(const std::shared_ptr<Payoff>& payoff) const =0;
		virtual double getEngineTheta(const std::shared_ptr<Payoff>& payoff) const =0;
		virtual double getEngineVega(const std::shared_ptr<Payoff>& payoff) const =0;
		// Validate if engine was passed to a correct option
		virtual void validate(const Payoff::Exercise& exercise) const =0;
	};
}

#endif
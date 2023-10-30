// Define engine to price european vanilla options numerically

#ifndef NUMERICALEUROPEANENGINE_HPP
#define NUMERICALEUROPEANENGINE_HPP

#include "PricingEngine.hpp"
#include "EngineException.hpp"
#include "IncorrectEngineException.hpp"

#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace PricingLibrary {

	class NumericalEuropeanEngine : public PricingEngine
	{
	private:
		double _S;       // underlying price
		double _sigma;   // volatility
		double _r;       // risk-free rate
		double _b;       // cost of carry
		double _h; // greeks precision parameter

		// Analytic call price
		double getAnalyticCallPrice(const std::shared_ptr<Payoff>& payoff, double S_new) const;
		// Analytic put price
		double getAnalyticPutPrice(const std::shared_ptr<Payoff>& payoff, double S_new) const;
		// Analytic call delta
		double getEngineCallDelta(const std::shared_ptr<Payoff>& payoff) const;
		// Analytic put price
		double getEnginePutDelta(const std::shared_ptr<Payoff>& payoff) const;
		// Analytic call gamma
		double getEngineCallGamma(const std::shared_ptr<Payoff>& payoff) const;
		// Analytic put gamma
		double getEnginePutGamma(const std::shared_ptr<Payoff>& payoff) const;

	public:
		NumericalEuropeanEngine(double S, double sigma, double r, double b); // default constructor
		NumericalEuropeanEngine(const NumericalEuropeanEngine& source);	// copy constructor
		NumericalEuropeanEngine& operator= (const NumericalEuropeanEngine& source); // copy assignment
		~NumericalEuropeanEngine(); // default destructor

		// Validate if engine was passed to a correct option
		void validate(const Payoff::Exercise& exercise) const override;

		// Price: not implemented
		double getEnginePrice(const std::shared_ptr<Payoff>& payoff) const override;

		// Calculate Greeks
		double getEngineDelta(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineGamma(const std::shared_ptr<Payoff>& payoff) const override;
		// Below are not implemented
		double getEngineTheta(const std::shared_ptr<Payoff>& payoff) const override;
		double getEngineVega(const std::shared_ptr<Payoff>& payoff) const override;
	};
}

#endif
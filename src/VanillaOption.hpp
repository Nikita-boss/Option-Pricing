// Vanilla option class to price and get greeks of vanilla options
// Has pricing engine and payoff as member variables
// pricing engine is used to return option price and greeks
// payoff includes data such as maturity, strike, option type and exercise

#ifndef VANILLAOPTION_HPP
#define VANILLAOPTION_HPP

#include "Option.hpp"
#include "Payoff.hpp"
#include "PricingEngine.hpp"
#include "AnalyticEuropeanEngine.hpp"

#include <memory>

namespace PricingLibrary {

	class VanillaOption : public Option
	{
	private:
		std::shared_ptr<PricingEngine> _engine; // pricing engine

	public:
		explicit VanillaOption(const std::shared_ptr<Payoff>& payoff); // default constructor
		// construct with Payoff object and Engine object
		VanillaOption(const std::shared_ptr<Payoff>& payoff, const std::shared_ptr<PricingEngine>& engine);
		VanillaOption(const VanillaOption& source); // copy constructor
		VanillaOption& operator= (const VanillaOption& source); // copy assignment
		~VanillaOption(); // default destructor

		// Set pricing engine
		void setEngine(const std::shared_ptr<PricingEngine>& engine);

		// Option price
		double getPrice() const override;
		// Greeks
		double getDelta() const;
		double getGamma() const;
		double getVega() const;
		double getTheta() const;
	};
}

#endif
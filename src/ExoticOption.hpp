// Exotic option class to price perpetual american options
// Has pricing engine and payoff as member variables
// pricing engine is used to return option price and greeks
// payoff includes data such as maturity, strike, option type and exercise

#ifndef EXOTICOPTION_HPP
#define EXOTICOPTION_HPP

#include "Option.hpp"
#include "Payoff.hpp"
#include "PricingEngine.hpp"
#include "AnalyticAmericanPerpetualEngine.hpp"

#include <memory>

namespace PricingLibrary {

	class ExoticOption : public Option
	{
	private:
		std::shared_ptr<PricingEngine> _engine; // pricing engine

	public:
		explicit ExoticOption(const std::shared_ptr<Payoff>& payoff); // default constructor
		// construct with Payoff object and Engine object
		ExoticOption(const std::shared_ptr<Payoff>& payoff, const std::shared_ptr<PricingEngine>& engine);
		ExoticOption(const ExoticOption& source); // copy constructor
		ExoticOption& operator= (const ExoticOption& source); // copy assignment
		~ExoticOption();  // default destructor

		// Set pricing engine
		void setEngine(const std::shared_ptr<PricingEngine>& engine);

		// Option price
		double getPrice() const override;
	};
}

#endif